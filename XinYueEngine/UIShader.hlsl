// 通过像素着色器传递的每个像素的颜色数据。
cbuffer ConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
 
};

Texture2D MainTexture : register(t0);
SamplerState SimpleSampler : register(s0);

 

struct appdata
{
	float4 vertex : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct v2f
{
	float4 vertex : SV_POSITION;
	float2 uv : TEXCOORD0;
};

v2f vert(appdata v)
{
	v2f o;
	float4 temp = v.vertex;
	temp.w = 1.0f;
	temp = mul(temp, model);
	temp = mul(temp, view);
	temp = mul(temp, projection);
	o.vertex = temp;
	o.uv = v.texcoord;
	return o;
}


float4 frag(v2f i) : SV_TARGET
{
	return MainTexture.Sample(SimpleSampler, i.uv);
}
