//�C���N���[�h��
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
* @brief	�R�R�R�R���X�g���N�^
*/
CSeeColl::CSeeColl()
	:m_pTransform(nullptr), m_pColl(nullptr)
	, m_pObject(nullptr) {
	m_eUpdateOrder = COM_UPDATE_DRAW;
}

/**
* @fn		CSeeColl::~CSeeColl
* @brief	����͒N�����ƌ������ƃf�X�g���N�^��
*/
CSeeColl::~CSeeColl() {

}

/**
* @fn		CSeeColl::Start
* @brief	����������
*/
void CSeeColl::Start() {

	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();
	//��
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);


	//�R���|�[�l���g�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pColl = Parent->GetComponent<CCollider>();

	//�����蔻��p�̃{�b�N�X�̍쐬
	Object* CollBox	= new Object("CollBox", UPDATE_DEBUG, DRAW_DEBUG);
	auto CollPos = CollBox->AddComponent<CTransform>();
	auto CollDraw = CollBox->AddComponent<CDrawMesh>();

	//�I�u�W�F�N�g�̐ݒ�
	CollPos->SetPosition(m_pTransform->Pos.x, m_pTransform->Pos.y);
	//�����蔻��̑傫���̎擾
	XMFLOAT3 CollSize = m_pColl->GetColliderSize();
	CollDraw->SetSize(CollSize.x,CollSize.y);
	CollDraw->SetTexture(pTexManager->GetTexture(DEBUG_BLOCK_NUM));
	CollDraw->SetColor(1.0f,1.0f,1.0f);
	
	ObjectManager::GetInstance()->AddObject(CollBox);

	m_pObject = CollBox;
}

/**	
* @fn		CSeeColl::Update
* @brief	�X�V����
*/
void CSeeColl::Update() {
	m_pObject->GetComponent<CTransform>()->Pos = Parent->GetComponent<CTransform>()->Pos;
}

/**
* @fn		CSeeColl:Draw
* @brief	�`�悷�邽�߂̊֐�
*/
void CSeeColl::Draw() {

}