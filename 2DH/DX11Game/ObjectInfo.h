/*
	オブジェクトの情報をまとめるためのヘッダー
	志水陽祐
	2022/6/7
*/

//===== インクルードガード =====
#ifndef __OBJECT_INFO_H__
#define __OBJECT_INFO_H__

//===== 列挙体 ======

//	オブジェクトの更新の順番
enum UPDATE_ORDER
{
	UPDATE_BG,			//	背景
	UPDATE_PLAYER,		//	プレイヤー
	UPDATE_FIELD,		//	地形
	UPDATE_EFFECT,		//	エフェクト
	UPDATE_MODEL,		//	モデル
	UPDATE_DEBUG,		//	debug
	UPDATE_UI,			//	UI
	UPDATE_OPTION,		//	オプションオブジェクト
	UPDATE_GOLETTER,

	MAX_UPDATE_ORDER
};

//	オブジェクトの描画の順番
enum DRAW_ORDER
{
	DRAW_BG,		//	背景
	DRAW_FIELD,		//	地形
	DRAW_MODEL,		//	プレイヤー
	DRAW_PLAYER,	//	プレイヤー
	DRAW_EFFECT,	//	エフェクト

	DRAW_DEBUG,		//	debug
	DRAW_UI_FRAME,	//	UI枠
	DRAW_GOLETTER,
	DRAW_UI,		//	UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//	オプションオブジェクト
	DRAW_OPTION,	//	オプションオブジェクト

	MAX_DRAW_ORDER
};

	/***** アニメーション ******/

// プレイヤーのFBXの数
enum PLAYER_FBX
{
	//FBX_ONE = 0,	// 一つ目(待機、風に吹かれている、落ちている、ゲットしたと
	//FBX_TWO,		// 二つ目(ゴールした時
};

// 1つ目のfbx
enum ANIM_ID
{
	/*PLAYER_IDLE = 0,
	PLAYER_CARRIED,
	PLAYER_FALL,
	PLAYER_GET,
	PLAYER_BIND,
	PLAYER_GAMEOVER,

	MAX_ANIMATION*/
};
#endif