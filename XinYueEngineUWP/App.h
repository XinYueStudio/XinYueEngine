#pragma once

#include "pch.h"
 
#include "../Content/Sample3DSceneRenderer.h"
namespace XinYueEngineUWP
{
	// 我们的应用程序的主入口点。使用 Windows shell 连接应用程序并处理应用程序生命周期事件。
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		App();

	public:
		// IFrameworkView Methods
		virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
		virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
		virtual void Load(_In_ Platform::String^ entryPoint);
		virtual void Run();
		virtual void Uninitialize();

	private:
		// Event Handlers
		void OnWindowSizeChanged(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
		);


		void OnVisibilityChanged(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::VisibilityChangedEventArgs^ args
		);

		void OnKeyDown(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
		);

		void OnDpiChanged(_In_ Windows::Graphics::Display::DisplayInformation^ sender, _In_ Platform::Object^ args);

		void OnActivated(
			_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView,
			_In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args
		);

		void OnSuspending(
			_In_ Platform::Object^ sender,
			_In_ Windows::ApplicationModel::SuspendingEventArgs^ args
		);

		void OnResuming(
			_In_ Platform::Object^ sender,
			_In_ Platform::Object^ args
		);

		void OnWindowClosed(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::CoreWindowEventArgs^ args
		);

		Sample3DSceneRenderer^ m_renderer;
		bool m_windowClosed;
		bool m_windowVisible;
	};
}
ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
