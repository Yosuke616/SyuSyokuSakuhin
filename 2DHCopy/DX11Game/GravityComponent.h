/**
* @file		GravityComponent.h
* @brief	�d�͂�������R���|�[�l���g
* @author	�u���z�S
* @date		2022/7/24
*/

/**�C���N���[�h�K�[�h**/
#ifndef _GRAVITY_H_
#define _GRAVITY_H_

/**�C���N���[�h**/
#include "mainApp.h"
#include "Component.h"

/**�萔��`**/
/** @brief �d�͂̑傫��*/
#define GRAVITY (0.3f)
#define MAX_VELOCITY (7.5f)

/**�O���錾**/
class CTransform;

/**�N���X��`**/
class CGravity :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W*/
	CTransform* m_pTransform;
	/** @brief �W�����v����Ƃ��ɂ��̃R���|�[�l���g��؂�ׂ̕ϐ�*/
	bool m_bUse;
	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CGravity();
	/** @brief �f�X�g���N�^*/
	~CGravity();
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V*/
	void Update()override;

	/**�Z�b�^�[**/
	/** @brief �X�V���~�߂邩�~�߂Ȃ���*/
	void SetUse(bool use);
};

#endif