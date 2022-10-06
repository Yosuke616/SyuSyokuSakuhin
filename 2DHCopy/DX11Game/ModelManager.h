/**
* @file		ModelManager.h
* @brief	モデルを管理するためのマネージャー
* @author	志水陽祐
* @date		2022/7/7
*/
//インクルードガード
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//インクルード部
#include <unordered_map>
#include "mainApp.h"
#include "AssimpModel.h"

//定数定義
//仮
#define WALK_ENEMY_MODEL_NUM	(0)
#define PATH_WALK_ENEMY			("data/model/WalkEnemy.fbx")
#define MINT_GREEN_BLOCK_NUM	(1)
#define PATH_MINT_GREEN_BLOCK	("data/model/Grass_Block.fbx")
#define RARD_BLOCK_NUM			(2)
#define PATH_RARD_BLOCK			("data/model/Rard_Block.fbx")
#define GRASS_BLOCK_RE_NUM		(3)
#define PATH_GRASS_RE_BLOCK		("data/model/Grass_Block_RE.fbx")
#define RARD_BLOCK_RE_NUM		(4)
#define PATH_RARD_RE_BLOCK		("data/model/Rard_Block_RE.fbx")
#define KOBAN_MODEL_NUM			(5)
#define PATH_KOBAN				("data/model/Koban.fbx")
#define MAGA_MODEL_NUM			(6)
#define PATH_MAGA				("data/model/Magatama.fbx")

//クラス定義
/**
 * @class	ModelManager
 * @brief	モデルマネージャー
*/
class ModelManager {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static ModelManager* m_pInstance;
	/** @brief モデルリスト*/
	std::unordered_map<int, CAssimpModel*>	m_ModelList;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	ModelManager();

public:
	//メンバ変数

	//メンバ関数
	/** @brief デストラクタ*/
	~ModelManager();
	/** @brief インスタンス化*/
	static ModelManager* GetInstance();
	/** @brief モデルマネージャーの解放*/
	static void Uninit();
	/** @brief モデルの追加*/
	bool AddModel(const char* pszFileName,int nModelID);
	/** @brief モデルの取得*/
	CAssimpModel* GetModel(int nModelID);
	/** @brief モデル削除*/
	bool EraseModel(int nModelID);

};

#endif