//インクルード部
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
#include "ObjectManager.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"

//コンストラクタ
SceneTitle::SceneTitle() {

}

//デストラクタ
SceneTitle::~SceneTitle() {

}

//初期化
void SceneTitle::Init() {
	//管理クラスのインスタンスの取得
	m_pObjectManager = ObjectManager::GetInstance();

	//オブジェクトの作成
	//仮
	Object* Box = new Object(PLAYER_NAME,UPDATE_PLAYER,DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CDraw2D>();
	//設定仮
	TransBox->SetPosition(25.0f,25.0f);
	DrawBox->SetSize(100.0f,100.0f);
	DrawBox->SetColor(1.0f,0.0f,0.0f);

	m_pObjectManager->AddObject(Box);
}

//終了処理
void SceneTitle::Uninit() {
	//オブジェクトリストの終了処理
	m_pObjectManager->Uninit();
}

//更新処理
void SceneTitle::Update() {
	//オブジェクトリストの更新
	m_pObjectManager->Update();
}

//描画処理
void SceneTitle::Draw(){
	//オブジェクトリストの描画
	m_pObjectManager->Draw();
}






