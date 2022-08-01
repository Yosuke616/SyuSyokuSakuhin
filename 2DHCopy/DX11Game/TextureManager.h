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
#define DXCHAN_STAND_TEX_NUM				(0)
#define PATH_TEX_DXCHAN_STAND				("data/texture/Stand_Def.PNG")

#define DEBUG_BLOCK_NUM						(1)
#define PATH_TEX_DEBUG_BLOCK				("data/texture/Debug.jpg")

//タイトル画面用
#define TITLE_BACK_GROUND_NUM				(2)
#define PATH_TEX_TITLE_BACK_GROUND			("data/texture/TitleBack.jpg")

//ボタン
#define TITLE_BUTTON_NUM					(3)
#define PATH_TEX_TITLE_BUTTON				("data/texture/press_enter.png")

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