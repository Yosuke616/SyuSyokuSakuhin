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
#include "imgui.h"
#include "SceneManager.h"

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
	//メニューリスト内のオブジェクトの初期化が終わったらイテレータを先頭に持ってくる
	m_itr_Menu = m_MenuList.begin();
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

	//ここに選択されているボタンなどを選ぶ関数を追加する
	//選ばれているボタンの色を変えるなど
	SelectButton();
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

#ifdef _DEBUG /**imguiでデバッグのやつを出す**/
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Button_State");
	Text("B_STATE : %d", m_MenuList.size());
	End();

#endif
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

	//**********************************************************************************************
	//仮
	Object* pButton2 = new Object("Button_Kari2", UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto transButton2 = pButton2->AddComponent<CTransform>();
	auto drawButton2 = pButton2->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton2->SetPosition(0.0f, -50.0f);
	drawButton2->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton2->SetSize(100.0f, 50.0f);

	//メニューリストに追加
	AddMenu(pButton2);
	//***************************************************************************************************

	//**********************************************************************************************
	//仮
	Object* pButton3 = new Object("Button_Kari3", UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto transButton3 = pButton3->AddComponent<CTransform>();
	auto drawButton3 = pButton3->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton3->SetPosition(0.0f, -100.0f);
	drawButton3->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton3->SetSize(100.0f, 50.0f);

	//メニューリストに追加
	AddMenu(pButton3);
	//***************************************************************************************************

	//追加されたオブジェクトの初期化を行う
	Start();
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
	//m_MenuList.insert(itr,pMenuObject);
	m_MenuList.push_back(pMenuObject);

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

/**
* @fn		MenuManager::GetCreateMenu
* @brief	作られるメニューの番号を返す
* @return	(int)	どの番号のメニューが作られるか
*/
int MenuManager::GetCreateMenu() {
	return m_nCreateMenu;
}

/**
* @fn		MenuManager::SelectButton
* @brief	選ばれたボタンの色変えや、押された時の処理
*/
void MenuManager::SelectButton() {
	//色を全てデフォルトに戻しておく
	for (auto&& menuObject : m_MenuList) {
		menuObject->GetComponent<CDraw2D>()->SetColor(1.0f,1.0f,1.0f);
	}

	//キー入力でボタンを選べるようにする
	if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
		//上方向
		//イテレーターがスタート地点だった場合終わりに行かせる
		if (m_itr_Menu == m_MenuList.begin()) {
			m_itr_Menu = (m_MenuList.end());
			m_itr_Menu--;
		}
		else {
			//そうでない場合は1個下げる
			m_itr_Menu--;
		}
	}
	else if(InputManager::Instance()->GetKeyTrigger(DIK_S)){
		//下方向
		//イテレーターが終端にいた場合スタート地点に行かせる
		std::list<Object*>::iterator itr = m_MenuList.end();
		itr--;
		if (m_itr_Menu == itr) {
			m_itr_Menu = m_MenuList.begin();
		}
		else {
			m_itr_Menu++;
		}
	}


	//選ばれたボタンの色を変える
	switch (m_nCreateMenu) {
	case TITLE_STATE:
		if ((*m_itr_Menu)->GetName() == "Button_Kari") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f,1.0f,0.0f);
		}else if ((*m_itr_Menu)->GetName() == "Button_Kari2") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f, 1.0f, 0.0f);
		}
		else  if ((*m_itr_Menu)->GetName() == "Button_Kari3") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f, 1.0f, 0.0f);
		}
		
		break;
	default:break;
	}

	//決定ボタンが押された時の処理
	if (InputManager::Instance()->GetKeyTrigger(DIK_RETURN)) {
		PushButton();
	}
}

/**
* @fn		MenuManager::PushButton
* @brief	決定ボタンが押された時の処理
* @detail	シーンごとに処理を変えるのでスイッチ文がとんでもなくなる予想
*/
void MenuManager::PushButton() {
	//作られたシーンごとで処理を変える
	switch (m_nCreateMenu) {
	case TITLE_STATE:
		if ((*m_itr_Menu)->GetName() == "Button_Kari") {
			//シーン遷移
			SceneManager::Instance()->SetScene(SCENE_GAME);
		}
		
		break;

	default:break;
	}
}