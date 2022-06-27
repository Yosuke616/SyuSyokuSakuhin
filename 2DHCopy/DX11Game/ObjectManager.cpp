//インクルード部
#include "ObjectManager.h"
#include "Component.h"
#include "Object.h"
#include "ColliderComponent.h"

//静的メンバ変数
ObjectManager* ObjectManager::m_pInstance = nullptr;	//インスタンス

/**
* @fn		ObjectManager::ObjectManager
* @brief	コンストラクタ
*/
ObjectManager::ObjectManager() {

}

/**
* @fn		ObjectManager::~ObjectManager
* @brief	デストラクタ
*/
ObjectManager::~ObjectManager() {

}

/**
* @fn		ObjectManager::GetInstance
* @brief	objectマネージャーのインスタンス化
* @return	シングルトンで生成されたObjectManagerクラスのポインタ
*/
ObjectManager* ObjectManager::GetInstance() {
	if (!m_pInstance) {
		//インスタンスが作られていない場合は作る
		m_pInstance = new ObjectManager();
	}
	return m_pInstance;
}

/**
* @fn		ObjectManager::Start
* @brief	オブジェクトマネージャーの初期化
* @detail   各々のオブジェクトが持つコンポーネントの初期化
*/
void ObjectManager::Start() {
	//全オブジェクトの初期化
	for (auto&& object : m_UpdateList) {
		object->Start();
	}
}

/**
* @fn		Object::Unint
* @brief	全てのオブジェクトの削除
* @detail	各々が持つコンポーネントの削除
*			このクラスも解放する
*/
void ObjectManager::Uninit() {
	//オブジェクトリストの削除
	for (auto&& Object : m_UpdateList) {
		delete Object;
	}

	//リストをカラにする
	m_UpdateList.clear();
	m_DrawList.clear();

	//このオブジェクトの削除
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}

}

/**
* @fn		Object::Update
* @brief	全てのオブジェクトの更新
* @detail	各々のオブジェクトが持っているコンポーネントの更新
*			デリートフラグが立っているオブジェクトは更新の最後で削除している
*/
void ObjectManager::Update() {
	//オブジェクトの更新
	for (auto&& object : m_UpdateList) {
		object->Update();
	}

	//衝突処理
	CCollider::UpdateColliders();

	//オブジェクト削除の確認
	m_UpdateList.remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});
	m_DrawList.remove_if([](Object* pObj) {
		if (pObj->GetDeleteFlag()) {
			delete pObj;
			return true;
		}
		else{
			return false;
		}
	});
	//当たり判定のリストの削除
	CCollider::GetColliderList().remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});

}

/**
* @fn		Object::UpdateEdit()
* @brief	削除フラグの確認を行う
* @detail	debug用の関数なので最終的には使わない			
*/
void ObjectManager::UpdateEdit() {
	//	オブジェクト削除の確認
	m_UpdateList.remove_if([](Object* pObj)
	{
		return pObj->GetDeleteFlag();
	});
	m_DrawList.remove_if([](Object* pObj)
	{
		if (pObj->GetDeleteFlag())
		{
			delete pObj;
			return true;
		}
		else return false;
	});
	//当たり判定の削除フラグの確認
	CCollider::GetColliderList().remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});
}

/**
* @fn		ObjectManager::Draw
* @brief	全てのオブジェクトの描画
* @detail	各々のオブジェクトが持つコンポーネントの描画
*/
void ObjectManager::Draw() {
	SetZBuffer(true);
	for (auto&& object:m_DrawList) {
		object->Draw();
	}
}

/**
* @fn		ObjectManager::AddObject
* @brief	オブジェクトの追加
* @param    (Object*) 追加するオブジェクト
* @detail	登録されている優先順位で追加している
*			ObjectInfo.hで優先順位を登録している
*/
void ObjectManager::AddObject(Object* pObject) {
	//オブジェクトの初期化
	pObject->Start();

	//更新リストに追加
	std::list<Object*>::iterator itrUpdate = m_UpdateList.begin();
	for (auto&& UpdateObject : m_UpdateList) {
		if (pObject->GetUpdateOrder() > UpdateObject->GetUpdateOrder()) {
			itrUpdate++;
			continue;
		}
		else {
			break;
		}
	}
	m_UpdateList.insert(itrUpdate,pObject);

	//描画リストに追加
	std::list<Object*>::iterator itrDraw = m_DrawList.begin();
	for (auto&& DrawObject : m_DrawList) {
		if (pObject->GetDrawOrder() > DrawObject->GetDrawOrder()) {
			itrDraw++;
			continue;
		}
		else {
			break;
		}
	}
	m_DrawList.insert(itrDraw,pObject);
}

/**
* @fn		ObjectManager::GetUpdateList
* @brief	オブジェクトリストの取得
*/
std::list<Object*>& ObjectManager::GetUpdateList() {
	return m_UpdateList;
}

/**
* @fn		ObjectManager::GetDrawList
* @brief	描画リストの取得
*/
std::list<Object*>& ObjectManager::GetDrawList() {
	return m_DrawList;
}

/**
* @fn		ObjectManager::DeleteObjectByName
* @brief	指定したリストから名前に一致したオブジェクトを削除する
* @param	(string)		削除するオブジェクト名
* @param	(list<Object*>)	リストの指定
*/
void ObjectManager::DeleteObjectByName(std::string pszName, std::list<Object*>& list) {
	for (auto&& obj : list) {
		//名前が一致したオブジェクトを削除
		if (obj->GetName() == pszName) {
			obj->Delete();
		}
	}
}

/**
* @fn		ObjectManager::GetGameObject
* @brief	名前でオブジェクトを検索
* @param	(string)		削除するオブジェクト名
* @return	存在した場合：そのオブジェクトのポインタ　存在しない場合：nullptr
*/
Object* ObjectManager::GetGameObject(std::string name) {
	for (auto&& obj:m_UpdateList) {
		//名前が一致したオブジェクトを削除
		if (obj->GetName() == name) {
			return obj;
		}
	}
	return nullptr;
}

/**
* @fn		ObjectManager::GetObjectList
* @brief	名前が一致したオブジェクトをリストに追加
* @param	(string)		オブジェクトの名前
* @rerurn	(list<Object*>)	リストを返す
*/
std::list<Object*> ObjectManager::GetObjectList(std::string name) {
	std::list<Object*>list;
	for (auto&& obj:m_UpdateList) {
		if (obj->GetName() == name) {
			list.push_back(obj);
		}
	}
	return list;
}

/**
* @fn		ObjectManager::NoFunction
* @brief	関数の更新を止める
*/
void ObjectManager::NoFunction() {
	for (auto object:m_UpdateList) {
		object->StopUpdate();
	}
}

/**
* @fn		ObjectManager::Resume
* @brief	機能を再開する
*/
void ObjectManager::Resume() {
	for (auto object:m_UpdateList) {
		object->ResumeUpdate();
	}
}

/**
* @fn		ObjectManager::GetGameObjects
* @brief	名前が一致しているオブジェクトリストを追加する
* @param	(string)		検索するオブジェクトの名前
* @param	(list<Object*>)	オブジェクトのリスト
*/
void ObjectManager::GetGameObjects(std::string name, std::list<Object*>& resultList) {
	for (auto&& object:m_UpdateList) {
		if (object->GetName() == name) {
			resultList.push_back(object);
		}
	}
}
