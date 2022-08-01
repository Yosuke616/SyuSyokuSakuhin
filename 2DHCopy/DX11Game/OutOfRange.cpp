/**�C���N���[�h��**/
#include "OutOfRange.h"
#include "TransformComponent.h"
#include "Camera.h"

/**
* @fn		COutOfRange::COutOfRange
* @brief	����ȃR���X�g���N�^�ő��v���H
*/
COutOfRange::COutOfRange() 
	:m_pTransform(nullptr){
	//�͈͓��̐ݒ�
	m_LimitRange.x = OUT_RANGE_X;
	m_LimitRange.y = OUT_RANGE_Y;
}

/** 
* @fn		COutOfRange::~COutOfRange
* @brief	��Ԃ����f�X�g���N�^������
*/
COutOfRange::~COutOfRange() {

}

/**
* @fn		CoutOfRange::Start
* @brief	������
*/
void COutOfRange::Start() {
	//�e�I�u�W�F�N�g�̍��W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		COutOfRange::Update
* @brief	�X�V����
* @detail	�J��������ɂ��A���ȏ㗣�ꂽ��I�u�W�F�N�g�̋@�\���~����
*/
void COutOfRange::Update() {
	//��ʂ̒��S���W�̎擾
	XMFLOAT3 Center = CCamera::Get()->GetPos();

	//���̋�������Ă�����X�V���~�߂�
	if (Center.x + m_LimitRange.x < m_pTransform->Pos.x ||		// ��ʊO
		Center.x - m_LimitRange.x > m_pTransform->Pos.x	||		// ��ʊO
		Center.y + m_LimitRange.y < m_pTransform->Pos.y ||		// ��ʊO
		Center.y - m_LimitRange.y > m_pTransform->Pos.y)		// ��ʊO
	{ 
		//�@�\�̒�~
		Parent->StopUpdate();
		Parent->StopDraw();

		//���̃R���|�[�l���g�̍X�V�t���O���I���ɂ���
		this->m_bUpdateFlag = true;
	}
	else {
		//�X�V���ĊJ
		Parent->Use();
	}
}

/**
* @fn		COutOfRange::Draw
* @brief	�`�揈��
*/
void COutOfRange::Draw() {

}

/**
* @fn		COutOfRange::OnCollisionEnter
* @brief	�Փ˔���̏���
* @detail	�����͂Ȃ������ł�
* @param	(Object*)	�Փ˂����I�u�W�F�N�g�̏��
*/
void COutOfRange::OnCollisionEnter(Object* pObject) {

}

/**
* @fn		COutOfRange::NoFunction
* @brief	���̃R���|�[�l���g�ȊO�̏������~����
*/
void COutOfRange::NoFunction() {
	//�@�\�̒�~
	Parent->NoUse();

	//���̃R���|�[�l���g�����X�V����
	this->m_bUpdateFlag = true;
}

/**
* @fn		COutOfRange::SetLimitRange 
* @brief	�͈͂̐ݒ�
* @detail	�ʂŔ͈͂�ݒ肵�����Ƃ��ɕK�v�ɂȂ��Ă���̂ł��傤
* @param	(float)	���͈̔͂̐ݒ�
* @param	(float)	�c�͈̔͂̐ݒ�
*/
void COutOfRange::SetLimitRange(float x, float y) {
	m_LimitRange.x = x;
	m_LimitRange.y = y;
}