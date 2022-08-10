//インクルード部
#include "SeeCollComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "TextureManager.h"


/**
* @fn		CSeeColl::CSeeColl
* @brief	ココココンストラクタ
*/
CSeeColl::CSeeColl()
	:m_pTransform(nullptr), m_pColl(nullptr)
	, m_pObject(nullptr) {
	m_eUpdateOrder = COM_UPDATE_DRAW;
}

/**
* @fn		CSeeColl::~CSeeColl
* @brief	これは誰が何と言おうとデストラクタだ
*/
CSeeColl::~CSeeColl() {

}

/**
* @fn		CSeeColl::Start
* @brief	初期化する
*/
void CSeeColl::Start() {

	//テクスチャの読込
	TextureManager* pTexManager = TextureManager::GetInstance();
	//仮
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);


	//コンポーネントの取得
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pColl = Parent->GetComponent<CCollider>();

	//当たり判定用のボックスの作成
	Object* CollBox	= new Object("CollBox", UPDATE_DEBUG, DRAW_DEBUG);
	auto CollPos = CollBox->AddComponent<CTransform>();
	auto CollDraw = CollBox->AddComponent<CDrawMesh>();

	//オブジェクトの設定
	CollPos->SetPosition(m_pTransform->Pos.x + m_pColl->GetOffSet().x, m_pTransform->Pos.y + m_pColl->GetOffSet().y);
	//当たり判定の大きさの取得
	XMFLOAT3 CollSize = m_pColl->GetColliderSize();
	CollDraw->SetSize(CollSize.x,CollSize.y);
	CollDraw->SetTexture(pTexManager->GetTexture(DEBUG_BLOCK_NUM));
	CollDraw->SetColor(1.0f,1.0f,1.0f);
	
	ObjectManager::GetInstance()->AddObject(CollBox);

	m_pObject = CollBox;
}

/**	
* @fn		CSeeColl::Update
* @brief	更新処理
*/
void CSeeColl::Update() {
	m_pObject->GetComponent<CTransform>()->Pos.x = Parent->GetComponent<CTransform>()->Pos.x + Parent->GetComponent<CCollider>()->GetOffSet().x;
	m_pObject->GetComponent<CTransform>()->Pos.y = Parent->GetComponent<CTransform>()->Pos.y + Parent->GetComponent<CCollider>()->GetOffSet().y;
}

/**
* @fn		CSeeColl::Draw
* @brief	描画するための関数
*/
void CSeeColl::Draw() {

}

/**
* @fn		CSeeColl::SetCollBox
* @brief	当たり判定描画の大きさの設定し直し
* @param	(XMFLOAT3)	設定した大きさの数値
*/
void CSeeColl::SetCollBox(XMFLOAT3 CollSize,XMFLOAT2 Offset) {
	//大きさの設定
	m_pObject->GetComponent<CDrawMesh>()->SetSize(CollSize.x,CollSize.y);
	//オフセットの設定
	m_pObject->GetComponent<CTransform>()->SetPosition(Offset.x,Offset.y);
	m_pObject->Start();
}

/**
* @fn		CSeeColl::DeleteCollBox
* @brief	当たり判定表示メッシュの削除
*/
void CSeeColl::DeleteCollBox() {
	m_pObject->Delete();
}