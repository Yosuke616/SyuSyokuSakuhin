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
#define DXCHAN_SIZE_X				(100.0f)
#define DXCHAN_SIZE_Y				(100.0f)
#define DXCHAN_COLL_SIZE_X			(25.0f)
#define DXCHAN_COLL_SIZE_Y			(80.0f)
#define DXCHAN_COLL_SIZE_Z			(100.0f)

//@@@@@ ブロックの基本情報 ----------------------------------------
#define BLOCK_NAME					("Block")
#define MAPCHIP_WIDTH				(40.0f)	
#define MAPCHIP_HEIGHT				(40.0f)	
#define BLOCK_SIZE_X				(10.0f)
#define BLOCK_SIZE_Y				(10.0f)
#define BLOCK_SIZE_Z				(10.0f)
#define BLOCK_COLL_SIZE_X			(38.5f)
#define BLOCK_COLL_SIZE_Y			(38.5f)
#define BLOCK_COLL_SIZE_Z			(45.0f)

//@@@@@ 敵の基本情報 ----------------------------------------------
//敵ごとに設定する値は変わってくる
#define ENEMY_NAME					("Enemy")
//歩く敵　所謂クリボー
#define E_WALK_SIZE_X				(10.0f)
#define E_WALK_SIZE_Y				(10.0f)
#define E_WALK_SIZE_Z				(10.0f)
#define E_WALK_COLL_X				(43.5f)
#define E_WALK_COLL_Y				(43.5f)
#define E_WALK_COLL_Z				(50.0f)

#endif