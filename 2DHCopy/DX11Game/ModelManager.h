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
#define KARIKARIMODEL			(0)
#define PATH_MODEL_KARIKARI		("data/model/RosaRosa1.fbx")
#define BLOCK_KARIKARI			(1)
#define PATH_MODEL_BLOCKKARI	("data/model/BLOCK_KARI.fbx")

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
	static ModelManager* GettInstance();
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