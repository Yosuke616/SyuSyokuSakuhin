//インクルード部
#include "ModelManager.h"
#include <crtdbg.h>

//静的メンバ変数
ModelManager* ModelManager::m_pInstance = nullptr;

/**
* @fn		ModelManager::ModelManager
* @brief	ｵﾃﾞ...ｺﾝｽﾄﾗｸﾀ...ｸｳ...
*/
ModelManager::ModelManager() {

}

/**
* @fn		ModelManager::~ModelManager
* @brief	デストラクタって何だろう
*/	
ModelManager::~ModelManager() {
	for (auto buff : m_ModelList) {
		buff.second->Release();
		delete buff.second;
	}
	m_ModelList.clear();
}

/**
* @fn		ModelManager::GetInstance
* @brief	インスタンス化
* @return	(ModelManager*)	モデルマネージャーのポインタを返す
*/
ModelManager* ModelManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new ModelManager;
	}
	return m_pInstance;
}

/**
* @fn		ModelManager::Uninit
* @brief	モデルマネージャー開放
*/
void ModelManager::Uninit() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		ModelManager::AddModel
* @brief	モデルの追加
* @param	(const char*)	ファイルの名前
* @param	(int)	何番目の番号に紐づいているか
* @return	(bool)	モデルを読み込めたかどうか
*/
bool ModelManager::AddModel(const char* pszFileName, int nModelID) {
	//モデルが既にあったら返す
	if (m_ModelList[nModelID] != nullptr) {
		return true;
	}

	//変数宣言
	CAssimpModel* model = new CAssimpModel();

	//モデルデータの読込
	if (!model->Load(GetDevice(),GetDeviceContext(),pszFileName)) {
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitModel", MB_OK);
		return false;
	}

	//マップに追加
	m_ModelList[nModelID] = model;

	return true;
}

/**
* @fn		ModelManager::GetModel
* @brief	モデルの取得
* @param	(int)	取得したいモデルの番号
* @return	(CAssimpModel*)	モデルの情報を返す
*/
CAssimpModel* ModelManager::GetModel(int nModelID) {
	//検索
	auto itr = m_ModelList.find(nModelID);

	//結果の判定
	if (itr == m_ModelList.end()) {
		//無かった時
		return nullptr;
	}
	else {
		return (*itr).second;
	}
	return nullptr;
}

/**
* @fn		ModelManager::EraseModel
* @brief	モデルの削除
* @param	(int)	消したいモデルの番号
* @return	(bool)	消せたかどうか
*/
bool ModelManager::EraseModel(int nModelID){
	//検索
	auto Model = this->GetModel(nModelID);

	m_ModelList.erase(nModelID);
	return true;

	return false;
}