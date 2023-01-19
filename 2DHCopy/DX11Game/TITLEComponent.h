/**
* @file		TITLEComponent.h 
* @brief	�G�̓�����}������R���|�[�l���g
* @author	�u���z�S
* @date		2023/1/13
*/

/**�C���N���[�h�K�[�h**/
#ifndef _TITLE_COMPONENT_H_
#define _TITLE_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CDraw2D;
class CAnimMesh;

/**�N���X��`**/
class CTITLE :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�A�g�k�A��]*/
	CTransform* m_pTransform;
	/** @brief �`���� UI*/
	CDraw2D* m_pDraw2D;
	/** @brief �`���� ���b�V��*/
	CAnimMesh* m_pDrawMesh;
	/** @brief ���ꐯ�����Ă��锻��*/
	bool m_bFallStar;
	/** @brief ���ꐯ�̃X�^�[�g�n�_*/
	XMFLOAT2 m_StartPos;
	/** @brief ���ꐯ�̏I�_�n�_*/
	XMFLOAT2 m_EndPos;

	/** @brief �X�^�[�g�n�_�̃I�u�W�F�N�g�̃|�C���^���擾����*/
	Object* m_StartObj;
	/** @brief �I���n�_�̃I�u�W�F�N�g�̃|�C���^���擾����*/
	Object* m_EndObj;
	/** @brief ���������I�u�W�F�N�g���w���Ă����|�C���^*/
	Object* m_StarObj;
	/** @brief ���b���Ɉ�񗬂ꐯ���o�����߂̃J�E���^�[*/
	int m_nStarCnt;
	/** @brief ��x�����I�u�W�F�N�g�𐶐�����t���O*/
	bool m_bStarCre;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CTITLE();
	/** @brief �f�X�g���N�^*/
	~CTITLE()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw()override;

};

#endif