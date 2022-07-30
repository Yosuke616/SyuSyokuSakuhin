/**
* @file		ResourceCSV.h
* @brief	CSVの中身を実際にのぞいたりするやつ
* @author	志水陽祐
* @date		2022/7/29
*/

/**インクルードガード**/
#ifndef _RESOURCECSV_H_
#define _RESOURCECSV_H_

/**インクルード部**/
#include <string>
#include <vector>
#include "ResourceBase.h"

/**クラス定義**/
/**
* @brief	CSV読み込んじゃうよーん
* @detail	実際にファイルの中まで入り読み込んだり書き込んだりする
*/
class ResourceCSV : public ResourceBase {
private:
	//メンバ変数
	/** @brief 二次元配列を読み込む為*/
	/** @brief セル*/
	using Cell = std::string;
	/** @brief 行*/
	using Column = std::vector<Cell>;
	/** @brief 列*/
	using Row = std::vector<Column>;

	/** @brief 読み込んだCSVデータの格納先*/
	Row m_Grid;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	ResourceCSV();
	/** @brief デストラクタ*/
	~ResourceCSV();

	/** @brief ロード*/
	bool Load(std::string fileName)override;
	/** @brief セーブ*/
	bool Save(std::string fileName)override;

	/**ゲッター**/
	/** @brief セルの場所に入っている文字の取得*/
	std::string GetStr(int x,int y);
	/** @brief セルに記入されている文字を数値で取得*/
	int GetInt(int x,int y);
	/** @brief セルに記入されている文字を数値(float型)で取得*/
	float GetFloat(int x,int y);
	/** @brief 行の数を取得*/
	int GetRowSize();
	/** @brief 列の数を取得*/
	int GetColumnSize(int row);

	/**セッター**/
	/** @brief セルに文字を入力する*/
	void SetStr(int x,int y,std::string str);
	/** @brief セルに数値を入力する*/
	void SetInt(int x,int y,int num);
	/** @brief セルに数値(小数点込み)を入力する*/
	void SetFloat(int x,int y,float num);
	/** @brief 行の数を指定*/
	void SetRowSize(int Size);
	/** @brief 列の数を指定*/
	void SetColumnSize(int row,int size);

};

#endif