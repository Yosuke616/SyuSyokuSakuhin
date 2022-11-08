/**
* @file		fadePolygon.h
* @brief	�t�F�[�h�C��/�A�E�g�p�|���S������
* @author	�u���z�S
* @date		2022/10/22
*/

/**�C���N���[�h�K�[�h**/
#ifndef _FADEPOLYGON_H_
#define _FADEPOLYGON_H_

/**�C���N���[�h��**/
#include "mainApp.h"

/**�v���g�^�C�v�錾**/
HRESULT InitFadePolygon(ID3D11Device* pDevice);
void UninitFadePolygon(void);
void UpdateFadePolygon(float);
void DrawFadePolygon(ID3D11DeviceContext* pDeviceContext);
void SetFadePolygonTexture(ID3D11ShaderResourceView* pTexture);
void SetFadePolygonPos(float fX, float fY);
void SetFadePolygonSize(float fScaleX, float fScaleY);
void SetFadePolygonAngle(float fAngle);
void SetFadePolygonUV(float fU, float fV);
void SetFadePolygonFrameSize(float fWidth, float fHeight);
void SetFadePolygonColor(float fRed, float fGreen, float fBlue);
void SetFadePolygonAlpha(float fAlpha);

#endif