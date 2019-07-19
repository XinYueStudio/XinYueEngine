#pragma once

#include "DirectXBase.h"
#include "SampleOverlay.h"
#include "Stereo3DMatrixHelper.h"
// The constant buffer that is used with the DirectXMath library to draw the cube.
struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};


namespace XinYueEngineUWP
{
	// 此示例呈现器实例化一个基本渲染管道。
	ref	class Sample3DSceneRenderer : public DirectXBase
	{
	internal:
		Sample3DSceneRenderer();
		virtual void CreateDeviceIndependentResources() override;
		virtual void CreateDeviceResources() override;
		virtual void CreateWindowSizeDependentResources() override;
		virtual void Render() override;

		float GetStereoExaggeration();
		void RenderEye(_In_ unsigned int eyeIndex);
		void SetStereoExaggeration(_In_ float currentExaggeration);
		void Update(
			_In_ unsigned int eyeIndex,
			_In_ float timeTotal,
			_In_ float timeDelta
		);


	private:
		
		Microsoft::WRL::ComPtr<ID3D11InputLayout>           m_inputLayout;                // cube vertex input layout
		Microsoft::WRL::ComPtr<ID3D11Buffer>                m_vertexBuffer;               // cube vertex buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer>                m_indexBuffer;                // cube index buffer
		Microsoft::WRL::ComPtr<ID3D11VertexShader>          m_vertexShader;               // cube vertex shader
		Microsoft::WRL::ComPtr<ID3D11PixelShader>           m_pixelShader;                // cube pixel shader
 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewLeft;  // left texture view
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_textureShaderResourceViewRight;  // right texture view
		Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_sampler;                    // cube texture sampler
		Microsoft::WRL::ComPtr<ID3D11Buffer>                m_constantBuffer;             // constant buffer resource
	
		unsigned int             m_indexCount;                  // cube index count
		ConstantBuffer           m_constantBufferData;          // constant buffer resource data
		float                    m_projAspect;                  // aspect ratio for projection matrix
		float                    m_nearZ;                       // nearest Z-distance at which to draw vertices
		float                    m_farZ;                        // farthest Z-distance at which to draw vertices
		float                    m_widthInInches;               // estimated screen width in inches
		float                    m_heightInInches;              // estimated screen height in inches
		float                    m_stereoExaggerationFactor;    // stereo effect that is user adjustable
		Platform::String^        m_hintMessage;                 // hint message about customer manipulation
		float                    m_worldScale;                  // developer specified world unit
	};
}

