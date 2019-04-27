#pragma once
#	if SUPPORT_D3D12

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <assert.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <string>
#include <wrl.h>
#include <shellapi.h>
// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;


struct XinYueGraphicsD3D12ResourceState
{
	ID3D12Resource*       resource; // Resource to barrier.
	D3D12_RESOURCE_STATES expected; // Expected resource state before this command list is executed.
	D3D12_RESOURCE_STATES current;  // State this resource will be in after this command list is executed.
};

// Should only be used on the main thread.
class IXinYueGraphicsD3D12
{

public:
	virtual	ID3D12Device*  GetDevice() = 0;
	 

	virtual	ID3D12Fence*GetFrameFence() = 0;
 
	// Returns the value set on the frame fence once the current frame completes or the GPU is flushed
	virtual	UINT64  GetNextFrameFenceValue() = 0;
 

	// Executes a given command list on a worker thread.
	// [Optional] Declares expected and post-execution resource states.
	// Returns the fence value.
	virtual UINT64   ExecuteCommandList(ID3D12GraphicsCommandList* commandList, int stateCount, XinYueGraphicsD3D12ResourceState* states) = 0;
 
	 



};
 

#endif