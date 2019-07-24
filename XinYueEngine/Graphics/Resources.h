#pragma once
#include <stddef.h>

#if defined(_WINDOWS)
// Windows Í·ÎÄ¼þ
#include <windows.h>

#endif
#include <string.h>
using namespace std;
#include <vector>
#include <assert.h>

#include <Vector3.h>
#include <Vector2.h>
#include <Vector4.h>

#pragma comment(lib, "D3DCompiler.lib")

#include <d3dcompiler.h>

#include <directxmath.h>
#include <directxcolors.h>

using namespace DirectX;
#include <d3dcommon.h>
 

#include <wrl.h>
using namespace Microsoft::WRL;


#include "Stereo3DMatrixHelper.h"


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




 