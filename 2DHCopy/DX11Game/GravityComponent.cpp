/**�C���N���[�h��**/
#include "GravityComponent.h"
#include "TransformComponent.h"

/**�萔��`**/

/**
* @fn		CGravity::CGravity
* @brief	�R�A�R���X�g���N�^�I�I�����������̂��I�I�I
*/
CGravity::CGravity()
	:m_pTransform(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CGravity::~CGravity
* @brief	�f�X�g���N�^�A�T���i����
*/
CGravity::~CGravity() {

}

/**
* @fn		CGravity::Start
* @brief	����������
*/
void CGravity::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		CGravity::Update
* @brief	�X�V����
*/
void CGravity::Update() {
	m_pTransform->Vel.y -= GRAVITY;
	if (m_pTransform->Vel.y < MAX_VELOCITY) {
		m_pTransform->Vel.y = -MAX_VELOCITY;
	}
}

/**
* @fn		CGravity::SetUse
* @brief	�X�V�����́E�E�E�~�܂�˂��I�I(�~�܂�)
* @param	(bool)	�X�V���~�߂���A�ĊJ�����肷��֐�
*/
void CGravity::SetUse(bool use) {
	Parent->GetComponent<CGravity>()->m_bUpdateFlag = use;
}
