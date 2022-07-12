//�C���N���[�h��
#include "Animation.h"
#include "Draw3dComponent.h"
#include "InputManager.h"
#include <time.h>
#include "imgui.h"

//�萔��`
#define DEFAULT_PLAY_SPEED	0.024

/**
* @fn		Animation::Animatio
* @brief	�R���X�g���N�^�ł���Ƃ������Ƃ͎n�߂�Ƃ������Ƃł���
*/
Animation::Animation()
	:m_pDraw3D(nullptr),
	m_dStart(0), m_dEnd(0), m_dCurrentAnimTime(0),
	m_bLoop(0), m_bAnimation(false), m_bIdle(false) {

}

/**
* @fn		Animation::~Animation
* @brief	�Ԏ����Ȃ��@�����̃f�X�g���N�^�̂悤��
*/
Animation::~Animation() {

}

/**
* @fn		Animation::Update
* @brief	�X�V������...�I���˂��I�I�I�I
*/
void Animation::Update() {
#ifdef _DEBUG
	if (m_pDraw3D && m_pDraw3D->Parent->GetName() == PLAYER_NAME) {
		if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
			m_bAnimation = true;
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_V)) {
			m_dCurrentAnimTime = m_dStart[m_nCurrentAnimState];
			//�A�j���[�V�����̔��f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_N))
		{
			m_dCurrentAnimTime -= DEFAULT_PLAY_SPEED;
			// �A�j���[�V�������f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_M))
		{
			m_dCurrentAnimTime += DEFAULT_PLAY_SPEED;
			// �A�j���[�V�������f
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_B))
		{
			this->Stop();
		}
	}

#endif

	//�A�j���[�V�����̍Đ�
	if (m_bAnimation) {
		m_dCurrentAnimTime += m_dSpeed[m_nCurrentAnimState];

		//�A�j���[�V�������f
				// �A�j���[�V�������f
		m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);

		//�A�j���[�V��������������Ƃ�
		if (m_dCurrentAnimTime > m_dEnd[m_nCurrentAnimState]) {
			if (m_bLoop[m_nCurrentAnimState]) {
				//�A�j���[�V�����̎n�܂�
				m_dCurrentAnimTime = m_dStart[m_nCurrentAnimState];
			}
			else {
				if (m_bIdle) {
					//�I��������Ƃ�m�点��
					m_bEnd[m_nCurrentAnimState] = true;
				}
				else {
					m_bAnimation = false;
				}
			}
		}

	}

}

/**
* @fn		Animation::Draw
* @brief	�`�揈��
*/
void Animation::Draw() {
#ifdef _DEBUG
	/*if (m_pDraw3D && m_pDraw3D->Parent->GetName == PLAYER_NAME) {
		using namespace ImGui;
	}*/
#endif
}

/** 
* @fn		Animation::Play
* @brief	�ŏ�����Đ�
* @param	(int)	�A�j���[�V�����̏��
*/
void Animation::Play(int AnimState) {
	//�Đ��t���OON
	m_bAnimation = true;

	//���݂̃A�j���Ǝ��̃A�j�����ꏏ�̏ꍇ�������Ȃ�
	if (m_nCurrentAnimState == AnimState) {
		return;
	}

	//�A�j���ؑ�
	m_nCurrentAnimState = AnimState;

	//�A�j���[�V�������n�܂�̃t���[�����ɐݒ�
	m_dCurrentAnimTime = m_dStart[AnimState];
}

/**
* @fn		Animation::Stop
* @brief	��~����
*/
void Animation::Stop() {
	//�Đ��t���OON
	m_bAnimation = false;
}

/**
*@fn		Animation::SetLoop
*@brief		���[�v�Đ�		
*@param		(int)	�A�j���[�V�����̔ԍ�
*@param		(bool)	���[�v�����邩�ǂ���	
*/
void Animation::SetLoop(int AnimID,bool loop) {
	m_bLoop[AnimID] = loop;
}


/**
* @fn		Animation::SetModel
* @brief	���f���̐ݒ�
* @param	(CDraw3D*)	���f���̃|�C���^
*/
void Animation::SetModel(CDraw3D* model) {
	m_pDraw3D = model;
}

/**
* @fn		Animation::AddAnimation
* @brief	�A�j���[�V������ǉ�����
* @param	(int)		�A�j���[�V�����̔ԍ�
* @param	(double)	�A�j���[�V�����X�^�[�g�ԍ�		
* @param	(double)	�A�j���[�V�����I���̔ԍ�
* @param	(double)	���[�v���n�܂�t���[��
* @param	(bool)		���[�v�����邩�ǂ���
*/
void Animation::AddAnimation(int AnimID,double start,double end,double loopStartFrame,bool loop) {
	m_dStart[AnimID] = start;
	m_dEnd[AnimID] = end;
	m_bLoop[AnimID] = loop;
	m_dSpeed[AnimID] = DEFAULT_PLAY_SPEED;
	m_bEnd[AnimID] = false;
}

/**
* @fn		Animation::AlwaysAnimation
* @brief	��ɍĐ����Ă��邩�ǂ���
* @param	(bool)	��ɃA�j���[�V�����𗬂���
*/
void Animation::AlwaysAnimation(bool always) {
	m_bIdle = always;
}

/**
* @fn		Animation::SetPlaySpeed
* @brief	�A�j���[�V�����̍Đ��X�s�[�h�̐ݒ�
* @param	(int)		�A�j���[�V�����̔ԍ�
* @param	(double)	�A�j���[�V�����̃X�s�[�h
*/
void Animation::SetPlaySpeed(int AnimID,double speed) {
	m_dSpeed[AnimID] = speed;
}

/**
* @fn		Animation::AnimEnd
* @brief	�A�j���[�V�������I��点��
* @param	(int)	�ǂ̃��f�����I��点�邩
* @return	(bool)	�I��点�ꂽ���ǂ���
*/
bool Animation::AnimEnd(int AnimID) {
	if (m_bEnd[AnimID]) {
		//�I���t���O�̃��Z�b�g
		m_bEnd[AnimID] = false;

		return true;
	}
	else {
		return false;
	}
}