/**
* @file		ResourceBase.h
* @brief	外部から読み込むデータを管理
* @author	志水陽祐
* @date		2022/7/29
*/

/**インクルードガード**/
#ifndef _RESOURCEBASE_H_
#define _RESOURCEBASE_H_

/**インクルード部**/
#include <string>

/**クラス定義**/
/**
* @brief	外部データを管理する
* @detail	つまり外部データを管理すること
*/
class ResourceBase {
private:
	//メンバ変数

	//メンバ関数

protected:
	//メンバ変数
	/** @brief ファイルの名前*/
	std::string m_fileName;

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	ResourceBase();
	/** @brief デストラクタ*/
	~ResourceBase();

	/** @brief ロード*/
	virtual bool Load(std::string fileName) = 0;
	/** @brief セーブ*/
	virtual bool Save(std::string fileName) = 0;
};


#endif