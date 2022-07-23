/**
 * @file	PixcelFade.hlsl
 * @brief	フェードイン/アウト用ピクセルシェーダ
 * @author	志水翔伍
 * @date	2021/12/23  ワイプ作成
 *          2021/12/26  円形ワイプ追加
 */

// グローバル
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
    float2 wipeSize;
};

//cbuffer WipeSize : register(b1)
//{
//    float2 wipeSize;
//};

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

Texture2D    g_texture : register(t0);	// テクスチャ
SamplerState g_sampler : register(s0);	// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = input.Diffuse;
    //Color.rgb += 0.3f;
	if (g_mTexture._44 > 0.0f) {
		Color *= g_texture.Sample(g_sampler, input.TexCoord);
	}
#if 0   // ワイプ
    
    clip(input.Position.x - wipeSize.x);
    
    return Color;
    
#endif
    
#if 1   // 円形ワイプ
    
    float2 posFromCenter = input.Position.xy - float2(640.0f, 360.0f);
    clip(length(posFromCenter) - wipeSize.x);
    
    return Color;
    
#endif

    //if (wipeSize.x < 0.0f)
    //{
    //    clip(-input.Position.x - wipeSize.x);
    //}
    //if (wipeSize.x >= 0.0f)
    //{
    //    clip(wipeSize.x - input.Position.x);
    //}
    
    //clip(input.Position.x - wipeSize.x);
    
    //clip(wipeSize.x - input.Position.x);
    //input.Position.x - wipeSize.x;
    
	return Color;
	//return float4(0,1,0,1);	// 全部のピクセルがこの色(緑)に変化する
}
