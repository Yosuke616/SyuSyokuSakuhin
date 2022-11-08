/**
* @file		fade.h
* @brief	フェードイン/アウト
* @author	志水陽祐
* @date		2022/10/22
*/

/**インクルードガード**/
#ifndef _FADE_H_
#define _FADE_H_

/**インクルード**/
#include "SceneManager.h"
#include "StageManager.h"

/**列挙体宣言**/
enum EFade {
	FADE_NONE = 0,			//何もしていない
	FADE_IN,				//フェードイン処理中
	FADE_OUT,				//フェードアウト処理中
	FADE_OUT_SCENE_CHANGE,	//フェードアウト処理中(シーン遷移)
	MAX_FADE
};

/**プロトタイプ宣言**/
/** @brief 初期化*/
void InitFade();				
/** @brief 終了処理*/
void UninitFade();				
/** @brief 更新処理*/
void UpdateFade();					
/** @brief 描画処理*/
void DrawFade();				
/** @brief シーン遷移を含めたフェードアウト*/
void StartFadeOutSceneChange(EScene eNext, int deley = 0);
/** @breif フェードだけさせたい*/
void StartFadeOut(int deley = 0);

/** @brief 状態取得*/
EFade GetFade();					
/** @brief フェードアウト色設定*/
void SetFadeColor(float fR, float fG, float fB); 
#endif