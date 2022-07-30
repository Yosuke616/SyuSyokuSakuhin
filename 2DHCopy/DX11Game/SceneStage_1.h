/**
* @file		SceneStage_1.h
* @brief	ステージ1の作成
* @author	志水陽祐
* @date		2022/7/30
*/

/**インクルードガード**/
#ifndef _STAGE_1_H_
#define _STAGE_1_H_

/**インクルード部**/
#include "Scene.h"
#include "mainApp.h"

/**クラス定義**/
class SceneStage_1 :public Scene {
private:
	//メンバ変数

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	/** @brief コンストラクタ*/
	SceneStage_1();
	/** @brief デストラクタ*/
	~SceneStage_1()override;
	/** @brief 初期化*/
	void Init()override;
	/** @brief 終了処理*/
	void Uninit()override;
	/** @brief 更新処理*/
	void Update()override;
	/** @brief 描画*/
	void Draw() override;

	//メンバ関数
};

#endif