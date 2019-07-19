// 通过像素着色器传递的每个像素的颜色数据。
Texture2D SimpleTexture : register(t0);
SamplerState SimpleSampler : register(s0);

struct sPSInput
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
};

float4 main(sPSInput input) : SV_TARGET
{
	float3 lightDirection = normalize(float3(1,-1,0));
	return SimpleTexture.Sample(SimpleSampler, input.tex) * (0.8f * saturate(dot(input.norm, -lightDirection)) + 0.2f);
}
