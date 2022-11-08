/**
* @file		fadePolygon.h
* @brief	フェードイン/アウト用ポリゴン処理
* @author	志水陽祐
* @date		2022/10/22
*/

/**インクルードガード**/
#ifndef _FADEPOLYGON_H_
#define _FADEPOLYGON_H_

/**インクルード部**/
#include "mainApp.h"

/**プロトタイプ宣言**/
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