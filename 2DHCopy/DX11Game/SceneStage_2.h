/**
* @file		SceneStage_2.h
* @brief	ステージ1の作成
* @author	志水陽祐
* @date		2022/11/25
*/

/**インクルードガード**/
#ifndef _STAGE_2_H_
#define _STAGE_2_H_

/**インクルード部**/
#include "Scene.h"
#include "mainApp.h"
#include "Object.h"

/**クラス定義**/
class SceneStage_2 :public Scene {
private:
	//メンバ変数
	/** @brief プレイヤーのポインタを保存する変数*/
	Object* m_pPlayer;
	/** @brief インスタンス*/
	static SceneStage_2* m_pInstance;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static SceneStage_2* GetInstance();
	/** @brief インスタンスの破棄*/
	static void Destroy();
	/** @brief コンストラクタ*/
	SceneStage_2();
	/** @brief デストラクタ*/
	~SceneStage_2()override;
	/** @brief 初期化*/
	void Init()override;
	/** @brief 終了処理*/
	void Uninit()override;
	/** @brief 更新処理*/
	void Update()override;
	/** @brief 描画*/
	void Draw() override;

};

#endif