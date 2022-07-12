/**
* @file		TexScrollComponent.h
* @brief	�e�N�X�`���̃X�N���[��
* @author	�u���z�S
* @date		2022/6/30
*/

//�C���N���[�h�K�[�h
#ifndef _TEX_SCROLL_H_
#define _TEX_SCROLL_H_

//�C���N���[�h
#include "Component.h"

//�O���錾
class CDraw2D;

//�N���X��`
class CTexScroll : public Component {
private:
	//�����o�ϐ�
	/** @brief �`��̐ݒ�*/
	CDraw2D*	m_pDraw2D;
	/** @brief �X�N���[���̊�Ƃ���X���W*/
	float*		m_pPosX;
	/** @brief �X�N���[���̊�Ƃ���Y���W*/
	float*		m_pPosY;

	/** @brief �X�N���[���̒l*/
	XMFLOAT2 m_Scroll;
	/** @brief �X�N���[���̒l(0~1)*/
	XMFLOAT2 m_UVscroll;
	/** @brief �X�N���[���̗�*/
	XMFLOAT2 m_ScrollValue;
	/** @brief �X�N���[�����邩�t���O*/
	static bool m_bScroll;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CTexScroll();
	/** @brief �f�X�g���N�^*/
	~CTexScroll() override;
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update()override;
	/** @brief �`�揈��*/
	void Draw()override;

	//�Z�b�^�[
	void SetAxizX(float* px);
	void SetAxizY(float* py);
	void SetScrollValueX(float fx);
	void SetScrollValueY(float fy);
	static void Scroll(bool bScroll);
};

#endif