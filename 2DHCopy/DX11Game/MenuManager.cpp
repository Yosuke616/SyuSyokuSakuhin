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
#include "AnimMeshComponent.h"
#include "ModelManager.h"
#include "Draw3dComponent.h"
#include "ObjInfo.h"
#include "PlayerComponent.h"
#include "ObjectManager.h"

/**定数定義**/
/** @brief ステージ選択画面のオブジェクトが、
		   どれだけの速度で移動するか*/
#define STAGEOBJ_VEL	(2.0f)
/** @brief どれだけ移動したかを制御する定数*/
#define MAX_OBJ_MOVE	(150.0f)

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

	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		//オブジェクトリストの解放
		/*for (auto&& menuObject : m_StageObjList) {
			delete menuObject;
		}*/
		m_StageObjList.clear();
		m_SelectSub.clear();
	}
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
	//ゲームシーンでブラックアウトようだったら次に持っていく
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		if ((*m_itr_Menu)->GetName() == "Black") {
			m_itr_Menu++;
		}
	}
	
	//黒いテクスチャは透明状態にする
	m_fBlackAlpha = 0.0f;
	m_nChangeCnt = 100;

	//ステージ選択画面のみ初期化する
	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		m_bRestart = false;
		m_fMoveObj = 0.0f;
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
			case PAUSE_STATE: CreatePauseMenu(); break;
			case SELECT_STATE:break;
			case OPTION_STATE:break;
			case MISS_STATE:  CreateMissMenu(); break;
			case GAMECLEAR_STATE:break;
			case GAMEOVER_STATE:CreateGameOverMenu(); break;
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

	/**シーンごとの処理を追加していく**/

	/** @brief タイトルなど始めたりボタンを選ぶやつ*/
	//応急処置としてシーンで入るようにする
	if (SceneManager::Instance()->GetScene() == SCENE_TITLE) {	
		//ここに選択されているボタンなどを選ぶ関数を追加する
		//選ばれているボタンの色を変えるなど
		if (m_nDelayCnt == -1) {	
			SelectButton();
		}	
	}
	/** @brief ステージセレクト専用の処理*/
	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		if (m_nDelayCnt == -1) {
			StageSelect();
		}
	}

	/** @brief ミスメニュー専用の処理*/
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		if (m_nDelayCnt == -1) {
			Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			if (obj->GetComponent<CPlayer>()->GetPlayerSta() == MISS_PLAYER) {
				MissMenu();
			}else {
				SelectButton();
				//PauseMenu();
			}
		}
	}
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

#pragma region ---タイトルメニュー作成
/**
* @fn		MenuManager::CreateTitleMenu
* @brief	タイトルメニューの作成
*/
void MenuManager::CreateTitleMenu() {
	//オブジェクトリストの削除
	DeleteMenu();

	//タイトルが選ばれているようにする
	m_nCreateMenu = TITLE_STATE;

	//必要になってくるボタンを追加する
	//初めから
	Object* pButton = new Object(UI_BEGIN_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton->SetPosition(UI_BEGIN_POS_X, UI_BEGIN_POS_Y);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(BEGIN_TEX_NUM));
	drawButton->SetSize(UI_BEGIN_SIZE_X, UI_BEGIN_SIZE_Y);

	//メニューリストに追加
	AddMenu(pButton);

	//**********************************************************************************************
	//続きから
	Object* pButton2 = new Object(UI_CONTINUE_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto transButton2 = pButton2->AddComponent<CTransform>();
	auto drawButton2 = pButton2->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton2->SetPosition(UI_CONTINUE_POS_X, UI_CONTINUE_POS_Y);
	drawButton2->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_TEX_NUM));
	drawButton2->SetSize(UI_CONTINUE_SIZE_X, UI_CONTINUE_SIZE_Y);

	//メニューリストに追加
	AddMenu(pButton2);
	//***************************************************************************************************

	//**********************************************************************************************
	//仮
	Object* pButton3 = new Object(UI_END_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto transButton3 = pButton3->AddComponent<CTransform>();
	auto drawButton3 = pButton3->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton3->SetPosition(UI_END_POS_X, UI_END_POS_Y);
	drawButton3->SetTexture(TextureManager::GetInstance()->GetTexture(END_TEX_NUM));
	drawButton3->SetSize(UI_END_SIZE_X, UI_END_SIZE_Y);

	//メニューリストに追加
	AddMenu(pButton3);
	//***************************************************************************************************

	//追加されたオブジェクトの初期化を行う
	Start();
}
#pragma endregion

