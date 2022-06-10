#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>

//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------
#include "TitleScene.h"
//-----------------------------------------------------------

//===== 静的メンバ変数 =====
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

// インスタンス化
MenuManager* MenuManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new MenuManager;
	}

	return m_pInstance;
}

// インスタンスの破棄
void MenuManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::MenuManager
* @brief	コンストラクタ
*/
MenuManager::MenuManager()
	: m_nDelayCnt(-1), m_nCreateState(STATE_NONE)
{
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	全てのメニューオブジェクトの開放
*/
MenuManager::~MenuManager()
{
	//	オブジェクトリストの削除
	for (auto&& menuObject : m_MenuList)
		delete menuObject;
	m_MenuList.clear();
}

/**
* @fn		MenuManager::~Start
* @brief	初期化
*/
void MenuManager::Start()
{
	//	メニューの初期化
	for (auto&& MenuObject : m_MenuList)
		MenuObject->Start();
}

/**
* @fn		MenuManager::Update
* @brief	全てのオブジェクトの更新
* @detail	各々のオブジェクトが持つコンポーネントの更新
*/
void MenuManager::Update()
{
	if (m_nDelayCnt > 0)
	{
		// メニュー作成を遅らせる
		m_nDelayCnt--;

		if (m_nDelayCnt == 0)
		{
			switch (m_nCreateState)
			{
			case STATE_TITLE:	 CreateTitleMenu(); break;
			case STATE_OPTION:	  break;
			case STATE_SELECT:	  break;
			case STATE_GAMEOVER:  break;
			default:
				break;
			}

			m_nDelayCnt = -1;
		}
	}

	//	メニューオブジェクトの更新
	for (auto&& object : m_MenuList)
	{
		object->Update();
	}

	//	オブジェクト削除の確認
	m_MenuList.remove_if([](Object* pObj)
	{
		return pObj->GetDeleteFlag();
	});
}

/**
* @fn		MenuManager::DrawMenu
* @brief	メニューオブジェクトの描画
* @detail	各々のオブジェクトが持つコンポーネントの描画
*/
void MenuManager::Draw()
{
	SetZBuffer(true);
	//	メニューオブジェクト描画
	for (auto&& menuObject : m_MenuList)
		menuObject->Draw();
}

//  指定フレーム後にメニュー作成
void MenuManager::Create(int state, int delayCnt)
{
	m_nCreateState = state;

	m_nDelayCnt = delayCnt;
}

// タイトルメニュー表示
void MenuManager::CreateTitleMenu()
{
	// オブジェクトリストの削除
	DeleteMenu();
}

/**
* @fn		MenuManager::AddMenu
* @brief	メニューリストに追加
* @param	(Object*) 追加するオブジェクト
*/
void MenuManager::AddMenu(Object* pMenuObject)
{
	//	更新リストに追加
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList)
	{
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	//	メニューリストに追加
	m_MenuList.insert(itr, pMenuObject);


	// オブジェクト初期化
	pMenuObject->Start();
}

// メニューリストの取得
std::list<Object*>& MenuManager::GetMenuList()
{
	return m_MenuList;
}

// メニューリストを空にする
void MenuManager::DeleteMenu()
{
	//	オブジェクトリストの削除
	for (auto&& menuObject : m_MenuList)
		menuObject->Delete();

	m_MenuList.clear();
}



