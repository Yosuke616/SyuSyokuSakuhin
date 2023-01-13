/**インクルード部**/
#include "TITLEComponent.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "SceneTitle.h"
#include "imgui.h"

#include "ObjInfo.h"

/**
* @fn		CTITLE::CTITLE
* @brief	コンストラクタ。。。お前船降りろ
*/
CTITLE::CTITLE() 
	:m_pTransform(nullptr),m_pDraw2D(nullptr),m_pDrawMesh(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CTITLE::~CTITLE
* @brief	デストラクタ、シンジに乗れ
*/
CTITLE::~CTITLE() {
	
}

/**
* @fn		CTITLE::START
* @brief	開始処理
*/
void CTITLE::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	//プレイヤーキャラクターのみこっちの処理
	if (Parent->GetName() == "Miko") {
		m_pDrawMesh = Parent->GetComponent<CAnimMesh>();
		//右に移動させる
		m_pTransform->Vel.x = 2.0f;
	}
	else {
		m_pDraw2D = Parent->GetComponent<CDraw2D>();
	}

}

/**
* @fn		CTITLE::UPDATE
* @brief	更新処理
* @detail	巫女ちゃんは処理換えるよ
*/
void CTITLE::Update(){
	//処理換えるよ～ん
	if (Parent->GetName() == "Miko") {

		//画面端!に言ったら逆向きに走らせる
		if (m_pTransform->Pos.x >= 450.0f) {
			//テクスチャを反転させるて左向きに力を加える
			m_pDrawMesh->SetVertex(false);
			m_pTransform->Vel.x = -2.0f;
		}
		if (m_pTransform->Pos.x <= -450.0f) {
			//テクスチャを反転させて右向きに力を加える
			m_pDrawMesh->SetVertex(true);
			m_pTransform->Vel.x = 2.0f;
		}

	}	
	else if (Parent->GetName() == TITLE_PURPLE_NAME)  {
		//色をたまに紫に変更する
		if (rand()%10 == 5 || rand() % 10 == 4 || rand() % 10 == 3) {
			m_pDraw2D->SetColor(1.0f,0.0f,1.0f);
		}
		else {
			m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
		}
	}
}

/**
* @fn
* @brief
*/
void CTITLE::Draw() {

}