#pragma region ---ポーズメニュー作成
/**
* @fn		MenuManager::CeratePauseMenu
* @brief	ポーズメニューの作成と表示
*/
void MenuManager::CreatePauseMenu() {
	//オブジェクトのリストの削除
	DeleteMenu();
	//オプションが選ばれているようにする
	m_nCreateMenu = PAUSE_STATE;

	//必要になってくるオブジェクトを追加していく
	//黒背景を作る
	Object* Black = new Object("Black", UPDATE_UI, DRAW_UI);
	//コンポーネント追加
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//オブジェクトの設定
	transBlack->SetPosition(0.0f, 0.0f, 0.0);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	drawBlack->SetColor(0.0f, 0.0f, 0.0f);
	drawBlack->SetAlpha(0.5f);

	//メニューリストに追加
	AddMenu(Black);
	
	//再開
	Object* Resume = new Object(UI_RESUME_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Resume = Resume->AddComponent<CTransform>();
	auto D_Resume = Resume->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Resume->SetPosition(UI_RESUME_POS_X, UI_RESUME_POS_Y);
	D_Resume->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_TEX_NUM));
	D_Resume->SetSize(UI_RESUME_SIZE_X, UI_RESUME_SIZE_Y);
	//追加
	AddMenu(Resume);

	//ステージセレクト
	Object* Select = new Object(UI_STAGE_SELECT_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Select = Select->AddComponent<CTransform>();
	auto D_Select = Select->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Select->SetPosition(UI_STAGE_SELECT_POS_X, UI_STAGE_SELECT_POS_Y);
	D_Select->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_SELECT_TEX_NUM));
	D_Select->SetSize(UI_STAGE_SELECT_SIZE_X, UI_STAGE_SELECT_SIZE_Y);
	//追加
	AddMenu(Select);

	//オプション
	Object* OptionObj = new Object(UI_OPTION_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Option = OptionObj->AddComponent<CTransform>();
	auto D_Option = OptionObj->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Option->SetPosition(UI_OPTION_POS_X, UI_OPTION_POS_Y);
	D_Option->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_TEX_NUM));
	D_Option->SetSize(UI_OPTION_SIZE_X, UI_OPTION_SIZE_Y);
	//追加
	AddMenu(OptionObj);

	//追加されたオブジェクトの初期化をする
	Start();
}
#pragma endregion

