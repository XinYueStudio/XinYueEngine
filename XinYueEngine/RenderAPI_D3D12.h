#pragma once
#include "RenderAPI.h"
#include "PlatformBase.h"

// Direct3D 12 implementation of RenderAPI.


#if SUPPORT_D3D12


#include <assert.h>
#include <d3d12.h>
#include "IXinYueGraphicsD3D12.h"

class RenderAPI_D3D12 : public RenderAPI, public IXinYueGraphicsD3D12
{
public:

	virtual ~RenderAPI_D3D12() { }
 

	/**********************************************************************************/
	/***************************IXinYueGraphicsD3D12******************************/
	/**********************************************************************************/
	virtual	ID3D12Device*  GetDevice();

	virtual void* GetRenderDevice();

	virtual	ID3D12Fence*GetFrameFence();

	// Returns the value set on the frame fence once the current frame completes or the GPU is flushed
	virtual	UINT64  GetNextFrameFenceValue();


	// Executes a given command list on a worker thread.
	// [Optional] Declares expected and post-execution resource states.
	// Returns the fence value.
	virtual UINT64   ExecuteCommandList(ID3D12GraphicsCommandList* commandList, int stateCount, XinYueGraphicsD3D12ResourceState* states);
public:
	virtual	void Init(HWND hwnd, Size resolution, bool stereo);
	virtual	void LoadAssets();
	virtual	void Resize();
	virtual	void Update();
	virtual	void Render(int eyeindex);
	virtual	void Present();
	virtual	void Destroy();

private:
	IXinYueGraphicsD3D12* s_D3D12;

	// Pipeline objects.
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr < ID3D12Device>m_device;
	ComPtr < ID3D12Resource> m_renderTargets[m_FrameCount];
	ComPtr < ID3D12CommandAllocator> m_commandAllocator;
	ComPtr < ID3D12CommandQueue> m_commandQueue;
	ComPtr < ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr < ID3D12PipelineState> m_pipelineState;
	ComPtr < ID3D12GraphicsCommandList> m_commandList;
	UINT m_rtvDescriptorSize;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr < ID3D12Fence> m_fence;
	UINT64 m_fenceValue;


	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
	void LoadPipeline();
	void PopulateCommandList();
	void WaitForPreviousFrame();



 
};
#endif