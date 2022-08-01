/**インクルード部**/
#include "FileManager.h"
#include "ResourceCSV.h"

/**静的メンバ変数**/
FileManager* FileManager::m_pInstance = nullptr;

/**
* @fn		FileManager::GetInstance
* @brief	インスタンスを取得する
*/
FileManager* FileManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new FileManager();
	}
	return m_pInstance;
}

/** 
* @fn		FileManager::Destroy
* @brief	インスタンスの破棄
*/
void FileManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
	}
}

/**
* @fn		FileManager::FileManager
* @brief	コンストラクタはもういない
*/
FileManager::FileManager() {

}

/**  
* @fn		FileManager::~FileManager
* @brief	デストラクタ、ついやっちゃうんだ
*/
FileManager::~FileManager() {
	//読み込まれたデータを解放
	ResourceMapIt it = m_Resources.begin();
	while (it != m_Resources.end())
	{	
		delete it->second;
		it++;
	}
}

/**
* @fn		FileManager::Load
* @brief	ロードします
*/
bool FileManager::Load(std::string fileName) {
	//読み込み済みのデータであれば処理しない
	if (m_Resources.find(fileName) != m_Resources.end()) {
		//キーが見つからなければ読み込まれている
		return true;
	}

	//キーがないので読込処理を開始
	//拡張子ごとにデータクラスメモリを確保

	ResourceBase* pResource;
	if (fileName.find(".csv") != std::string::npos) {
		pResource = new ResourceCSV();
	}
	else {
		return false;
	}

	//データの読込
	bool result = false;
	result = pResource->Load(fileName.c_str());
	if (!result) {
		//読み込めなかった場合
		delete pResource;
		return false;
	}

	//マップに登録
	m_Resources.insert(ResourceMapPair(fileName,pResource));

	return true;
}

/**
* @fn		FileManager::Get
* @brief	データを取得する
* @param	(string)	どこのデータを取得するか
* @return	(ResourceBase*)	取得したデータを返す
*/
ResourceBase* FileManager::Get(std::string fileName) {
	//データが存在するかどうか
	if (m_Resources.find(fileName) == m_Resources.end()) {
		//無かったらnullptr
		return nullptr;
	}

	//存在するデータを返す
	return m_Resources.at(fileName);
}
