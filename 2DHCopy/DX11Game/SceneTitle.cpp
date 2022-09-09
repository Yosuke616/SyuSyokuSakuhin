//インクルード部
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
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

#pragma region ---背景
	//オブジェクトの生成
	Object* objBG = new Object("BG",UPDATE_BG,DRAW_BG);
	//コンポーネントの追加
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CDraw2D>();
	//オブジェクトの設定
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//リストの追加
	m_pObjectManager->AddObject(objBG);

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
