#pragma once
#include <stddef.h>

#if defined(_WINDOWS)
// Windows Í·ÎÄ¼þ
#include <windows.h>

#endif

//Include and link appropriate libraries and headers//
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "D3dx9.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
 
#include <d3d11.h>

#include <dxgi1_2.h>
#include <d3d11_2.h>
using namespace DirectX;
#include <d3dcommon.h>
 
#include <string.h>
using namespace std;
#include <vector>
#include <assert.h>
 
#include <wrl.h>
using namespace Microsoft::WRL;

#include <Vector3.h>
#include <Vector2.h>
#include <Vector4.h>

// Defines the vertex format for the shapes generated in the functions below.
struct BasicVertex
{
	Vector3 pos;  // position
	Vector2 tex;  // texture coordinate
};

struct ConstantBuffer
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 Point0;
	XMFLOAT4 Point1;
	XMFLOAT4 Point2;
	XMFLOAT4 Point3;
	XMFLOAT4X4 worldToLocalMatrix;
	int id;
	int eyeindex;
	int HoloRender;
	int Stereo;

};
struct	Size
{
	UINT Width;
	UINT Height;
};

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}

// Check for SDK Layer support.
inline bool SdkLayersAvailable()
{
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		0,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);

	return SUCCEEDED(hr);
}




 