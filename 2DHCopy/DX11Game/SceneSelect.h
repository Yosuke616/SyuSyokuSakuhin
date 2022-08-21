/**
* @file		SceneSelect.h
* @brief	ステージセレクト画面
* @author	志水陽祐
* @date		2022/8/16
*/

/**インクルードガード**/
#ifndef _SCENE_SELECT_H_
#define _SCENE_SELECT_H_

/**インクルード部**/
#include "Scene.h"
#include <vector>

/**クラス定義**/
/**
* @brief	ステージ選択用のクラス
* @detail	色々
*/
class StageSelect : public Scene {
private:
	//メンバ変数
	//ベクターの二次元配列
	/** @brief セル(中身)*/
	using Cell = int;					
	/** @brief 行*/
	using Column = std::vector<Cell>;	
	/** @brief 列*/
	using Row = std::vector<Column>;	
	/** @brief クリア状況の配列*/
	static Row m_StageGrid;

	//メンバ関数
	///** @brief ロード*/
	//static bool Load();
	///** @brief セーブ*/
	//static bool Save();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	StageSelect();
	/** @brief デストラクタ*/
	~StageSelect() override;
	/** @brief 初期化処理*/
	void Init() override;
	/** @brief 終了処理*/
	void Uninit() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;

	///** @brief 選択可能かどうかの取得*/
	//static bool GetSelectAble(int stage_num);
	///** @brief 選択可能かどうかの配列の取得*/
	//static Row GetSelectGrid();
	///** @brief CSVファイルの取得*/
	//static bool NewGame();
	///** @brief クリア状況の保存*/
	//static bool SaveClearInfo(int nStage);
};

#endif