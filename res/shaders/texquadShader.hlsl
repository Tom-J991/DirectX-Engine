cbuffer CONSTANTS : register(b0)
{
	float4x4 modelViewProj;
};

struct INPUT
{
	float2 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct OUTPUT
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D myTex			: register(t0);
SamplerState mySampler	: register(s0);

OUTPUT vs_main(INPUT input)
{
	OUTPUT output;
	output.Position = mul(float4(input.Position, 0.0f, 1.0f), modelViewProj);
	output.UV = input.UV;
	return output;
}

float4 ps_main(OUTPUT input) : SV_TARGET
{
	return myTex.Sample(mySampler, input.UV);
}
