/**注意喚起のやつ(名称不明)**/
#define _CRT_SECURE_NO_WARNINGS

/**インクルード部**/
#include "ResourceCSV.h"
#include <sstream>

/**
* @fn		ResourceCSV::ResourceCSV
* @brief	癖になってるんだよね、コンストラクタするの
*/
ResourceCSV::ResourceCSV() {

}

/**
* @fn		ResourceCSV::~ResourceCSV
* @brief	デストラクタお前はここで消えねばならん・・・わかるな
*/
ResourceCSV::~ResourceCSV() {

}

/**
* @fn		ResiurceCSV::Load
* @brief	CSVファイルの読込
* @param	(string)	読み込むファイルの名前
* @return	(bool)		問題なく読み込めたかどうか
*/
bool ResourceCSV::Load(std::string fileName) {
	//ファイルの読込
	FILE* fp;
	fp = fopen(fileName.c_str(),"rt");
	if (!fp) {
		//読み込めなかった
		return false;
	}

	//中身の吸出し
	size_t fileSize;
	fseek(fp, 0, SEEK_END);		//	ファイル終端までポインタを移動
	fileSize = ftell(fp);		//	ファイルポインタが始点からどれだけ離れているか = サイズ
	fseek(fp, 0, SEEK_SET);		//	ファイルの先頭に移動
	std::string text;
	text.resize(fileSize);		//	vectorみたいに扱える???
	if (fileSize > 0)
		fread(&text.front(), fileSize, 1, fp);	//	textにtextの先頭からfileSize分の読み込みを１回する

	//ファイルを閉じる
	fclose(fp);

	//--- csvの解析
//	1行ずつ区切る
	std::vector<std::string> lines;
	int stsrtIndex = 0;				//	区切りの開始位置
	int endIndex = 0;				//	区切りの終了位置
	while (endIndex != std::string::npos)	// string::npos = findで見つからなかった時の値
	{
		//	'\n'(改行の文字記号が出てくる位置の探索
		endIndex = (int)text.find("\n", stsrtIndex);	//	見つからなかったらstring::nposの値を格納

		//	見つかってたら格納して読み込みますよ
		if (endIndex != std::string::npos)
		{
			//	探索開始位置から見つかった部分までの文字を切り出し
			lines.push_back(text.substr(stsrtIndex, endIndex - stsrtIndex));
		}

		//	次の探索開始位置の更新
		stsrtIndex = endIndex + 1;
	}

	//	1セルずつデータを格納
	std::vector<std::string>::iterator linesItr = lines.begin(); // stringのvector
	while (linesItr != lines.end())
	{
		Column column;	// 列

		stsrtIndex = 0;
		endIndex = 0;

		while (endIndex != std::string::npos)
		{
			//	','の位置を探索
			endIndex = (int)linesItr->find(",", stsrtIndex);	// その行の,を探す

			//	先ほど取り出した行を','で区切って取り出す
			if (endIndex != std::string::npos)
			{
				//	探索開始位置から見つかった部分までの文字を切り出し
				column.push_back(linesItr->substr(stsrtIndex, endIndex - stsrtIndex));
			}
			else
			{
				column.push_back(linesItr->substr(stsrtIndex));
			}

			//	次の探索開始位置の更新
			stsrtIndex = endIndex + 1;
		}

		//	セルごとに区切った行を全体のデータ(grid)へ追加
		m_Grid.push_back(column);

		//	次の行
		linesItr++;
	}

	return true;

}

/**
* @fn		ResourceCSV::Save
* @brief	CSVファイルに書き込む
* @param	(string)	読込先の名前
* @return	(bool)	無事に書き込めたかどうか
*/
bool ResourceCSV::Save(std::string fileName) {
	//	書込み
	FILE *pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	書き込み先が
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;

	for (auto&& row : m_Grid)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// セルの中身格納
			//fwrite(cell.c_str(), sizeof(char) * cell.size(), 1, pFile);
			fprintf(pFile, cell.c_str());
			//	csvはセルを","で区切る
			if ((int)row.size() != cnt_x)	// 最後はいらない
			{
				//fwrite(",", sizeof(char), 1, pFile);
				fprintf(pFile, ",");
			}
		}

		//	csvは"\n"で改行する
		if ((int)row.size() != cnt_y)	// 最後はいらない
		{
			//fwrite("\n", sizeof(char) * 2, 1, pFile);
			fprintf(pFile, "\n");
		}

		// カウント初期化
		cnt_x = 0;
	}

	fclose(pFile);

	return true;
}

