/**
* @fn		Draw2DComponent.h
* @brief	2D�`�������R���|�[�l���g
* @author	�u���z�S
* @date		2022/6/22
*/

//�C���N���[�h�K�[�h
#ifndef _DRAW_2D_COMPONENT_H_
#define _DRAW_2D_COMPONENT_H_

//�C���N���[�h��
#include "mainApp.h"
#include "Component.h"

//�O���錾
class CTransform;

//�N���X��`
class CDraw2D : public Component {
private:
	//�����o�ϐ�

	//�����o�֐�

protected:
	//�����o�ϐ�
	/** @brief ���W�Ȃǂׂ̈̕ϐ�*/
	CTransform* m_pTransform;
	/** @brief �e�N�X�`���̏��ׂ̈̕ϐ�*/
	ID3D11ShaderResourceView* m_pTexture;
	/** @brief �l�p�`�|���S���̑傫��*/
	XMFLOAT2 m_fPolygonSize;
	/** @brief �e�N�X�`���̊��蓖�Ĕ͈�*/
	XMFLOAT2 m_fTexFrameSize;
	/** @brief ���_�J���[*/
	XMFLOAT4 m_fVertevColor;
	/** @brief �p�x*/
	float m_fAngle;
	/** @brief �X�N���[��*/
	XMFLOAT2 m_Scroll;
	/** @brief �u�����h�̎��*/
	EBlendState m_BlendState;
	/** @brief �A�j���[�V�����ԍ�*/
	int m_nAnimNum;
	/** @brief ���̃A�j���[�V�����̕�����*/
	int m_nAnimSplitX;
	/** @brief �c�̃A�j���[�V�����̕�����*/
	int m_nAnimSplitY;
	
	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CDraw2D();
	/** @brief �f�X�g���N�^*/
	~CDraw2D() override;
	/** @brief ����������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
	/** @brief �|���S���̃T�C�Y�ݒ�*/
	void SetSize(float,float);
	/** @brief UV�T�C�Y�ݒ�*/
	void SetUVsize(float,float);
	/** @brief �A�j���[�V�����������̐ݒ�*/
	void SetAnimSplit(int ,int);
	/** @brief �A�j���[�V�����ԍ��̐ݒ�*/
	void SetAnimNum(int);
	/** @brief �e�N�X�`���̐ݒ�*/
	void SetTexture(ID3D11ShaderResourceView*);
	/** @brief �J���[�ݒ�*/
	void SetColor(float r,float g,float b);
	/** @brief �����x�̐ݒ�*/
	void SetAlpha(float alpha);
	/** @brief �p�x�̐ݒ�*/
	void SetAngle(float angle);
	/** @brief �X�N���[���ʂ̐ݒ�*/
	void SetScroll(XMFLOAT2 scroll);
	/** @brief �u�����h�X�e�[�g�̐ݒ�*/
	void SetBlend(EBlendState state);
	/** @brief UV��U�̐ݒ�*/
	void SetU(float u);
	/** @brief UV��V�̐ݒ�*/
	void SetV(float v);

	/** @brief �������̕������𓾂�*/
	int GetAnimSplitX();
};

#endif