//インクルード部
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
#include "ObjInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "imgui.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "Draw3dComponent.h"
#include "AnimMeshComponent.h"
#include "DrawMeshComponent.h"
#include "ColliderComponent.h"
#include "TITLEComponent.h"
#include "Sound.h"
#include "SoundData.h"

#include "Load.h"

/**定数定義**/

//コンストラクタ
SceneTitle::SceneTitle() {

}

//デストラクタ
SceneTitle::~SceneTitle() {

}

//初期化
void SceneTitle::Init() {
	//ロード画面のスレッドを作る
	Load::Begin();
	
	//管理クラスのインスタンスの取得
	m_pObjectManager = ObjectManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	TextureManager* pTexManager = TextureManager::GetInstance();
	
	//背景のテクスチャ
	pTexManager->AddTexture(PATH_TEX_TITLE_BACK_GROUND, TITLE_BACK_GROUND_NUM);
	//UI関係のテクスチャも追加する
	pTexManager->AddTexture(PATH_TEX_TITLE_BUTTON, TITLE_BUTTON_NUM);
	pTexManager->AddTexture(PATH_TEX_BEGIN, BEGIN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CONTINUE, CONTINUE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_END, END_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BGM, BGM_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SE, SE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BRIGHTNESS, BRIGHTNESS_TEX_NUM);
	
	//矢印
	pTexManager->AddTexture(PATH_ARROW, ARROW_NUM);
	pTexManager->AddTexture(PATH_TEX_KARIYOZORA,KARIYOZORA_TEX_NUM);
	
	//背景用オブジェクト
	ModelManager::GetInstance()->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);
	ModelManager::GetInstance()->AddModel(PATH_RARD_BLOCK, RARD_BLOCK_NUM);

	//タイトル用テクスチャの設定
	pTexManager->AddTexture(PATH_TEX_EIGHT_TITLE, EIGHT_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOUD_TITLE,CLOUD_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_NO_TITLE,NO_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_MI_TITLE,MI_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_GIRL_TEX,GIRL_TITLE_TEX_NUM);

#pragma region ---背景
	//オブジェクトの生成
	Object* objBG = new Object("BG",UPDATE_BG,DRAW_BG);
	//コンポーネントの追加
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CDraw2D>();
	//オブジェクトの設定
	Draw_BG->SetTexture(pTexManager->GetTexture(KARIYOZORA_TEX_NUM));
	Draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//リストの追加
	m_pObjectManager->AddObject(objBG);

#pragma endregion

	

#pragma region ---地面表面
	for (int i = 0; i < 16;i++) {
		//オブジェクトの生成
		Object* objGround = new Object("Gruond",UPDATE_MODEL,DRAW_MODEL);
		//コンポーネントの追加
		auto Groundtrans = objGround->AddComponent<CTransform>();
		auto GroundDraw = objGround->AddComponent<CDraw3D>();
		//オブジェクトの設定
		GroundDraw->SetModel(ModelManager::GetInstance()->GetModel(MINT_GREEN_BLOCK_NUM));
		Groundtrans->SetPosition(-300.0f+(i* 40.0f), -30.0f, 150.0f);
		Groundtrans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(objGround);
	}
#pragma endregion

#pragma region ---地面中身
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 16;j++) {
			//オブジェクトの生成
			Object* objRard = new Object("GroundRard",UPDATE_MODEL,DRAW_MODEL);
			//コンポーネントの追加
			auto RardTrans = objRard->AddComponent<CTransform>();
			auto RardDraw = objRard->AddComponent<CDraw3D>();
			//オブジェクトの設定
			RardDraw->SetModel(ModelManager::GetInstance()->GetModel(RARD_BLOCK_NUM));
			RardTrans->SetPosition(-300.0f + (j * 40.0f),-70.0f + (40.0f * -(i)), 150.0f);
			RardTrans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
			//オブジェクトマネージャー
			ObjectManager::GetInstance()->AddObject(objRard);
		}
	}	
#pragma endregion

#pragma region ---巫女ちゃん
	//オブジェクトの生成
	Object* Medium = new Object("Miko",UPDATE_PLAYER,DRAW_PLAYER);
	//コンポーネントの追加
	auto TransMedium = Medium->AddComponent<CTransform>();
	auto DrawMedium = Medium->AddComponent<CAnimMesh>();
	Medium->AddComponent<CTITLE>();
	//オブジェクトの設定
	DrawMedium->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
	TransMedium->SetPosition(-350.0f,35.0f,150.0f);
	DrawMedium->SetAnimSplit(7,2);
	DrawMedium->SetSwapFrame(3);
	DrawMedium->SetLoop(true);
	//オブジェクトの追加
	ObjectManager::GetInstance()->AddObject(Medium);
#pragma endregion 

