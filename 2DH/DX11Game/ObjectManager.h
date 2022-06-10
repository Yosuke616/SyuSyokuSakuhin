/*
	ObjectManager.h
	すべてのオブジェクトを管理するクラス
	志水陽祐
	2022/6/7
 */

 //===== インクルードガード ======
#ifndef __COBJECT_MANAGER_H__
#define __COBJECT_MANAGER_H__

//===== インクルード ======
#include <list>
#include <string>


//===== 全方宣言 =====
class Object;


//===== クラス定義 ======
class ObjectManager
{
	//=== メンバ変数
private:
	std::list<Object*>		m_UpdateList;	//	更新リスト
	std::list<Object*>		m_DrawList;		//	描画リスト
	static ObjectManager*	m_pInstance;	//	インスタンス


	//=== メンバ関数
protected:
	ObjectManager();						//	コンストラクタ
public:
	static ObjectManager* GetInstance();	//	インスタンスの取得
	~ObjectManager();						//	デストラクタ
	void Start();							//	開始
	void Update();							//	mode_game  ゲーム更新
	void UpdateEdit();						//	mode_debug ステージ作成時の更新
	void Draw();							//	描画
	void Uninit();							//	終了

	//	オブジェクト追加
	void AddObject(Object* pObject);

	// 名前でオブジェクトを削除
	void DeleteObjectByName(std::string pszName, std::list<Object*>& list);

	// 名前が一致したオブジェクトをリストに追加
	std::list<Object*> GetObjectList(std::string name);

	// 機能停止(更新だけ
	void NoFunction();
	// 機能再開(更新だけ
	void Resume();



	//getter
	Object* GetGameObject(std::string name);//	名前からオブジェクトを特定する
	std::list<Object*>& GetUpdateList();	//	更新リストの取得
	std::list<Object*>& GetDrawList();		//	描画リストの取得

	// 名前が一致しているオブジェクトをリストに追加する
	void GetGameObjects(std::string name, std::list<Object*>& resultList);
};
#endif // __OBJECT_MANAGER_H__