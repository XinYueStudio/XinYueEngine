#pragma once
#	if SUPPORT_D3D11

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <assert.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <wrl.h>
#include <shellapi.h>
// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;



// Should only be used on the main thread.
class IXinYueGraphicsD3D11
{

public:
	virtual	ID3D11Device*  GetDevice() = 0;

};


#endif