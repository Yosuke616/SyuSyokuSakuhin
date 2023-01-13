/**インクルード部**/
#include "KADAIComponent.h"
#include "ObjectManager.h"

#include "TransformComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "ColliderComponent.h"
#include "SceneTitle.h"
#include "SeeCollComponent.h"
#include "imgui.h"

/**定位数定義**/

/**
* @fn		CKADAI::CKADAI
* @brief	こんなコンストラクタに意味なんてない
* @detail	色々初期化しよーね
*/
CKADAI::CKADAI()
	:m_pTrans(nullptr),m_pDraw(nullptr),m_pColl(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CKADAI::~CKADAI
* @brief	デストラクタを削除する
* @detail	終わる
*/
CKADAI::~CKADAI() {
	
}

/**
* @fn		CKADAI::Start
* @brief	初期化して役目でしょ
*/
void CKADAI::Start() {
	m_pTrans = Parent->GetComponent<CTransform>();
	m_pDraw = Parent->GetComponent<CAnimMesh>();
	m_pColl = Parent->GetComponent<CCollider>();


	//ランダムに初速を決める
	m_pTrans->SetVelocity(rand()%3,rand()%3,0.0f);

}

/**
* @fn		CKADAI::Update
* @brief	更新処理
* @detail   反射させて動かすよーーん
*/
void CKADAI::Update() {
	//画面外に行こうとしたら反射させる
	if (m_pTrans->Pos.x >= (SCREEN_WIDTH/4)) {
		m_pTrans->Vel.x *= -1;
	}
	if (m_pTrans->Pos.x <= -(SCREEN_WIDTH/4)) {
		m_pTrans->Vel.x *= -1;
	}
	if (m_pTrans->Pos.y >= (SCREEN_HEIGHT/4)) {
		m_pTrans->Vel.y *= -1;
	}
	if (m_pTrans->Pos.y <= -(SCREEN_HEIGHT/4)) {
		m_pTrans->Vel.y *= -1;
	}
}

/**
* @fn		CKADAI::Draw
* @brief	描画させてお願い
*/
void CKADAI::Draw() {

#ifdef _DEBUG

	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("BALL");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pTrans->Pos.x, m_pTrans->Pos.y, m_pTrans->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pTrans->Vel.x, m_pTrans->Vel.y);
	End();
#endif
}

/**
* @fn		CKADAI::OnCollisionEnter
* @brief	オブジェクト同士ぶつかったら反射させるよ～ん
* @param	(Object*)	相手方のオブジェクトとぶつかったら反射する	
*/
void CKADAI::OnCollisionEnter(Object* pObject){
	if (pObject->GetName() == "TITLE_BALL") {
		
	}
}
