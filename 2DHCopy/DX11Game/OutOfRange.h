/**
* @file		OutOfRange.h
* @brief	�J�����͈̔͊O�Ȃ�`����X�L�b�v����
* @author	�u���z�S
* @date		2022/7/31
*/

/**�C���N���[�h�K�[�h**/
#ifndef _OUTOFRANGE_H_
#define _OUTOFRANGE_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;

/**�萔��`**/
//===== �}�N����` =====
#define OUT_RANGE_X	(300.0f)
#define OUT_RANGE_Y	(220.0f)

/**�N���X��`**/
class COutOfRange :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ��̊Ǘ�*/
	CTransform* m_pTransform;
	/** @brief �͈͊O���ǂ����̔���*/
	XMFLOAT2 m_LimitRange;

	//�����o�֐�
	/** @brief ���̃R���|�[�l���g�ȊO�̋@�\�̒�~*/
	void NoFunction();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	COutOfRange();
	/** @brief �f�X�g���N�^*/
	~COutOfRange()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V*/
	void Update() override;
	/** @brief �`��*/
	void Draw();
	/** @brief �Փ˔���̏���*/
	void OnCollisionEnter(Object* pObject)override;

	/** @brief �`�悷��͈͂̐ݒ�*/
	void SetLimitRange(float x,float y);

};

#endif