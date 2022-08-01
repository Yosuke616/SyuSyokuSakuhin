/**
* @file		sceneGame.h
* @brief	実はゲームシーンである
* @author	志水陽祐
* @date		2022/6/30
*/

//インクルードガード
#ifndef _SCENEGAME_H_
#define _SCENEGAME_H_

//インクルード
#include "Scene.h"
#include "mainApp.h"
#include "AssimpModel.h"
#include <vector>
#include <unordered_map>

/**前方宣言**/

//前方宣言
class StageManager;

/*列挙体宣言***/
/** @brief どのステージを読み込むかを決める*/
enum Stage {
	STAGE_1 = 1,

	MAX_STAGE
};

//クラス定義
/**
* @brief		ゲームシーンの為のクラス
* @detail		どのステージを読み込む、ゲームをするうえで必要な情報のみを読み込む
*/
class SceneGame :public Scene {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static SceneGame* m_pInstance;
	/** @brief シーンのリスト(ここにどんどんステージを追加していく)*/
	std::vector<Scene*> m_GameSceneList;
	/** @brief 現在どのシーンか*/
	Scene* m_CurrentScene;
	/** @brief どのステージか*/
	Stage m_eStageState;

	/** @brief 初期化したいシーン*/
	Scene* m_pInitScene;

	/** @brief ステージマネージャーを作る予定*/
	StageManager* m_pStageManager;

	//メンバ関数
	/** @brief コンストラクタ*/
	SceneGame();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	/** @brief 編集モード*/
	bool m_bEditMode;
	/** @brief メニュー画面*/
	bool m_bPauseMode;


	//メンバ関数
	/** @brief インスタンス取得*/
	static SceneGame* GetInstance();
	/** @brief インスタンス破棄*/
	static void Destroy();

	/** @brief デストラクタ*/
	~SceneGame() override;
	/** @brief 初期化*/
	void Init() override;
	/** @brief 終了処理*/
	void Uninit() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;

	/** @brief シーンの切替*/
	void SetStage(Stage NextStage);
	/** @brief シーン情報の取得*/
	Stage GetStage();

};

#endif