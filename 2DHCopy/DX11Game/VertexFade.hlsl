/**
 * @file	VertexFade.hlsl
 * @brief	フェードイン/アウト用頂点シェーダ
 * @author	志水翔伍
 * @date	2021/12/23
 */

// グローバル
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
    float2 wipeSize;
};

// パラメータ
struct VS_INPUT {
	float3	Position	: POSITION;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 P = mul(float4(input.Position, 1.0f), g_mWorld);
	P = mul(P, g_mView);
	output.Position = mul(P, g_mProjection);
	output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	output.Diffuse = input.Diffuse;
	return output;
}
