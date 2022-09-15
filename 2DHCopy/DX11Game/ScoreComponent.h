/**
* @file		ScoreComponent.h
* @brief	�X�R�A���J�E���g���A�e�N�X�`���̊��蓖�Ă�ύX���Ă���
* @detail	�X�R�A�𑝂₵���肷��̂͂��̃R���|�[�l���g�ł����Ǝv�����Ǘ��̓}�l�[�W���ɂ�����ׂ�����
* @author	�u���z�S			
* @data		2022/9/10
*/

/**�C���N���[�h�K�[�h**/
#ifndef _SCORE_COMPONENT_H_
#define _SCORE_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�萔��`**/
#define MAX_SCORE (5)

/**�N���X��`**/
/**
* @brief	�X�R�A�𑝂₵������������R���|�[�l���g
* @detail	�}�l�[�W���[�ɒl��n�����肷��
*/
class CScore : public Component {
private:
	//�����o�ϐ�
	/** @brief �����������ڂ���ۑ�����ϐ�*/
	int m_nDigits;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CScore();
	/** @brief �f�X�g���N�^*/
	~CScore() override;
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �����������ڂ��ݒ肷��֐�*/
	void SetDigits(int digit);
};

#endif