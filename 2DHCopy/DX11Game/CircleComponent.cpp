/**インクルード部**/
#include "CircleComponent.h"
#include "ObjectManager.h"
#include "TransformComponent.h"
#include "AnimMeshComponent.h"
#include "sceneGame.h"
#include "imgui.h"

/**
* @fn		CCircle::CCircle
* @brief	コンストラクタフレンド(コフレ)
*/
CCircle::CCircle() 
:m_pTransform(nullptr),m_pDraw(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CCircle::~CCircle
* @brief	恋人はデストラクタ
*/
CCircle::~CCircle() {
	
}

/**
* @fn		CCircle::Start
* @brief	初期化処理
*/
void CCircle::Start() {
	m_pTransform = Parent->GetComponent <CTransform>();
	m_pDraw		 = Parent->GetComponent<CAnimMesh>();
}

/**
* @fn		CCircle::Update
* @brief	更新処理
*/
void CCircle::Update() {
	//ポジションをプレイヤーの座標と同じ場所に持っていく
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	//座標を移動
	m_pTransform->Pos = obj->GetComponent<CTransform>()->Pos;
}

/**
* @fn		CCircle::Draw
* @brief	描画処理
*/
void CCircle::Draw() {

}

/**
* @fn		CCircle::OnCollisionEnter
* @brief	当たり判定の処理
* @param	(Object*)	当たった先の情報が入ったポインタ
*/
void CCircle::OnCollisionEnter(Object* pObject) {

}
