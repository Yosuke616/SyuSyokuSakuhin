//===== インクルード =====
#include "Object.h"
#include "Component.h"


// デフォルトコンストラクタ（使わない）
Object::Object()
	: m_bDelete(false),
	m_eDrawOrder(MAX_DRAW_ORDER), m_eUpdateOrder(MAX_UPDATE_ORDER)
{
}

/**
* @fn		Object::Object(std::string szName, UPDATE_ORDER UpdateOrder, DRAW_ORDER DrawOrder)
* @brief	オブジェクト生成時に使用するコンストラクタ
* @param	(string)		オブジェクト名
* @param	(UPDATE_ORDER)	オブジェクトの更新の順番
* @param	(DRAW_ORDER)	オブジェクトの描画の順番
* @detail	更新、描画の順番はObjectInfo.h
*/
Object::Object(std::string szName, UPDATE_ORDER UpdateOrder, DRAW_ORDER DrawOrder)
	: m_bDelete(false)
{
	m_szName = szName;		//	名前登録
	m_eUpdateOrder = UpdateOrder;	//	更新リストの順位をセット
	m_eDrawOrder = DrawOrder;	//	描画リストの順位をセット
}

/**
* @fn		Object::~Object
* @brief	オブジェクトの削除
* @detail	オブジェクトが持つコンポーネントの削除
*/
Object::~Object()
{
	//	機能リストの削除
	for (auto&& component : ComponentList)
	{
		delete component;
		component = nullptr;
	}

	//	機能リスト削除
	ComponentList.clear();
}

/**
* @fn		Object::Start
* @brief	オブジェクトの初期化
* @detail	オブジェクトが持つコンポーネントの初期化
*/
void Object::Start()
{
	//	各コンポーネントの初期化
	for (auto&& component : ComponentList)
		component->Start();
}

/**
* @fn		Object::Update
* @brief	オブジェクトの更新
* @detail	オブジェクトが持つコンポーネントの更新
*/
void Object::Update()
{
	//	各コンポーネントの更新
	for (auto&& component : ComponentList)
	{
		// 更新するか(更新だけ止めたいときこれをfalseにする)
		if (component->m_bUpdateFlag == false) continue;

		// コンポーネントの更新	
		component->Update();
	}


	// 削除フラグの確認
	ComponentList.remove_if([](Component* pObj)
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
* @brief	オブジェクトの描画
* @detail	オブジェクトが持つコンポーネントの描画
*/
void Object::Draw()
{
	//	各コンポーネントの描画
	for (auto&& component : ComponentList)
	{
		// 描画フラグ
		if (component->m_bDrawFlag == false) continue;

		// コンポーネントの描画
		component->Draw();
	}
}

/**
* @fn		Object::OnCollisionEnter
* @brief	オブジェクトとの衝突処理
* @param	(Object*)	衝突した相手のポインタ
* @detail	オブジェクトが持つコンポーネントの衝突処理
*/
void Object::OnCollisionEnter(Object* pObject)
{
	//	各コンポーネントの衝突時の処理
	for (auto&& component : ComponentList)
	{
		// 更新するか(更新だけ止めたいときこれをfalseにする)
		if (component->m_bUpdateFlag == false) continue;

		// 衝突処理
		component->OnCollisionEnter(pObject);
	}
}

/**
* @fn		Object::Delete
* @brief	オブジェクトの削除をする関数
*/
void Object::Delete()
{
	m_bDelete = true;
}

// 描画順の取得
DRAW_ORDER Object::GetDrawOrder()
{
	return m_eDrawOrder;
}

// 更新順の取得
UPDATE_ORDER Object::GetUpdateOrder()
{
	return m_eUpdateOrder;
}

// 機能停止
void Object::StopUpdate()
{
	for (auto&& component : ComponentList)
	{
		// 更新フラグOFF
		component->m_bUpdateFlag = false;
	}
}

// 機能の再開
void Object::ResumeUpdate()
{
	for (auto&& component : ComponentList)
	{
		// 更新フラグON
		component->m_bUpdateFlag = true;
	}
}

// 描画停止
void Object::StopDraw()
{
	for (auto&& component : ComponentList)
	{
		// 描画フラグON
		component->m_bDrawFlag = false;
	}
}
// 描画再開
void Object::ResumeDraw()
{
	for (auto&& component : ComponentList)
	{
		// 描画フラグON
		component->m_bDrawFlag = true;
	}
}

// 機能再開(更新と描画
void Object::Use()
{
	for (auto&& component : ComponentList)
	{
		// 更新再開
		component->m_bUpdateFlag = true;

		// 描画再開
		component->m_bDrawFlag = true;
	}
}

// 機能の再開(更新と描画
void Object::NoUse()
{
	for (auto&& component : ComponentList)
	{
		// 更新停止
		component->m_bDrawFlag = false;

		// 描画停止
		component->m_bDrawFlag = false;
	}
}