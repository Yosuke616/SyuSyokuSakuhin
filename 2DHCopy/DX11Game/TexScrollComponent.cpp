//�C���N���[�h��
#include "TexScrollComponent.h"
#include "Draw2DComponent.h"
#include "Camera.h"

#define RATE_CHASE_SCROLL (0.1f)

//�ÓI�����o�ϐ�
bool CTexScroll::m_bScroll = true;

/**
* @fn		CTexScroll::CTexScroll
* @brief	���͂�R���X�g���N�^�Ƃ͌����Ȃ�
*/
CTexScroll::CTexScroll() 
	:m_pDraw2D(nullptr),
	 m_pPosX(nullptr),m_pPosY(nullptr){
	m_Scroll = XMFLOAT2(0.0f,0.0f);
	m_UVscroll = XMFLOAT2(0.0f,0.0f);
	m_ScrollValue = XMFLOAT2(0.0f,0.0f);
}

/**
* @fn		CTexScroll::~CTexScroll
* @brief	�f�X�g���N�^�����񂾁@���̐l�łȂ��I
*/
CTexScroll::~CTexScroll() {

}

/**
* @fn		CTexScroll::Start()
* @brief	����������
*/
void CTexScroll::Start() {

	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

/**
* @fn		CTexScroll::Update
* @brief	�X�V����
*/
void CTexScroll::Update() {
	//�X�N���[�����Ȃ��ꍇ
	if (!m_bScroll) {
		//���݂�UV�ړ��ʂ�ݒu
		m_pDraw2D->SetScroll(m_UVscroll);
		return;
	}

	//X�����̃X�N���[������ݒ肳��Ă�����
	if (m_pPosX) {
		//���݂̃X�N���[���ʂƖړI�̃X�N���[���ʂ̍��������߂�
		float defferenceX = (*m_pPosX * m_ScrollValue.x) - m_Scroll.x;

		//����̃X�N���[�� = ���܂ł̃X�N���[���� + ���� * ����
		m_Scroll.x = m_Scroll.x + defferenceX * RATE_CHASE_SCROLL;

		//�����_�����Ŏ��o��
		m_UVscroll.x = m_Scroll.x - (int)m_Scroll.x;
	}

	//Y�����̃X�N���[������ݒ肳��Ă�����
	if(m_pPosY){
		m_UVscroll.y = *m_pPosY;
	}

	//�e�N�X�`����UV�ړ��ʂ��Z�b�g
	m_pDraw2D->SetScroll(m_UVscroll);

}

/**
* @fn		CTexScroll::Draw
* @brief	�`��֐��͕`�悵�Ȃ��炵��
*/
void CTexScroll::Draw() {

}

/**
* @fn		CTexScroll::SetAxizX
* @brief	????????
* @param	????????
*/
void CTexScroll::SetAxizX(float *px) {
	m_pPosX = px;
}

/**
* @fn		CTexScroll::SetAxizY
* @brief	????????
* @param	????????
*/
void CTexScroll::SetAxizY(float* py) {
	m_pPosY = py;
}

/**
* @fn		CTexScroll::SetScrollValue
* @brief	????????
* @param    ????????
*/
void CTexScroll::SetScrollValueX(float fx) {
	m_ScrollValue.x = fx;
}

/**
* @fn		CTexScroll::SetScrollValueY
* @brief	????????
* @param	????????
*/
void CTexScroll::SetScrollValueY(float fy) {
	m_ScrollValue.y = fy;
}

/**
* @fn		CTexScroll::Scroll
* @brief	?????????
* @param	?????????
*/
void CTexScroll::Scroll(bool bScroll) {
	m_bScroll = bScroll;
}