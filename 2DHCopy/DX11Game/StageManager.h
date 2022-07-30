/**
* @file		StageManager.h
* @brief	ステージごとの情報を管理するためのクラス
* @author	志水陽祐
* @date		2022/7/27
*/

/**インクルードガード**/
#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_

/**インクルード部**/
#include "mainApp.h"
#include "Object.h"
#include "Component.h"

#include <map>
#include <vector>

/**クラス定義**/
/**
* @brief	ステージを管理するためのマネージャー
* @detail	地形情報やどんなオブジェクトがあるかの管理
*/
class StageManager {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static StageManager* m_pInstance;

	//vectorの二次元配列
	/** @brief セル*/
	using Cell = int;
	/** @brief 行*/
	using Column = std::vector<Cell>;
	/** @brief 列*/
	using Row = std::vector<Column>;
	/** @brief ステージの配列*/
	Row m_StageGrid;
	/** @brief ステージの要素数x*/
	int m_nBlock_X;
	/** @brief ステージの要素数y*/
	int m_nBlock_Y;
	/** @brief ステージの大きさ*/
	XMFLOAT2 m_vStageSize;
	/** @brief ワールド座標での立幅と横幅*/
	XMFLOAT2 m_WorldSize;
	/** @brief ステージ描画の為の起点*/
	XMFLOAT2 m_BasePos;

	/**デバッグ用**/
	/** @brief カーソルに表示するもの*/
	Object* m_pShowObject;
	/** @brief マウス座標の要素数x*/
	int m_Element_X;
	/** @brief マウス座標の要素数y*/
	int m_Element_Y;
	/** @brief 配置する物の種類*/
	int m_nMapSta;

	//メンバ関数
	/** @brief コンストラクタ*/
	StageManager();
	/** @brief ステージ状態の保存*/
	bool Save(int Stage);
	/** @brief ステージの読込*/
	bool Load(std::string file);
	
	/**デバック**/
	/** @brief ステージオブジェクト生成*/
	void UpdateShowObject(float fPosX, float fPosY);
	/** @brief ステージオブジェクト生成*/
	void CreateBlock_Deb(float fPosX, float fPosY, int nNumber, int x, int y, int nCreate_Status);

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static StageManager* GetInstance();
	/** @brief インスタンス破棄*/
	static void Destroy();
	/** @brief デストラクタ*/
	~StageManager();
	/** @brief 更新処理*/
	void Update();
	/** @brief 描画処理*/
	void Draw();

	/** @brief 地形生成*/
	void CreateStage(int Stage_State);

	/** @brief ステージオブジェクト生成(ゲームモード)*/
	Object* CreateBlock(float fPosX, float fPosY, int nState, int nBlockID);

	/** @brief ブロックの番号から情報を取得する*/
	Object* GetBlock(int nNum);
};

#endif