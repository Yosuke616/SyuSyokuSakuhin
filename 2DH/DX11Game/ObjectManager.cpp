//===== インクルード =====
#include "ObjectManager.h"
#include "Component.h"
#include "Object.h"

//===== 静的メンバ変数 =====
ObjectManager* ObjectManager::m_pInstance = nullptr;	//	インスタンス

 /**
 * @fn		ObjectManager::ObjectManager
 * @brief	コンストラクタ
 */
ObjectManager::ObjectManager()
{
}

/**
* @fn		ObjectManager::~ObjectManager
* @brief	デストラクタ
*/
ObjectManager::~ObjectManager()
{
}

/**
 * @fn		ObjectManager::GetInstance
 * @brief	オブジェクトマネージャーのインスタンス化
 * @return	シングルトンで生成されたObjectManagerクラスのポインタ
*/
ObjectManager* ObjectManager::GetInstance()
{
	if (!m_pInstance)
	{
		//	インスタンスが作られていない場合は作る
		m_pInstance = new ObjectManager();
	}
	return m_pInstance;
}

/**
* @fn		Object::Start
* @brief	全てのオブジェクトの初期化
* @detail	各々のオブジェクトが持つコンポーネントの初期化
*/
void ObjectManager::Start()
{
	//	全オブジェクトの初期化
	for (auto&& object : m_UpdateList)
		object->Start();
}

/**
* @fn		Object::Uninit
* @brief	全てのオブジェクトの削除
* @detail	各々のオブジェクトが持つコンポーネントの削除
*			このクラスも解放する
*/
void ObjectManager::Uninit()
{
	//	オブジェクトリストの削除
	for (auto&& Object : m_UpdateList)
		delete Object;


	//	リストを空にする
	m_UpdateList.clear();
	m_DrawList.clear();	//	更新リストと描画リストは同じオブジェクトが入っているため片方でデリートするだけでいい

	//	このオブジェクトの削除
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		Object::Update
* @brief	全てのオブジェクトの更新
* @detail	各々のオブジェクトが持つコンポーネントの更新
*			デリートフラグが立っているオブジェクトは更新の最後で削除している
*/
void ObjectManager::Update()
{
	// オブジェクトの更新
	for (auto&& object : m_UpdateList)
		object->Update();


	// sオブジェクト削除の確認
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
}

//	mode_debug 削除フラグの確認のみ行う
void ObjectManager::UpdateEdit()
{
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
}


/**
* @fn		Object::Draw
* @brief	全てのオブジェクトの描画
* @detail	各々のオブジェクトが持つコンポーネントの描画
*/
void ObjectManager::Draw()
{
	SetZBuffer(true);
	for (auto&& object : m_DrawList)
		object->Draw();

}


/**
* @fn		ObjectManager::AddObject
* @brief	オブジェクトを追加
* @param	(Object*) 追加するオブジェクト
 * @detail	登録されている優先順位で追加している
*			ObjectInfo.hで優先順位を登録しています
*/
void ObjectManager::AddObject(Object* pObject)
{
	// オブジェクト初期化
	pObject->Start();

	//	更新リストに追加
	std::list<Object*>::iterator itrUpdate = m_UpdateList.begin();
	for (auto&& UpdateObject : m_UpdateList)
	{
		if (pObject->GetUpdateOrder() > UpdateObject->GetUpdateOrder())
		{
			itrUpdate++;
			continue;
		}
		else break;
	}
	m_UpdateList.insert(itrUpdate, pObject);

	//	描画リストに追加
	std::list<Object*>::iterator itrDraw = m_DrawList.begin();
	for (auto&& DrawObject : m_DrawList)
	{
		if (pObject->GetDrawOrder() > DrawObject->GetDrawOrder())
		{
			itrDraw++;
			continue;
		}
		else break;
	}
	m_DrawList.insert(itrDraw, pObject);
}


//	+++オブジェクトリストの取得
std::list<Object*>& ObjectManager::GetUpdateList()
{
	return m_UpdateList;
}

//	+++更新リストの取得
std::list<Object*>& ObjectManager::GetDrawList()
{
	return m_DrawList;
}

/**
* @fn		ObjectManager::DeleteObjectByName
* @brief	指定したリストから名前に一致したオブジェクトを削除する
* @param	(string)		削除するオブジェクト名
* @param	(list<Object*>) リストの指定
*/
void ObjectManager::DeleteObjectByName(std::string pszName, std::list<Object*>& list)
{
	for (auto&& obj : list)
	{
		//	名前が一致したオブジェクトを削除
		if (obj->GetName() == pszName)
		{
			obj->Delete();
		}
	}
}

/**
* @fn		ObjectManager::etGameObject
* @brief	名前でオブジェクトを検索(プレイヤー検索しか使い道なさそう)
* @param	(string)		削除するオブジェクト名
* @return	あった場合 そのオブジェクトポインタ	なかった場合 nullptr
*/
Object* ObjectManager::GetGameObject(std::string name)
{
	for (auto&& obj : m_UpdateList)
	{
		//	名前が一致したオブジェクトを削除
		if (obj->GetName() == name)
		{
			return obj;
		}
	}
	return nullptr;
}


// 名前が一致したオブジェクトをリストに追加
std::list<Object*> ObjectManager::GetObjectList(std::string name)
{
	std::list<Object*> list;
	for (auto&& obj : m_UpdateList)
	{
		if (obj->GetName() == name)
		{
			list.push_back(obj);
		}
	}

	return list;
}

void ObjectManager::NoFunction()
{
	for (auto object : m_UpdateList)
	{
		object->StopUpdate();
	}
}

// 機能再開
void ObjectManager::Resume()
{
	for (auto object : m_UpdateList)
	{
		object->ResumeUpdate();
	}
}





void ObjectManager::GetGameObjects(std::string name, std::list<Object*>& resultList)
{
	for (auto && object : m_UpdateList)
	{
		if (object->GetName() == name)
		{
			resultList.push_back(object);
		}
	}
}