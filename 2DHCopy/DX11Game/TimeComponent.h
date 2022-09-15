/**
* @file		TimeComponent.h
* @brief	���Ԃ��ǂ�ǂ񌸂炵�Ă���
* @author	�u���z�S
* @data		2022/9/13
*/

/**�C���N���[�h�K�[�h**/
#ifndef _TIME_COMPONENT_H_
#define _TIME_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�N���X��`**/
class CTime : public Component {
private:
	//�����o�ϐ�
	/** @brief �����ڂ���ۑ�����*/
	int m_nDigits;

	/** @brief 60�b�Ɉ��X�V���邽�߂̕ϐ�*/
	int m_nSixCnt;

	/** @brief ���������炷���ǂ����̃t���O�������Ă���֐�*/
	bool m_bSetTime;

	/** @brief �������Ă��鎞��*/
	int m_nTimeCnt;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CTime();
	/** @brief �f�X�g���N�^*/
	~CTime()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V*/
	void Update() override;
	/** @brief �����������ڂ���ݒ肷��֐�*/
	void SetDigits(int digit);
	/** @brief �^�C����ς��邽�߂̕ϐ����Z�b�g����*/
	void SetTime(bool main);
};

#endif