cbuffer CONSTANTS : register(b0)
{
	float4x4 modelViewProj;
};

struct INPUT
{
	float3 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct OUTPUT
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D diffuse1	: register(t0);
SamplerState samp1	: register(s0);

OUTPUT vs_main(INPUT input)
{
	OUTPUT output;
	output.Position = mul(float4(input.Position, 1.0f), modelViewProj);
	output.UV = input.UV;
	return output;
}

float4 ps_main(OUTPUT input) : SV_TARGET
{
	return diffuse1.Sample(samp1, input.UV);
}