#pragma region ---ステージセレクト
/**
* @fn		MenuManager::CreateSelectMenu
* @brief	ステージセレクトメニューの作成
*/
void MenuManager::CreateSelectMenu() {
	//オブジェクトリストの削除
	DeleteMenu();
	//ステージセレクトメニューが選ばれているようにする
	m_nCreateMenu = SELECT_STATE;

	//テクスチャマネージャー
	TextureManager* pTexManager = TextureManager::GetInstance();
	//モデルマネージャー
	ModelManager* pModelManager = ModelManager::GetInstance();

	//必要になってくるオブジェクトを生成する
	//プレイヤー
	Object* Box = new Object("Stand", UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	//設定仮
	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	TransBox->SetPosition(0.0f, 15.0f,-65.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X + 25, DXCHAN_SIZE_Y + 25);
	DrawBox->SetUVsize(XMFLOAT2(-1.0f, 1.0f));
	DrawBox->SetAnimSplit(3, 3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	AddMenu(Box);

	//ステージを表すオブジェクトの配置
	//ステージ1
	//オブジェクトの生成
	Object* Stage1 = new Object("Stage_1",UPDATE_MODEL,DRAW_MODEL);
	//コンポーネントの追加
	auto T_Stage1 = Stage1->AddComponent<CTransform>();
	auto D_Stage1 = Stage1->AddComponent<CDraw3D>();
	//オブジェクトの設定
	D_Stage1->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage1->SetPosition(0.0f,55.0f,-10);
	T_Stage1->SetScale(50.0f,50.0f,50.0f);
	T_Stage1->SetRotate(0.0f,0.0f,0.0f);
	//オブジェクトマネージャーに追加
	AddMenu(Stage1);
	//ステージオブジェクトを管理するリストにも追加しておく
	m_StageObjList.push_back(Stage1);

	//ステージ2
	Object* Stage2 = new Object("Stage_2", UPDATE_MODEL, DRAW_MODEL);
	//コンポーネントの追加
	auto T_Stage2 = Stage2->AddComponent<CTransform>();
	auto D_Stage2 = Stage2->AddComponent<CDraw3D>();
	//オブジェクトの設定
	D_Stage2->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage2->SetPosition(150.0f, 55.0f, -10);
	T_Stage2->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage2->SetRotate(0.0f, 0.0f, 0.0f);
	//オブジェクトマネージャーに追加
	AddMenu(Stage2);
	//ステージオブジェクトを管理するリストにも追加しておく
	m_StageObjList.push_back(Stage2);

	//ステージ3
	Object* Stage3 = new Object("Stage_3", UPDATE_MODEL, DRAW_MODEL);
	//コンポーネントの追加
	auto T_Stage3 = Stage3->AddComponent<CTransform>();
	auto D_Stage3 = Stage3->AddComponent<CDraw3D>();
	//オブジェクトの設定
	D_Stage3->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage3->SetPosition(300.0f, 55.0f, -10);
	T_Stage3->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage3->SetRotate(0.0f, 0.0f, 0.0f);
	//オブジェクトマネージャーに追加
	AddMenu(Stage3);
	//ステージオブジェクトを管理するリストにも追加しておく
	m_StageObjList.push_back(Stage3);

	//UI関係の追加
	Object* Push_Button = new Object(UI_PUSH_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Push = Push_Button->AddComponent<CTransform>();
	auto D_Push = Push_Button->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Push->SetPosition(UI_PUSH_POS_X, UI_PUSH_POS_Y);
	D_Push->SetTexture(TextureManager::GetInstance()->GetTexture(PUSH_TEX_NUM));
	D_Push->SetSize(UI_PUSH_SIZE_X, UI_PUSH_SIZE_Y);

	//オブジェクトマネージャーに追加
	AddMenu(Push_Button);

	//上下に移動する為のリストに追加する
	m_SelectSub.push_back(Push_Button);

	//UI関係の追加
	Object* To_Title = new Object(UI_TITLE_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto T_ToTitle = To_Title->AddComponent<CTransform>();
	auto D_ToTitle = To_Title->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_ToTitle->SetPosition(UI_TITLE_POS_X, UI_TITLE_POS_Y);
	D_ToTitle->SetTexture(TextureManager::GetInstance()->GetTexture(TO_TITLE_TEX_NUM));
	D_ToTitle->SetSize(UI_TITLE_SIZE_X, UI_TITLE_SIZE_Y);

	//オブジェクトマネージャーに追加
	AddMenu(To_Title);

	//上下に移動する為のリストに追加する
	m_SelectSub.push_back(To_Title);

	//追加されたオブジェクトの初期化
	Start();

	//ステージオブジェクトのリストを動かせるイテレーターは先頭を指しておく
	m_itr_Stage = m_StageObjList.begin();
	//上下に移動するためのイテレータを先頭に
	m_itr_SelectSub = m_SelectSub.begin();

}
#pragma endregion

#pragma region ---ゲームオーバーメニューの作成
/**
* @fn		MenuManager::CreateGameOver
* @brief	ゲームオーバーメニューの作成
*/
void MenuManager::CreateGameOverMenu() {
	//オブジェクトリストの削除
	DeleteMenu();
	//ゲームオーバーが選ばれているようにする
	m_nCreateMenu = GAMEOVER_STATE;

	//必要になってくるボタンを追加する
		//仮
	Object* pButton = new Object("Button_Kari", UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//オブジェクトの設定
	transButton->SetPosition(0.0f, 0.0f);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton->SetSize(100.0f, 50.0f);

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
#pragma endregion

#pragma region ---ミスしたときに表示
void MenuManager::CreateMissMenu() {
	//オブジェクトリストの削除
	DeleteMenu();
	//ミスメニューが選ばれているようにする
	m_nCreateMenu = MISS_STATE;

	//テクスチャの追加
	TextureManager::GetInstance()->AddTexture(PATH_TEX_FAID_OUT, FEAD_OUT_NUM);

	//必要になってくるオブジェクトを追加していく
	//暗くするためのテクスチャ
	Object* Black = new Object("Black",UPDATE_UI,DRAW_UI);
	//コンポーネント追加
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//オブジェクトの設定
	transBlack->SetPosition(0.0f,0.0f);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	drawBlack->SetAlpha(0.0f);

	//メニューリストに追加
	AddMenu(Black);

	//ミスの文字
	Object* MissMoji = new Object("MissMoji",UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto transMiss = MissMoji->AddComponent<CTransform>();
	auto drawMiss = MissMoji->AddComponent<CDraw2D>();
	//オブジェクトの設定
	transMiss->SetPosition(0.0f, 400.0f);
	drawMiss->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawMiss->SetSize(100.0f,50.0f);

	//メニューリストに追加
	AddMenu(MissMoji);

	//追加されたオブジェクトの初期化
	Start();
}
#pragma endregion

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
		//メニューの黒色は黒色のままで
		if (menuObject->GetName() == "Black") {
			menuObject->GetComponent<CDraw2D>()->SetColor(0.0f,0.0f,0.0f);
		}
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
			//下げた結果背景の黒だった場合お尻に行かせる
			if ((*m_itr_Menu)->GetName() == "Black") {
				m_itr_Menu = (m_MenuList.end());
				m_itr_Menu--;
			}
		}
	}
	else if(InputManager::Instance()->GetKeyTrigger(DIK_S)){
		//下方向
		//イテレーターが終端にいた場合スタート地点に行かせる
		std::list<Object*>::iterator itr = m_MenuList.end();
		itr--;
		if (m_itr_Menu == itr) {
			m_itr_Menu = m_MenuList.begin();
			//背景の黒だった場合,さらに1つ先に進める
			if ((*m_itr_Menu)->GetName() == "Black") {
				m_itr_Menu++;
			}
		}
		else {
			m_itr_Menu++;
		}
	}

	/*switch (m_nCreateMenu) {
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
	}*/

	//選ばれたボタンの色を変える
	(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(1.0f,0.0f,0.0f);

	//決定ボタンが押された時の処理
	if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
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
		if ((*m_itr_Menu)->GetName() == UI_BEGIN_NAME) {
			//シーン遷移
			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_CONTINUE_NAME) {
			//ステージセレクトへ移動する
			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_END_NAME){
			//ゲーム終了
			PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
		}
		break;
	case PAUSE_STATE:
		if ((*m_itr_Menu)->GetName() == UI_RESUME_NAME) {
			//ゲームを再開する
			//オンオフをひっくり返す
			SceneGame::GetInstance()->m_bPauseMode = false;
			//メニューを破棄する
			DeleteMenu();
			//オブジェクトを全て再開する
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			//カメラを再開する
		
		}
		else if ((*m_itr_Menu)->GetName() == UI_STAGE_SELECT_NAME) {
			//シーン遷移
			//ステージセレクトへ移動する
			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_OPTION_NAME) {
			//今回はゲームを終わる
			//ゲーム終了
			PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
		}
		break;
	case OPTION_STATE:break;
	case GAMECLEAR_STATE:break;
	case GAMEOVER_STATE:break;
	default:break;
	}
}

/**
* @fn
* @brief
* @detail
*/
void MenuManager::StageSelect() {
	//イテレーターで選ばれている奴以外の大きさを変更する
	for (auto&& obj : m_StageObjList) {
		if ((*m_itr_Stage) == obj) {
			//デフォルトの大きさにする
			obj->GetComponent<CTransform>()->SetScale(50.0f, 50.0f, 50.0f);
			continue;
		}
		obj->GetComponent<CTransform>()->SetScale(30.0f,30.0f,30.0f);
	}
	//上下リストで選ばれているものは色変える
	//そのためにまずはデフォルトに変える
	for (auto&& obj : m_SelectSub) {
		obj->GetComponent<CDraw2D>()->SetColor(1.0f,1.0f,1.0f);
	}

	//操作不可能フラグがオフになっている場合操作できる
	if (!m_bRestart) {
		//上下の移動をできるようにする
		if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
			//上方向に移動
			if (m_itr_SelectSub == m_SelectSub.begin()) {
				m_itr_SelectSub = m_SelectSub.end();
				m_itr_SelectSub--;
			}
			else {
				m_itr_SelectSub--;
			}
		}
		else if (InputManager::Instance()->GetKeyTrigger(DIK_S)) {
			//下方向に移動
			std::list<Object*>::iterator itr = m_SelectSub.end();
			itr--;
			if (m_itr_SelectSub == itr) {
				m_itr_SelectSub = m_SelectSub.begin();
			}
			else {
				m_itr_SelectSub++;
			}
		}

		//選ばれているものの色を変える
		(*m_itr_SelectSub)->GetComponent<CDraw2D>()->SetColor(1.0f,0.0f,0.0f);

		//ステージセレクトを右に動かす
		if (InputManager::Instance()->GetKeyTrigger(DIK_D)) {

			//操作不可能フラグを立てる
			m_bRestart = true;
			//作られているオブジェクトよりイテレーターが行かないようにする
			std::list<Object*>::iterator itr = m_StageObjList.end();
			itr--;
			if (m_itr_Stage == itr) {
				m_itr_Stage = itr;
				//端っこだった場合すぐに操作できるようにする
				m_bRestart = false;
			}
			else {
				//選ばれているオブジェクトの変更
				m_itr_Stage++;
				//オブジェクトを左にスライドしていく
				for (auto&& obj : m_StageObjList) {
					obj->GetComponent<CTransform>()->Vel.x -= STAGEOBJ_VEL;
				}
				//プレイヤーのテクスチャを走っているものに変更する
				Object* menuItr = nullptr;
				for (auto&& obj : m_MenuList) {
					if (obj->GetName() == "Stand") {
						menuItr = obj;
					}
				}
				Object* player = menuItr;
				player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
				player->GetComponent<CAnimMesh>()->SetAnimSplit(7,2);
				player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 10, DXCHAN_SIZE_Y + 10);
				player->GetComponent<CAnimMesh>()->SetVertex(true);
			}
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_A)) {
			//操作不可能フラグを立てる
			m_bRestart = true;
			//一番初めの値より下にいかにようにする
			if (m_itr_Stage == m_StageObjList.begin()) {
				m_itr_Stage = m_StageObjList.begin();
				//端っこだった場合すぐに操作できるようにする
				m_bRestart = false;
			}
			else {
				//選ばれているオブジェクトの変更
				m_itr_Stage--;
				//オブジェクトを左にスライドしていく
				for (auto&& obj : m_StageObjList) {
					obj->GetComponent<CTransform>()->Vel.x += STAGEOBJ_VEL;
				}
				//プレイヤーのテクスチャを走っているものに変更する
				Object* menuItr = nullptr;
				for (auto&& obj : m_MenuList) {
					if (obj->GetName() == "Stand") {
						menuItr = obj;
					}
				}
				Object* player = menuItr;
				player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
				player->GetComponent<CAnimMesh>()->SetAnimSplit(7, 2);
				player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 10, DXCHAN_SIZE_Y + 10);
				player->GetComponent<CAnimMesh>()->SetVertex(false);
			}
		}
		//決定ボタンでステージに入れるようにする
		//プッシュAが赤くなっているときのみステージに入る
		if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
			if ((*m_itr_SelectSub)->GetName() == UI_PUSH_NAME) {
				StageIN();			
			}
			else if((*m_itr_SelectSub)->GetName() == UI_TITLE_NAME){
				SceneManager::Instance()->SetScene(SCENE_TITLE);
			}
		}

	}
	else {
		//ある程度移動したら速度を0にして再び移動できるようにする
		m_fMoveObj += STAGEOBJ_VEL;
		if (m_fMoveObj >= MAX_OBJ_MOVE) {
			for (auto&& obj : m_StageObjList) {
				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
			}
			m_bRestart = false;
			m_fMoveObj = 0.0f;
			//プレイヤーのテクスチャを走っているものに変更する
			Object* menuItr = nullptr;
			for (auto&& obj : m_MenuList) {
				if (obj->GetName() == "Stand") {
					menuItr = obj;
				}
			}
			Object* player = menuItr;
			player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
			player->GetComponent<CAnimMesh>()->SetAnimSplit(3, 3);
			player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 25, DXCHAN_SIZE_Y + 25);
			player->GetComponent<CAnimMesh>()->SetVertex(true);
		}
	}
}

