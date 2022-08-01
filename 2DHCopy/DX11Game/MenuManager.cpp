/**インクルード部**/
#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TextureManager.h"
#include "sceneGame.h"

/**静的メンバ変数**/
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

/**
* @fn		MenuManager::MenuManager
* @brief	キノコ狩りの男　ｺﾝｽﾄﾗｸﾀｯ
*/
MenuManager::MenuManager() 
	:m_nDelayCnt(-1),m_nCreateMenu(STATE_NONE){
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	全てのメニューオブジェクトの解放
*/
MenuManager::~MenuManager() {
	//オブジェクトリストの解放
	for (auto&& menuObject :m_MenuList) {
		delete menuObject;
	}
	m_MenuList.clear();
}

/**
* @fn		MenuManager::GetInstance
* @brief	インスタンス化
*/
MenuManager* MenuManager::GetInsutance() {
	if (!m_pInstance) {
		m_pInstance = new MenuManager;
	}
	return m_pInstance;
}

/**
* @fn		MenuManager::Destroy
* @brief	インスタンスの破棄
*/
void MenuManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::Start
* @brief	初期化
*/
void MenuManager::Start() {
	//メニューの初期化
	for (auto&& MenuObject:m_MenuList) {
		MenuObject->Start();
	}
}

/**
* @fn		MenuManager::Update
* @brief	メニューのリストの入っているオブジェクトの更新
*/
void MenuManager::Update() {
	if (m_nDelayCnt > 0) {
		//メニュー作成を遅らせる
		m_nDelayCnt--;

		if (m_nDelayCnt == 0) {
			switch (m_nCreateMenu)
			{
			case TITLE_STATE: CreateTitleMenu(); break;
			default:break;
			}
	
			m_nDelayCnt = -1;
		}
	}

	//メニューオブジェクトの更新
	for (auto&& object : m_MenuList) {
		object->Update();
	}

	//Colliderの更新
	CCollider::GetColliderList().remove_if([](Object* pObj){
		return pObj->GetDeleteFlag();
	});

	//オブジェクト削除の確認
	m_MenuList.remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});
}

/**
* @fn		MenuManager::Draw
* @brief	メニューオブジェクトの描画
*/
void MenuManager::Draw() {
	SetZBuffer(true);
	//メニューオブジェクトの描画
	for (auto&& menuObject:m_MenuList) {
		menuObject->Draw();
	}
}

/**
* @fn		MenuManager::Create
* @brief	メニューの作成
* @param	(int)	どのメニューを作成するかの番号
* @param	(int)	どの位のフレームの後作成するか
*/
void MenuManager::Create(int state,int delayCnt) {
	m_nCreateMenu = state;
	m_nDelayCnt = delayCnt;
}

/**
* @fn		MenuManager::CreateTitleMenu
* @brief	タイトルメニューの作成
*/
void MenuManager::CreateTitleMenu() {
	//オブジェクトリストの削除
	DeleteMenu();

	//必要になってくるボタンを追加する
	//仮
	Object* pButton = new Object("Button_Kari",UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton->SetPosition(0.0f,0.0f);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton->SetSize(100.0f,50.0f);

	//メニューリストに追加
	AddMenu(pButton);

}

/**
* @fn		MenuManager::CeratePauseMenu
* @brief	ポーズメニューの作成と表示
*/
void MenuManager::CreatePauseMenu() {
	//オブジェクトのリストの削除
	DeleteMenu();
}

/**
* @fn		MenuManager::AddMenu
* @brief	メニューリストに追加
* @param	(Object*)	追加するオブジェクトのポインタ
*/
void MenuManager::AddMenu(Object* pMenuObject) {
	//更新リストに追加
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList) {
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder()) {
			itr++;
			continue;
		}
		else {
			break;
		}
	}
	m_MenuList.insert(itr,pMenuObject);

	pMenuObject->Start();
}

/**
* @fn		MenuManager::GetMenuList
* @brief	メニューリストの取得
*/
std::list<Object*>& MenuManager::GetMenuList() {
	return m_MenuList;
}

/**
* @fn		MenuManager::DeleteMenu
* @brief	メニューリストをカラにする
*/
void MenuManager::DeleteMenu() {
	//オブジェクトリストの削除
	for (auto&& menuObject : m_MenuList) {
		menuObject->Delete();
	}
	m_MenuList.clear();
}