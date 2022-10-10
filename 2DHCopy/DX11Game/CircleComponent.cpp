/**�C���N���[�h��**/
#include "CircleComponent.h"
#include "ObjectManager.h"
#include "TransformComponent.h"
#include "AnimMeshComponent.h"
#include "sceneGame.h"
#include "imgui.h"

/**
* @fn		CCircle::CCircle
* @brief	�R���X�g���N�^�t�����h(�R�t��)
*/
CCircle::CCircle() 
:m_pTransform(nullptr),m_pDraw(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CCircle::~CCircle
* @brief	���l�̓f�X�g���N�^
*/
CCircle::~CCircle() {
	
}

/**
* @fn		CCircle::Start
* @brief	����������
*/
void CCircle::Start() {
	m_pTransform = Parent->GetComponent <CTransform>();
	m_pDraw		 = Parent->GetComponent<CAnimMesh>();
}

/**
* @fn		CCircle::Update
* @brief	�X�V����
*/
void CCircle::Update() {
	//�|�W�V�������v���C���[�̍��W�Ɠ����ꏊ�Ɏ����Ă���
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	//���W���ړ�
	m_pTransform->Pos = obj->GetComponent<CTransform>()->Pos;
}

/**
* @fn		CCircle::Draw
* @brief	�`�揈��
*/
void CCircle::Draw() {

}

/**
* @fn		CCircle::OnCollisionEnter
* @brief	�����蔻��̏���
* @param	(Object*)	����������̏�񂪓������|�C���^
*/
void CCircle::OnCollisionEnter(Object* pObject) {

}
