/**インクルード**/
#include "fade.h"
#include "SceneTitle.h"
#include "sceneGame.h"
#include "fadePolygon.h"
#include "polygon.h"
#include "Sound.h"

/**マクロ定義**/
#define FADE_OUT_SPEED		(15.0f)				// フェードアウトのスピード
#define FADE_IN_SPEED		(15.0f)				// フェードインのスピード
#define START_FADE_OUT_POS	(SCREEN_WIDTH)		// フェードアウト開始位置
#define START_FADE_IN_POS	(0.0f)				// フェードイン開始位置

/**グローバル変数**/
static float g_fRed = 0.0f;				// フェードアウト色
static float g_fGreen = 0.0f;
static float g_fBlue = 0.0f;
static float g_fAlpha = 1.0f;			// 不透明度
static EFade g_eFade = FADE_IN;			// 状態
static EScene g_eNext = SCENE_TITLE;	// 次のシーン
static int g_nStage;					//　次のステージ

static int g_nDeleyCnt = 0;

static float	g_fFadeIN;				// フェードイン
static float	g_fFadeOUT;				// フェードアウト

/**
* @fn		InitFade
* @brief	初期化
*/
void InitFade()
{
	//g_fAlpha = 1.0f;
	g_eFade = FADE_IN;
	g_eNext = SCENE_TITLE;

	g_fFadeOUT = START_FADE_OUT_POS;	// フェードアウト開始位置
	g_fFadeIN = START_FADE_IN_POS;		// フェードイン開始位置
}

/**
* @fn		UninitFade
* @brief	終了処理
*/
void UninitFade()
{
	//g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}

/**
* @fn		UpdateFade
* @brief	更新処理
*/
void UpdateFade()
{
	switch (g_eFade)
	{
	case FADE_NONE:	//	フェードなし
		break;

	case FADE_OUT:
		
		if (g_nDeleyCnt > 0) {
			g_nDeleyCnt--;
			break;
		}

		g_fFadeIN = 0.0f;
		g_fFadeOUT -= FADE_OUT_SPEED;
		UpdateFadePolygon(g_fFadeOUT);

		if (g_fFadeOUT <= 0.0f) {
			//フェードイン処理に切替
			g_fFadeOUT = SCREEN_WIDTH;
			g_eFade = FADE_IN;
		}

		break;

	case FADE_OUT_SCENE_CHANGE:	//	フェードアウト

		if (g_nDeleyCnt > 0)
		{
			g_nDeleyCnt--;
			break;
		}

		g_fFadeIN = 0.0f;
		g_fFadeOUT -= FADE_OUT_SPEED;
		UpdateFadePolygon(g_fFadeOUT);

		// ボリュームもフェードアウト
		CSound::SetVolume(g_fFadeOUT / SCREEN_WIDTH);		// 音量を徐々に小さく

		if (g_fFadeOUT <= 0.0f)
		{
			// フェードイン処理に切り替え
			g_fFadeOUT = SCREEN_WIDTH;
			g_eFade = FADE_IN;

			// シーン切替
			auto pSceneMsnager = SceneManager::Instance();
			pSceneMsnager->SetScene(g_eNext);
		}
		break;

	case FADE_IN:	// フェードイン
		g_fFadeOUT = SCREEN_WIDTH;
		g_fFadeIN += FADE_IN_SPEED;
		UpdateFadePolygon(g_fFadeIN);

		// ボリュームもフェードイン
		CSound::SetVolume(g_fFadeIN / SCREEN_WIDTH);	// 音量を徐々に大きく

		if (g_fFadeIN >= SCREEN_WIDTH)
		{
			// フェードイン終了
			g_fFadeIN = 0.0f;
			g_eFade = FADE_NONE;
		}
		break;
	}
}

/**
* @fn		DrawFade
* @brief	描画
*/
void DrawFade()
{
	//	半透明描画はZバッファを更新しない(Zチェックは行う)
	SetZWrite(false);
	SetBlendState(BS_ALPHABLEND);

	// 画面全体に半透明ポリゴンを描画
	SetFadePolygonPos(0.0f, 0.0f);
	SetFadePolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetFadePolygonTexture(nullptr);
	SetFadePolygonFrameSize(1.0f, 1.0f);
	SetFadePolygonUV(0.0f, 0.0f);
	SetFadePolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetFadePolygonAlpha(1.0f);
	DrawFadePolygon(GetDeviceContext());

	// 元に戻す
	SetZWrite(true);
	SetFadePolygonColor(1.0f, 1.0f, 1.0f);
	SetFadePolygonAlpha(1.0f);
}

/**
* @fn		StartFadeOutSceneChange
* @brief	シーンチェンジしながらフェードする
* @param	(EScene)	次のシーン
* @param	(int)		どの位遅らせるか
*/
void StartFadeOutSceneChange(EScene eNext, int deley)
{
	if (g_eFade != FADE_OUT_SCENE_CHANGE)
	{
		g_eFade = FADE_OUT_SCENE_CHANGE;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
		g_nDeleyCnt = deley;
	}
}

/**
* @fn		StartFadeOut
* @brief	フェードアウトさせる
* @param	(int)	どの位遅らせるか
*/
void StartFadeOut(int deley) {
	if (g_eFade != FADE_OUT) {
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_nDeleyCnt = deley;
	}
}

/**
* @fn		GetFade
* @brief	フェード状態の取得
* @return	(EFade)	フェードの状態を取得する
*/
EFade GetFade()
{
	return g_eFade;
}

/**
* @fn		SetFadeColor
* @brief	フェードアウトの色の設定
* @param	(float)	R
* @param	(float)	G
* @param	(float)	B
*/
void SetFadeColor(float fR, float fG, float fB)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}