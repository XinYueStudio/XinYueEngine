#include "pch.h"
#include "App.h"

#include <ppltasks.h>
#include "BasicTimer.h"
using namespace XinYueEngineUWP;
 
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Input;

// 主函数仅用于初始化我们的 IFrameworkView 类。
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App();
}

App::App() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

// 创建 IFrameworkView 时调用的第一个方法。
void App::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	m_renderer = ref new Sample3DSceneRenderer();
}

// 创建(或重新创建) CoreWindow 对象时调用。
void App::SetWindow(CoreWindow^ window)
{
	window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyDown);

	DisplayInformation::GetForCurrentView()->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &App::OnDpiChanged);

	// Disable all pointer visual feedback for better performance when touching.
	auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
	pointerVisualizationSettings->IsContactFeedbackEnabled = false;
	pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

	m_renderer->Initialize(window, DisplayInformation::GetForCurrentView()->LogicalDpi);
}

// 初始化场景资源或加载之前保存的应用程序状态。
void App::Load(Platform::String^ entryPoint)
{
	 
}

// 将在窗口处于活动状态后调用此方法。
void App::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// render the mono content or the left eye view of the stereo content
			m_renderer->Update(0, timer->Total, timer->Delta);
			m_renderer->RenderEye(0);
			// render the right eye view of the stereo content
			if (m_renderer->GetStereoEnabledStatus())
			{
				m_renderer->Update(1, timer->Total, timer->Delta);
				m_renderer->RenderEye(1);
			}
			m_renderer->Present(); // this call is sychronized to the display frame rate
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}

}

// IFrameworkView 所必需的。
// 终止事件不会导致调用 Uninitialize。如果在应用程序在前台运行时销毁 IFrameworkView
// 类，则将调用该方法。
void App::Uninitialize()
{
}

// 应用程序生命周期事件处理程序。

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	if (args->Kind == ActivationKind::Launch)
	{
		// Load previously saved state only if the application shut down cleanly last time.
		if (args->PreviousExecutionState != ApplicationExecutionState::NotRunning)
		{
			// When this application is suspended, it stores the drawing state.
			// This code attempts to restore the saved state.
			IPropertySet^ set = ApplicationData::Current->LocalSettings->Values;
			// an int called StereoExaggerationFactor is used as a key
			if (set->HasKey("StereoExaggerationFactor"))
			{
				float tempStereoExaggerationFactor = (safe_cast<IPropertyValue^>(set->Lookup("StereoExaggerationFactor")))->GetSingle();
				m_renderer->SetStereoExaggeration(tempStereoExaggerationFactor);
			}
		}
	}
	else
	{
		DX::ThrowIfFailed(E_UNEXPECTED);
	}
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// This is also a good time to save your application's state in case the process gets terminated.
  // That way, when the user relaunches the application, they will return to the position they left.
	IPropertySet^ settingsValues = ApplicationData::Current->LocalSettings->Values;
	if (settingsValues->HasKey("StereoExaggerationFactor"))
	{
		settingsValues->Remove("StereoExaggerationFactor");
	}

	float tempStereoExaggerationFactor = m_renderer->GetStereoExaggeration();
	settingsValues->Insert("StereoExaggerationFactor", PropertyValue::CreateSingle(tempStereoExaggerationFactor));

	// Hint to the driver that the app is entering an idle state and that its memory
	// can be temporarily used for other apps.
	m_renderer->Trim();
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// 还原在挂起时卸载的任何数据或状态。默认情况下，
	// 在从挂起中恢复时，数据和状态会持续保留。请注意，
	// 如果之前已终止应用程序，则不会发生此事件。

	// 在此处插入代码。
}

// 窗口事件处理程序。

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	Windows::UI::ViewManagement::ApplicationView^ View = Windows::UI::ViewManagement::ApplicationView::GetForCurrentView();

	if (View->IsFullScreen && View->IsFullScreenMode)
		View->TryEnterFullScreenMode();

	m_windowVisible = args->Visible;
}

void XinYueEngineUWP::App::OnKeyDown(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
{
	
		Windows::System::VirtualKey Key;
		Key = args->VirtualKey;

		// if the image is in stereo, adjust for user keystrokes increasing/decreasing the stereo effect
		if (m_renderer->GetStereoEnabledStatus())
		{
			float stereoExaggeration = m_renderer->GetStereoExaggeration();
			// figure out the command from the keyboard
			if (Key == VirtualKey::Up)             // increase stereo effect
			{
				stereoExaggeration += 0.1f;
			}
			if (Key == VirtualKey::Down)           // descrease stereo effect
			{
				stereoExaggeration -= 0.1f;
			}
			stereoExaggeration = min(stereoExaggeration, 4.0f);
			stereoExaggeration = max(stereoExaggeration, -4.0f);
			m_renderer->SetStereoExaggeration(stereoExaggeration);
		}
	
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation 事件处理程序。

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	m_renderer->SetDpi(sender->LogicalDpi);
}



 