/**
* @fn		ResourceCSV::GetStr
* @brief	セルの縦横比を貰う
* @param	(int)	横の数
* @param	(int)	縦の数
* @return	指定の場所の文字情報を返す(中に誰もいませんよ)
*/
std::string ResourceCSV::GetStr(int x,int y) {
	return m_Grid[y][x];
}

/**
* @fn		ResourceCSV::GetInt
* @brief	セルの文字列を数値に変える
* @param	(int)	横の数
* @param	(int)	縦の数
* @return	(int)	返す数値
*/
int ResourceCSV::GetInt(int x, int y) {
	//文字列をintに変換する
	return atoi(m_Grid[y][x].c_str);
}

/**
* @fn		ResourceCSV::GetFloat
* @brief	セルの文字をfloatで取得
* @param	(int)	横の数
* @param	(int)	縦の数
* @return	(float)	返す数値(it浮動小数点)
*/
float ResourceCSV::GetFloat(int x,int y) {
	//文字列をfloatに変える
	return (float)atof(m_Grid[y][x].c_str);
}

/**
* @fn		ResourceCSV::GetRowSize
* @brief	行の数を取得す
* @return	(int)	何行あったか返す
*/
int ResourceCSV::GetRowSize() {
	return  (int)m_Grid.size();
}

/**
* @fn		ResourceCSV::GetColumnSize
* @brief	列の数を取得するよ
* @param	(int)	何行目かの数値
* @return	(int)	どのくらいあったかを返す
*/
int ResourceCSV::GetColumnSize(int row) {
	//例外処理
	if ((int)m_Grid.size() <= row) {
		return 0;
	}

	return (int)m_Grid[row].size();
}

/**
* @fn		ResourceCSV::SetStr
* @brief	セルに文字を入れる
* @param	(int)		横の数
* @param	(int)		縦の数
* @param	(string)	入れたい文字列
*/
void ResourceCSV::SetStr(int x,int y,std::string str) {
	m_Grid[y][x] = str;
}

/**
* @fn		ResourceCSV::SetInt
* @brief	セルの中に数値を入れる
* @param	(int)	横の数
* @param	(int)	縦の数
* @param	(int)	入れたい数値
*/
void ResourceCSV::SetInt(int x,int y,int num) {
	// 足りない行は追加
	if (y >= (int)m_Grid.size())
	{
		m_Grid.resize(y + 1);
	}

	// 足りない列は追加
	if (x >= (int)m_Grid[y].size())
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_Grid[y].push_back(ss.str());
	}
	else
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_Grid[y][x] = ss.str();
	}
}

/**
* @fn		ResourceCSV::SetFloat
* @brief	セルに実数を入れる
* @param	(int)	横の数
* @param	(int)	縦の数
* @param	(float)	入れたい実数
*/
void ResourceCSV::SetFloat(int x,int y,float num) {
	// 足りない行は追加
	if (y >= (int)m_Grid.size())
	{
		m_Grid.resize(y + 1);
	}

	// 足りない列は追加
	if (x >= (int)m_Grid[y].size())
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_Grid[y].push_back(ss.str());
	}
	else
	{
		// 整数を文字として格納する
		std::ostringstream ss;
		ss << num;
		m_Grid[y][x] = ss.str();
	}
}

/**
* @fn		ResourceCSV::SetRowSize
* @brief	行の数を設定
* @param	(int)	どの位の数に設定
*/
void ResourceCSV::SetRowSize(int size) {
	m_Grid.resize(size);
}

/**
* @fn		ResourceCSV::SetColumnSize
* @brief	列の数を設定
* @param	(int)	行の指定
* @param	(int)	どの位の大きさを設定
*/
void ResourceCSV::SetColumnSize(int row,int size) {
	//例外処理
	if ((int)m_Grid.size() <= row ){
		return;
	}
	m_Grid[row].resize(size);
}