/** 
* @fn		MenuManager::StageIN
* @brief	選ばれたステージによって読み込むステージを変える
*/
void MenuManager::StageIN() {
	if ((*m_itr_Stage)->GetName() == "Stage_1") {
		SceneGame::GetInstance()->SetStage(STAGE_1);
		
		int i = 1 + 1;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_2") {
		int i = 1 + 1;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_3") {
		int i = 1 + 1;
	}
	//シーン遷移
	SceneManager::Instance()->SetScene(SCENE_GAME);
}

/**
* @fn		MenuManager::MissMenu
* @brief	ミスしたときに出現するメニュー
*/
void MenuManager::MissMenu() {
	//やりたいこと
	//黒を半透明状態にする
	//上から文字を落とす
	//ステージの初めからやり直す
	for (auto obj:m_MenuList) {
		//黒背景の更新
		if (obj->GetName() == "Black") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();

			m_fBlackAlpha += 0.1f;
			if (m_fBlackAlpha >= 0.5f) {
				m_fBlackAlpha = 0.5f;
			}

			draw->SetAlpha(m_fBlackAlpha);

		}
	
		//文字の更新
		if (obj->GetName() == "MissMoji") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();

			trans->Pos.y -= 20.0f;

			if (trans->Pos.y <= 0.0f) {
				trans->Pos.y = 0.0f;
			}
		}
	}

	//シーン遷移カウントを減らす
	m_nChangeCnt--;

	if (m_nChangeCnt <= 0) {
		//オブジェクトリストの解放
		for (auto&& menuObject : m_MenuList) {
			delete menuObject;
		}
		m_MenuList.clear();
		//フェードアウトして初めからやり直す
		SceneManager::Instance()->SetScene(SCENE_GAME);
	}
}
