/**
* @file		MosaicRollComponent.h
* @brief	モザイクでロールします
* @author	志水陽祐
* @date		2022/9/28
*/

/**インクルードガード**/
#ifndef _MOSAICROLLCOMPONENT_H_
#define _MOSAICROLLCOMPONENT_H_

/**インクルード部**/
#include "Scene.h"
#include <vector>

/**クラス定義**/
/**
* @brief	モザイクでかくして変える
* @detail	画面全てのオブジェクトの見た目を変えるタイミングで呼ぶ
*/
class MosaicRoll {
private:
	//メンバ変数
	/** @brief スレッドハンドル*/
	static HANDLE m_handle;
	/** @brief ロード中かどうか*/
	static bool m_bLoading;
	/** @brief ロード時間のカウンタ*/
	static float m_fLoadCount;
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
	MosaicRoll();
	/** @brief デストラクタ*/
	~MosaicRoll();

	/** @brief ロード画面のスレッド*/
	static void Begin();
	/** @brief ロード画面は終わる*/
	static void End();
	/** @brief ロード画面のフレーム数取得*/
	static int GetLoadCount();
};

#endif