#pragma region ---タイトル
	//八
	//オブジェクトの生成
	Object* EIGHT = new Object(TITLE_PURPLE_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto TransEight = EIGHT->AddComponent<CTransform>();
	auto DrawEight = EIGHT->AddComponent<CDraw2D>();
	EIGHT->AddComponent<CTITLE>();
	//オブジェクトの設定
	DrawEight->SetTexture(pTexManager->GetTexture(EIGHT_TITLE_TEX_NUM));
	DrawEight->SetSize(TITLE_WHITE_SIZE_X,TITLE_WHITE_SIZE_Y);
	TransEight->SetPosition(-500.0f,225.0f,0.0f);
	//オブジェクトの追加
	m_pObjectManager->AddObject(EIGHT);

	//雲
	//オブジェクト
	Object* CLOUD = new Object(TITLE_PURPLE_NAME,UPDATE_UI,DRAW_UI);
	//コンポネントの追加
	auto TransCloud = CLOUD->AddComponent<CTransform>();
	auto DrawCloud = CLOUD->AddComponent<CDraw2D>();
	CLOUD->AddComponent<CTITLE>();
	//オブジェクトの設定　　
	DrawCloud->SetTexture(pTexManager->GetTexture(CLOUD_TITLE_TEX_NUM));
	DrawCloud->SetSize(TITLE_WHITE_SIZE_X,TITLE_WHITE_SIZE_Y);
	TransCloud->SetPosition(-370.0f,225.0f,0.0f);
	//オブジェクトの追加
	m_pObjectManager->AddObject(CLOUD);

	//ノ
	//オブジェクトの生成
	Object* NO_TITLE = new Object(TITLE_PURPLE_NAME, UPDATE_UI, DRAW_UI);
	//コンポネントの追加
	auto TransNO = NO_TITLE->AddComponent<CTransform>();
	auto DrawNO = NO_TITLE->AddComponent<CDraw2D>();
	NO_TITLE->AddComponent<CTITLE>();
	//オブジェクトの設定　　
	DrawNO->SetTexture(pTexManager->GetTexture(NO_TITLE_TEX_NUM));
	DrawNO->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransNO->SetPosition(-275.0f, 175.0f, 0.0f);
	//オブジェクトの追加
	m_pObjectManager->AddObject(NO_TITLE);

	//巫
	//オブジェクトの生成
	Object* MI_TITLE = new Object(TITLE_WHITE_NAME, UPDATE_UI, DRAW_UI);
	//コンポネントの追加
	auto TransMI = MI_TITLE->AddComponent<CTransform>();
	auto DrawMI = MI_TITLE->AddComponent<CDraw2D>();
	//オブジェクトの設定　　
	DrawMI->SetTexture(pTexManager->GetTexture(MI_TITLE_TEX_NUM));
	DrawMI->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransMI->SetPosition(-150.0f, 225.0f, 0.0f);
	//オブジェクトの追加
	m_pObjectManager->AddObject(MI_TITLE);

	//女
	//オブジェクトの生成
	Object* GIRL_TITLE = new Object(TITLE_WHITE_NAME, UPDATE_UI, DRAW_UI);
	//コンポネントの追加
	auto TransGirl = GIRL_TITLE->AddComponent<CTransform>();
	auto DrawGirl = GIRL_TITLE->AddComponent<CDraw2D>();
	//オブジェクトの設定　　
	DrawGirl->SetTexture(pTexManager->GetTexture(GIRL_TITLE_TEX_NUM));
	DrawGirl->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransGirl->SetPosition(-20.0f, 225.0f, 0.0f);
	//オブジェクトの追加
	m_pObjectManager->AddObject(GIRL_TITLE);

#pragma endregion
	
#pragma region ---開始地点と終了地点
	//オブジェクトの生成(終了地点)
	Object* objEnd_TITLE = new Object("TITLE_End_Pos", UPDATE_DEBUG, DRAW_DEBUG);
	//コンポーネントの追加
	auto objEnd_Trans = objEnd_TITLE->AddComponent<CTransform>();
	auto objEnd_Draw = objEnd_TITLE->AddComponent<CAnimMesh>();
	objEnd_TITLE->AddComponent<CTITLE>();
	//オブジェクトの設定
	objEnd_Draw->SetColor(1.0f, 1.0f, 0.0f);
	objEnd_Trans->SetPosition(0.0f, -100.0f);
	objEnd_Draw->SetSize(10.0f, 10.0f);
	objEnd_Draw->SetLoop(true);
	//オブジェクトマネージャーに追加
	ObjectManager::GetInstance()->AddObject(objEnd_TITLE);

	//オブジェクトの生成(開始地点)
	Object* objStart = new Object("TITLE_Start_Pos",UPDATE_DEBUG,DRAW_DEBUG);
	//コンポーネントの追加
	auto objStart_Trans = objStart->AddComponent<CTransform>();
	auto objStart_Draw = objStart->AddComponent<CAnimMesh>();
	objStart->AddComponent<CTITLE>();
	//オブジェクトの設定
	objStart_Draw->SetColor(1.0f,1.0f,0.0f);
	objStart_Trans->SetPosition(0.0f,250.0f);
	objStart_Draw->SetSize(10.0f,10.0f);
	objStart_Draw->SetLoop(true);
	//オブジェクトマネージャーに追加
	ObjectManager::GetInstance()->AddObject(objStart);
#pragma endregion

	//メニューの作成
	m_pMenuManager->CreateTitleMenu();
	
	CSound::Play(TITLE_BGM);

	//ロード終了処理
	Load::End();
}

//終了処理
void SceneTitle::Uninit() {
	//音楽を止める
	CSound::Stop(TITLE_BGM);

	//メニューの終了
	m_pMenuManager->Destroy();

	//オブジェクトリストの終了処理
	m_pObjectManager->Uninit();
}

//更新処理
void SceneTitle::Update() {
	//オブジェクトリストの更新
	m_pObjectManager->Update();

	//メニュー画面の更新
	m_pMenuManager->Update();
}

//描画処理
void SceneTitle::Draw(){
	//オブジェクトリストの描画
	m_pObjectManager->Draw();

	//メニュー画面の更新
	m_pMenuManager->Draw();
}
