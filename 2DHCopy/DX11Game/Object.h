/*
	オブジェクトのベースクラス
	志水陽祐
	2022/6/9 作成開始
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

//インクルード部
#include "mainApp.h"
#include "ObjInfo.h"

#include <list>
#include <string>

//前方宣言
class Component;

//クラス定義
class Object {
private:
	//メンバ変数
	std::list<Component*> ComponentList;	//機能リスト
	std::string		m_szName;				//オブジェクトの名前
	UPDATE_ORDER	m_eUpdateOrder;			//更新の順番
	DRAW_ORDER      m_eDrawOrder;			//描画の順番
	bool			m_bDelete;				//削除フラグ

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	Object();								//デフォルトコンストラクタ
	Object(std::string szName,				//イニシャライズ付きコンストラクタ
		UPDATE_ORDER UpdateOrder,
		DRAW_ORDER DrawOrder);

	~Object();								//デストラクタ
	void Start();							//開始
	void Update();							//更新
	void Draw();							//描画
	void OnCollisionEnter(Object*);			//衝突判定

	void StopUpdate();						//機能停止(更新)
	void ResumeUpdate();					//機能再開(更新)

	void Use();								//機能再開(描画と更新)
	void NoUse();							//機能再開(描画と更新)

	void StopDraw();						//描画停止
	void ResumeDraw();						//描画再開

	//ゲッター
	DRAW_ORDER GetDrawOrder();				//そのオブジェクトの描画順の取得
	UPDATE_ORDER GetUpdateOrder();			//そのオブジェクトの更新順の取得
	/** @brief コンポーネントリストを得る*/
	std::list<Component*> GetComponentList();

	//セッター
	void Delete();							//オブジェクトの削除

	//GetComponent
	//オブジェクトが持っているコンポーネントを取得する
	//コンポーネントリストから指定したポインタを取得する関数

	template<class T>
	T* GetComponent() {
		for (auto&& com : ComponentList) {
			//テンプレートで指定したクラスにキャストする
			T* buff = dynamic_cast<T*>(com);

			//成功時
			if (buff != nullptr) {
				return buff;
			}
		}
		//失敗したとき
		return nullptr;
	}

	//AddComponent()
	//オブジェクトにコンポーネントに追加する
	//オブジェクトが持つコンポーネントリストに指定したコンポーネントを追加する
	//このとき、コンポーネントの所有者のオブジェクトのポインタも指定される
	template<class T>
	T* AddComponent() {
		T* component = new T();		//コンポーネントの生成
		component->Parent = this;	//コンポネントの持ち主の設定

		//更新順序を決める
		std::list<Component*>::iterator itr = ComponentList.begin();
		for (auto&& com : ComponentList) {
			//現在のオブジェクトの機能を取得
			Component* buff = (Component*)com;

			//更新の優先度が同じとき、ループが抜ける
			if (component->m_eUpdateOrder <= buff->m_eUpdateOrder) {
				break;
			}
			else {
				//更新順序を遅らせる
				itr++;
			}
		}

		//コンポーネントリストに挿入
		ComponentList.insert(itr,component);
		
		return component;
	}

	//名前を取得する
	constexpr const std::string& GetName() const {
		return m_szName;
	}

	//削除フラグの取得
	constexpr const bool GetDeleteFlag() const {
		return m_bDelete;
	}
};



#endif