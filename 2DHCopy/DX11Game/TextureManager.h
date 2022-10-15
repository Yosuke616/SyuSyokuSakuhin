/**
* @file		TextureManager.h
* @brief	テクスチャの情報を管理するマネージャー
* @author	志水陽祐
* @date		2022/6/28
*/

//インクルードガード
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//インクルード部
#include <unordered_map>
#include "mainApp.h"

//定数定義
/** @brief 必要になってくるテクスチャをここに書いていく*/
//DXちゃん
#define DXCHAN_STAND_TEX_NUM				(500)
#define PATH_TEX_DXCHAN_STAND				("data/texture/Stand_Def.PNG")
#define DXCHAN_RUN_TEX_NUM					(501)
#define PATH_TEX_DXCHAN_RUN					("data/texture/Run_Def.PNG")
#define DXCHAN_JUMP_TEX_NUM					(502)
#define PATH_TEX_DXCHAN_JUMP				("data/texture/Jump_Def.png")

#define DEBUG_BLOCK_NUM						(1)
#define PATH_TEX_DEBUG_BLOCK				("data/texture/Debug.jpg")

//タイトル画面用
#define TITLE_BACK_GROUND_NUM				(2)
#define PATH_TEX_TITLE_BACK_GROUND			("data/texture/Bellboad.jpg")

//ボタン
#define TITLE_BUTTON_NUM					(3)
#define PATH_TEX_TITLE_BUTTON				("data/texture/press_enter.png")

//フェード用テクスチャ
#define FEAD_OUT_NUM						(4)
#define PATH_TEX_FAID_OUT					("data/texture/BlackOut.png")

//UI関係--------------------------------------------------
//スコア									
#define SCORE_TEX_NUM						(5)
#define PATH_TEX_SCORE						("data/texture/Score.png")

//ハイスコア
#define MAX_SCORE_TEX_NUM					(6)
#define PATH_TEX_MAX_SCORE					("data/texture/highscore.png")

//数字
#define NUMBER_TEX_NUM						(7)
#define PATH_TEX_NUMBER						("data/texture/Number.png")

//ポーズボタン
#define PAUSE_TEX_NUM						(8)
#define PATH_TEX_PAUSE						("data/texture/Pause.png")

//時間
#define TIME_TEX_NUM						(9)
#define PATH_TEX_TIME						("data/texture/Time.png")

//プッシュA
#define PUSH_TEX_NUM						(10)
#define PATH_TEX_PUSH						("data/texture/pushA.png")

//タイトルへ
#define TO_TITLE_TEX_NUM					(11)
#define PATH_TEX_TO_TITLE					("data/texture/Title.png")

//再開
#define RESUME_TEX_NUM						(12)
#define PATH_TEX_RESUME						("data/texture/Sakai.png")

//オプション
#define OPTION_TEX_NUM						(13)
#define PATH_TEX_OPTION						("data/texture/Option.png")

//初めから
#define BEGIN_TEX_NUM						(14)
#define PATH_TEX_BEGIN						("data/texture/Begin.png")

//続きから
#define CONTINUE_TEX_NUM					(15)
#define PATH_TEX_CONTINUE					("data/texture/Continue.png")

//ステージセレクト
#define STAGE_SELECT_TEX_NUM				(16)
#define PATH_TEX_STAGE_SELECT				("data/texture/Select.png")

//終わる
#define END_TEX_NUM							(17)
#define PATH_TEX_END						("data/texture/End.png")

//BGM
#define BGM_TEX_NUM							(18)
#define PATH_TEX_BGM						("data/texture/BYM.png")

//SE
#define SE_TEX_NUM							(19)
#define PATH_TEX_SE							("data/texture/se.png")

//明るさ
#define BRIGHTNESS_TEX_NUM					(20)
#define PATH_TEX_BRIGHTNESS					("data/texture/Brightness.png")

//モザイク
#define MOSAIC_TEX_NUM						(21)
#define PATH_TEX_MOSAIC						("data/texture/MosaicRoll.jpg")

//円
#define CIRCLE_TEX_NUM						(22)
#define PATH_TEX_CIRCLE						("data/texture/circle.png")

//矢印
#define ARROW_NUM							(999)
#define PATH_ARROW							("data/texture/Billboard_Arrow.png")

//ステージとか
#define STAGE_1_RE_BG_TEX_NUM				(100)
#define PATH_TEX_STAGE_1_RE_BG				("data/texture/RedFall.jpg")


//クラス定義
/**
* @class	TextureManager
* @brief	テクスチャを管理するクラス
*/
class TextureManager {
private:
	//メンバ変数
	/** @brief インスタンス用変数*/
	static TextureManager* m_pInstance;
	/** @brief テクスチャリスト*/
	std::unordered_map<int, ID3D11ShaderResourceView*> m_TextureList;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	TextureManager();

public:
	//メンバ変数

	//メンバ関数
	/** @brief デストラクタ*/
	~TextureManager();
	/** @brief インスタンス化*/
	static TextureManager* GetInstance();
	/** @brief 終了処理*/
	static void Uninit();
	/** @brief テクスチャの追加*/
	bool AddTexture(const char* pszFileName,int nTexID);
	/** @brief テクスチャ取得*/
	ID3D11ShaderResourceView* GetTexture(int nTexID);
	/** @brief テクスチャ削除*/
	bool EraseTexture(int nTexID);
	/** @brief 読み込んだテクスチャの数*/
	int GetSize();
};

#endif