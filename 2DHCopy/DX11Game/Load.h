/**
* @file		Load.h
* @brief	ロードしちゃうﾆｮｰﾝ
* @author	志水陽祐
* @date		2022/8/22
*/

/**インクルードガード**/
#ifndef _LOAD_H_
#define _LOAD_H_

/**インクルード部**/
#include "Scene.h"
#include <vector>

/**クラス定義**/
/**
* @brief  ロード画面用クラス
* @detail 画面転換で使うかも
*/
class Load {
private:
	//メンバ変数
	/** @brief スレッドハンドル*/
	static HANDLE m_handle;
	/** @brief ロード中かどうか*/
	static bool m_bLoading;
	/** @brief ロード時間のカウンタ*/
	static int  m_nLoadCount;
	/** @brief ロード画面のオブジェクトリスト*/
	static std::vector<Object*> m_LoadObject;

	//メンバ関数
	/** @brief ロード画面オブジェクトの初期化*/
	static void Init();
	/** @brief ロード画面のオブジェクトの終了処理*/
	static void Uninit();
	/** @brief ロード画面の更新と描画*/
	static unsigned __stdcall LoadScreen();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	Load();
	/** @brief デストラクタ*/
	~Load();

	/** @brief ロード画面のスレッド*/
	static void Begin();
	/** @brief ロード画面は終わる*/
	static void End();
	/** @brief ロード画面のフレーム数取得*/
	static int GetLoadCount();
};

#endif