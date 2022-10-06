/**�C���N���[�h��**/
#include "ItemComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

/**�萔��`**/
/** @brief ���ʂ��㉺���鑬��*/
#define MAGATAMA_SPEED	(0.05f)
 
/**
* @fn		CItem::CItem
* @brief	�R���X�g���N�^�I�����C��
*/
CItem::CItem()
	:m_pTransform(nullptr),m_pDraw(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CItem::~CItem
* @brief	�T��f�X�g���N�^
*/
CItem::~CItem() {

}

/**
* @fn		CItem::Start
* @brief	�J�n����
*/
void CItem::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw3D>();
	m_pCollider  = Parent->GetComponent<CCollider>();
	m_fUpDown = 0.0f;
	m_bUpDown = false;
}

/**
* @fn		CItem::Update
* @brief	�A�C�e���I�u�W�F�N�g�̍X�V
* @detail	�I�����ꂽ�A�C�e���œ��e�͕ω�����
*/
void CItem::Update() {
	//�I�����ꂽ�A�C�e���œ��e��ς���
	switch (m_eItem) {
	case ITEM_KOBAN:
		//��]������(Y����])
		m_pTransform->Rotate.y += 5.0f;
		break;
	case ITEM_MAGA:
		//��]������(Z����])
		m_pTransform->Rotate.z += 1.0f;

		//�㉺�Ɉړ�������
		if (m_bUpDown) {
			//��Ɉړ�������
			m_pTransform->Vel.y = MAGATAMA_SPEED;
			m_fUpDown += MAGATAMA_SPEED;

			//������x�オ�����牺�ɉ�����
			if (m_fUpDown > 2.0f) {
				m_bUpDown = false;
			}
		}
		else {
			//���Ɉړ�������
			m_pTransform->Vel.y = -MAGATAMA_SPEED;
			m_fUpDown -= MAGATAMA_SPEED;

			//������x�����������ɏグ��
			if (m_fUpDown < -2.0f) {
				m_bUpDown = true;
			}
		}

		break;
	case ITEM_OHUDA:break;
	}
}

/** 
* @fn		CItem::Draw
* @brief	�A�C�e���I�u�W�F�N�g�̕`��
*/
void CItem::Draw() {
	
}

/**
* @fn		CItem::OnCollisionEnter
* @brief	�����蔻��̏���
* @param	(Object*)	��������������
*/
void CItem::OnCollisionEnter(Object* obj) {

}

/**
* @fn		CItem::SetItem
* @brief	�A�C�e���̎�ނ����߂�
* @param	(ITEM_TYPE)	���A�C�e��
*/
void CItem::SetItem(ITEM_TYPE type) {
	m_eItem = type;
}

/** 
* @fn		CItem::GetItem
* @brief	�ݒ肳��Ă���A�C�e���̎�ނ��擾����
* @return	(ITEM_TYPE)	���̃I�u�W�F�N�g�̃A�C�e���^�C�v 
*/
ITEM_TYPE CItem::GetItem() {
	return m_eItem;
}