#ifndef _OBJECT_INFO_H_
#define _OBJECT_INFO_H_

//列挙隊宣言

//オブジェクトの更新順番
enum UPDATE_ORDER {
	UPDATE_BG,			//背景
	UPDATE_PLAYER,		//プレイヤー
	UPDATE_FIELD,		//地形
	UPDATE_EFFECT,		//エフェクト
	UPDATE_MODEL,		//モデル
	UPDATE_DEBUG,		//debug
	UPDATE_UI,			//UI
	UPDATE_OPTION,		//オプション
	UPDATE_GOLETTER,	//

	MAX_UPDATE_ORDER
};

//オブジェクトの描画の順番
enum DRAW_ORDER {
	DRAW_BG,			//背景
	DRAW_FIELD,			//フィールド
	DRAW_MODEL,			//モデル
	DRAW_PLAYER,		//プレイヤー
	DRAW_EFFECT,		//エフェクト

	DRAW_DEBUG,			//debug
	DRAW_UI_FRAME,		//UI枠
	DRAW_GOLETTER,
	DRAW_UI,			//UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//オプションオブジェクト
	DRAW_OPTION,		//オプション

	MAX_DRAW_ORDER
};

//@@@@@ ディエックスちゃん ----------------------------------------
#define PLAYER_NAME					("DXchan")
#define BLOCK_KARI					("Block")

#endif