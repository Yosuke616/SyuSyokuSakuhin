/**
* @file		SceneStage_1_Re.h
* @brief	ステージ1(Reloaded)の作成
* @author	志水陽祐
* @date		2022/7/30
*/

/**インクルードガード**/
#ifndef _STAGE_1_RE_H_
#define _STAGE_1_RE_H_

/**インクルード部**/
#include "Scene.h"
#include "mainApp.h"

/**クラス定義**/
/**
* @brief	ステージ1
* @detail	裏表ラバーズ
*/
class SceneStage_1_Re : public Scene {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static SceneStage_1_Re* m_pInstance;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static SceneStage_1_Re* GetInstance();
	/** @brief インスタンスの破棄*/
	static void Destroy();
	/** @brief コンストラクタ*/
	SceneStage_1_Re();
	/** @brief デストラクタ*/
	~SceneStage_1_Re();
	/** @brief 初期化*/
	void Init() override;
	/** @brief 終了処理*/
	void Uninit() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;
};

#endif