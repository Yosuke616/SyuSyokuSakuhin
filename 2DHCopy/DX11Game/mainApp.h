//=============================================================================
//
// メイン処理 [main.h]
// Author : SHIMIZU YOUSUKE
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "resource.h"

#include <math.h>
// 本来はヘッダに書かない方が良い
using namespace DirectX;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

#define SinDeg(degree)	sinf(XMConvertToRadians(degree))	//	Sinの値  引数は角度
#define CosDeg(degree)	cosf(XMConvertToRadians(degree))	//	Cosの値	引数は角度

// 2022/01/21 方向
enum Direct
{
	DIR_NONE = 0,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_UP,
	DIR_DOWN,
};


enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// カリングしない
	CULLMODE_CW,							// 前面カリング
	CULLMODE_CCW,							// 背面カリング

	MAX_CULLMODE
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

// 頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT3 nor;		// 法線ベクトル
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HWND GetMainWnd();							//	メイン ウィンドウ ハンドル取得
HINSTANCE GetInstance();					//	インスタンス ハンドル取得
ID3D11Device* GetDevice();					//	デバイス取得
ID3D11DeviceContext* GetDeviceContext();	//	デバイス コンテキスト取得
void SetBlendState(int nBlendState);		//	ブレンドステートの設定
void SetCullMode(int nCullMode);			//	カリングモードの設定
void SetZBuffer(bool bEnable);				//	深度バッファ有効無効制御
void SetZWrite(bool bEnable);				//	深度バッファ更新有効無効制御


#define 成田

// スクリーン座標をワールド座標に変換
XMVECTOR* CalcScreenToWorld(
	XMVECTOR* pout,		//	格納先
	float Sx,			//	スクリーンX座標
	float Sy,			//	スクリーンY座標
	float fZ,			//	射影空間でのZ値（0～1）
	int Screen_w,		//	スクリーンの幅
	int Screen_h,		//	スクリーンの高さ
	XMMATRIX View,		//	ビューマトリックス
	XMMATRIX Prj		//	射影行列
);
// XY平面とスクリーン座標の交点算出関数
XMVECTOR* CalcScreenToXY(
	XMFLOAT3* pout,		//	格納先
	float Sx,			//	スクリーンX座標
	float Sy,			//	スクリーンY座標
	int Screen_w,		//	スクリーンの幅
	int Screen_h,		//	スクリーンの高さ
	XMMATRIX View,		//	ビューマトリックス
	XMMATRIX Prj		//	射影行列
);


XMFLOAT2 CalcWorldSize();

// レンダラターゲットビューの取得
ID3D11RenderTargetView* GetRenderTargetView();
// Zバッファ
ID3D11DepthStencilView*	GetDepthStencilView();
// スワップチェーン
IDXGISwapChain*	GetSwapChain();
