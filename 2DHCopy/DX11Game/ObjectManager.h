/**
* @file		ファイル名
* @brief	オブジェクトマネージャークラス
* @author   Shimizu Yosuke
* @date		2022/6/16
*
* @details  オブジェクトを管理してどれを更新し、どれを描画するかを管理する
* @note		ポインタとかをたくさん使うよ
*/

//インクルードガード
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

//インクルード部
#include <list>
#include <string>

//前方宣言
class Object;

//クラス定義
/**
* @brief	生成されたオブジェクトを管理するクラス
* @details	インスタンス生成されるため1つしか存在しない
*/
class ObjectManager {
private:
	//メンバ変数
	/** @brief オブジェクトをまとめて更新するためのリスト*/	
	std::list<Object*>		m_UpdateList;
	/** @brief オブジェクトをまとめて描画するためのリスト*/
	std::list<Object*>		m_DrawList;
	/** @brief インスタンス生成用変数*/
	static ObjectManager*	m_pInstance;

	//メンバ関数

protected:
	//メンバ変数
	/** @brief コンストラクタ*/
	ObjectManager();

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief*/
	static ObjectManager* GetInstance();
	/** @brief デストラクタ*/
	~ObjectManager();
	/** @brief 初期化処理*/
	void Start();
	/** @brief 更新処理*/
	void Update();
	/** @brief ステージ作成時の更新処理*/
	void UpdateEdit();
	/** @brief 描画処理*/ 
	void Draw();
	/** @brief 終了処理*/
	void Uninit();

	/** @brief オブジェクトを追加する*/
	void AddObject(Object* pObject);

	/** @brief 名前検索でオブジェクトを削除する*/
	void DeleteObjectByName(std::string pszName,std::list<Object*>& list);

	/** @brief 名前が一致したオブジェクトをリストに追加*/
	std::list<Object*> GetObjectList(std::string name);

	/** @brief 更新を停止する関数*/
	void NoFunction();

	/** @brief 更新を再開する関数*/
	void Resume();

	//getter
	/** @brief 名前からオブジェクトを特定する*/
	Object* GetGameObject(std::string name);
	/** @brief 更新リストの取得*/
	std::list<Object*>& GetUpdateList();
	/** @brief 描画リストの取得*/
	std::list<Object*>& GetDrawList();

	/** @brief 名前が一致しているオブジェクトリストを追加する*/
	void GetGameObjects(std::string name,std::list<Object*>& resultList);
};


#endif