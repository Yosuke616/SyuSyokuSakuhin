/*
	Object.h
	ベースとなるオブジェクトクラス
	志水
	2022/6/7
 */

 //===== インクルードガード ======
#ifndef __OBJECT_H__
#define __OBJECT_H__

//===== インクルード ======
#include "mainApp.h"
#include "ObjectInfo.h"

#include <list>
#include <string>

//===== 前方宣言 =====
class Component;



//===== クラス定義 ======
class Object
{
private:
	//=== メンバ変数
	std::list<Component*>	ComponentList;	//	機能リスト
	std::string		m_szName;				//	オブジェクト名
	UPDATE_ORDER	m_eUpdateOrder;			//	更新の順番
	DRAW_ORDER		m_eDrawOrder;			//	描画の順番
	bool			m_bDelete;				//	デリートフラグ

public:
	//=== メンバ関数
	Object();									//	デフォルトコンストラクタ(使わない)
	Object(std::string szName					//	コンストラクタ
		, UPDATE_ORDER UpdateOrder
		, DRAW_ORDER DrawOrder);

	~Object();									//	デストラクタ
	void Start();								//	開始
	void Update();								//	更新
	void Draw();								//	描画
	void OnCollisionEnter(Object*);				//	衝突処理（接触したタイミングで実行）

	void StopUpdate();	// 機能停止(更新だけ
	void ResumeUpdate();// 機能再開(更新だけ

	// 機能再開(更新と描画
	void Use();
	void NoUse();


	void StopDraw();		// 描画停止(更新
	void ResumeDraw();		// 描画再開(更新

	//getter
	DRAW_ORDER GetDrawOrder();				//	描画順の取得
	UPDATE_ORDER GetUpdateOrder();			//	更新順の取得
	//setter
	void Delete();							//	オブジェクトの削除

/**
 * @fn		GetComponent()
 * @brief	コンポーネントを取得
 * @return	テンプレートで指定したクラスのポインタ	失敗の場合nullptr
 * @detail	オブジェクトが持つコンポーネント(機能)リストから
 *			指定したコンポーネントクラスのポインタを取得する関数
 */
	template<class T>
	T* GetComponent()
	{
		for (auto&& com : ComponentList)
		{
			//	テンプレートで指定したクラスに型キャストする
			T* buff = dynamic_cast<T*>(com);

			//	成功時
			if (buff != nullptr)
				return buff;
		}

		//	失敗したとき
		return nullptr;
	}

	/**
	 * @fn		AddComponent()
	 * @brief	オブジェクトにコンポーネントを追加する
	 * @return	テンプレートで指定したクラスのポインタ
	 * @detail	オブジェクトが持つコンポーネント(機能)リストに指定したコンポーネントを追加する
	 *			この時、コンポーネントの所有者オブジェクトのポインタも設定される
	 */
	template<class T>
	T* AddComponent()
	{
		T* component = new T();		//	コンポーネントの生成
		component->Parent = this;	//	このコンポーネントの持ち主の設定

		//	更新順序を決める
		std::list<Component*>::iterator itr = ComponentList.begin();
		for (auto&& com : ComponentList)
		{
			//	現在のオブジェクトの機能を取得
			Component* buff = (Component*)com;

			//	更新の優先度が同じとき、ループから抜ける
			if (component->m_eUpdateOrder <= buff->m_eUpdateOrder)	break;
			else	itr++;	//	更新順序を遅らせる
		}

		//	コンポーネントリストに挿入
		ComponentList.insert(itr, component);

		return component;
	}

	// 名前の取得
	constexpr const std::string& GetName() const
	{
		return m_szName;
	}

	// 削除フラグの取得
	constexpr const bool GetDeleteFlag() const
	{
		return m_bDelete;
	}
};
#endif // __OBJECT_H__