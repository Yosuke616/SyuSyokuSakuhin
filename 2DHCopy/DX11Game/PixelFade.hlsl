/**
 * @file	PixcelFade.hlsl
 * @brief	�t�F�[�h�C��/�A�E�g�p�s�N�Z���V�F�[�_
 * @author	�u���Č�
 * @date	2021/12/23  ���C�v�쐬
 *          2021/12/26  �~�`���C�v�ǉ�
 */

// �O���[�o��
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

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

Texture2D    g_texture : register(t0);	// �e�N�X�`��
SamplerState g_sampler : register(s0);	// �T���v��

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = input.Diffuse;
    //Color.rgb += 0.3f;
	if (g_mTexture._44 > 0.0f) {
		Color *= g_texture.Sample(g_sampler, input.TexCoord);
	}
#if 0   // ���C�v
    
    clip(input.Position.x - wipeSize.x);
    
    return Color;
    
#endif
    
#if 1   // �~�`���C�v
    
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
	//return float4(0,1,0,1);	// �S���̃s�N�Z�������̐F(��)�ɕω�����
}
