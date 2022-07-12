/**
* @file		Animation.h
* @brief	�A�j���[�V�������Ǘ�����N���X
* @author	�u���z�S
* @date		2022/7/8
*/

//�C���N���[�h�K�[�h
#ifndef _ANIMATION_H_
#define  _ANIMATION_H_

//�C���N���[�h��
#include "Component.h"
#include <unordered_map>

//�O���錾
class CDraw3D;

//�N���X��`
class Animation {
private:
	//�����o�ϐ�
	/** @brief �A�j���[�V�����J�n�t���[��*/
	std::unordered_map<int, double> m_dStart;
	/** @brief �A�j���[�V�����I���t���[��*/
	std::unordered_map<int, double> m_dEnd;
	/** @brief �Đ����x*/
	std::unordered_map<int, double> m_dSpeed;
	/** @brief ���[�v�Đ�*/
	std::unordered_map<int, bool> m_bLoop;
	/** @brief �A�j���[�V�����̗D��x*/
	std::unordered_map<int, int> m_nOrder;
	/** @brief Play�֐����Ă΂�Ă��邩*/
	std::unordered_map<int, bool> m_bEnd;
	
	/** @brief ���݂̃A�j���[�V�����̎���*/
	double m_dCurrentAnimTime;
	/** @brief ���݂̃A�j���[�V�����̏��*/
	int m_nCurrentAnimState;
	/** @brief ture ���[�v���I����0�Ԗڂ̃A�j���[�V���� false �A�j���[�V�������I���ΏI��*/
	bool m_bIdle;
	/** @brief �Đ��t���O*/
	bool m_bAnimation;
	/** @���f��*/
	CDraw3D* m_pDraw3D;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	Animation();
	/** @brief �f�X�g���N�^*/
	~Animation();
	/** @brief �X�V����*/
	void Update();
	/** @brief �`�揈��*/
	void Draw();

	/** *brief ���f���̃Z�b�g*/
	void SetModel(CDraw3D* model);

	/** @brief �Đ�*/
	void Play(int AnimState);
	/** @brief��~*/
	void Stop();

	/** @brief �A�j���[�V�����̒ǉ�*/
	void AddAnimation(int AnimID,double start,double end,double loopStartFrame,bool loop);

	/** @brief ���[�v�Đ��̐ݒ�*/
	void SetLoop(int AnimID,bool loop);
	/** @brief �Đ����x�̐ݒ�*/
	void SetPlaySpeed(int AnimID,double speed);
	/** @brief �A�j���[�V�������I��������̃t���O*/
	bool AnimEnd(int AnimID);
	/** @��ɏ펞*/
	void AlwaysAnimation(bool always);
};

#endif