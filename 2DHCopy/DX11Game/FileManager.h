/**
* @file		FileManager.h
* @brief	CSVとか読み込んだり書き込んだりするやつ
* @author	志水陽祐
* @date		2022/7/29
*/

/**インクルードガード**/
#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

/**インクルード部**/
#include <map>
#include "ResourceBase.h"

/**定数定義**/
//ステージ情報
#define STAGE_1_CSV	("Asserts/Stage_1.csv")
/**クラス定義**/
/**
* @brief	アセットに存在するファイルを管理する		
* @detail	ステージごとに読込先を決めたりする
*/
class FileManager {
private:
	//メンバ変数
	/** @brief ファイルの名前と読み込むものを紐づける*/
	using ResourceMap = std::map<std::string,ResourceBase*>;
	/** @brief ファイルを読み込むためのマップを調べるイテレーター*/
	using ResourceMapIt = ResourceMap::iterator;
	/** @brief ファイル*/
	using ResourceMapPair = std::pair<std::string,ResourceBase*>;

	/** @brief リソースの一括管理*/
	ResourceMap m_Resources;
	/** @brief インスタンス*/
	static FileManager* m_pInstance;

	//メンバ関数
	/** @brief コンストラクタ*/
	FileManager();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンスの取得*/
	static FileManager* GetInstance();
	/** @brief インスタンスの破棄*/
	static void Destroy();
	/** @brief デストラクタ*/
	~FileManager();
	/** @brief ロード*/
	bool Load(std::string fileName);

	/** @brief データ取得*/
	ResourceBase* Get(std::string fileName);
};

#endif