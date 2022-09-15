/**�C���N���[�h��**/
#include "TimeComponent.h"
#include "sceneGame.h"
#include "Draw2DComponent.h"
#include <chrono>
#include "PlayerComponent.h"
#include "ObjectManager.h"

/**
* @fn		CTime::CTime
* @brief	�R���X�g���N�^�����񂾂�
*/
CTime::CTime() {
	this->m_eUpdateOrder = COM_UPDATE_1;
	m_nDigits = 0;
	m_bSetTime = false;
}

/**
* @fn		CTime::~CTime
* @brief	�f�X�g���N�^�͍ӂ��Ȃ�
*/
CTime::~CTime() {

}

 /**
 * @fn		CTime::Start
 * @brief	������
 */
void CTime::Start() {
	m_nSixCnt = 0;
	m_nTimeCnt = 0;
}

/**
* @fn		CTime::Update
* @brief	�X�V����
*/
void CTime::Update() {
	m_nSixCnt++;
	//std::chrono;

	//��b�Ɉ��X�V����
	if (m_nSixCnt > 60) {
		if (m_nTimeCnt > (STAGE_TIME + 2)) {
			auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			Player->GetComponent<CPlayer>()->SetPlayerState(MISS_PLAYER);
		}

		if (m_bSetTime) {
			m_nTimeCnt++;
		}

		auto draw = Parent->GetComponent<CDraw2D>();

		//��O����
		float f = SceneGame::GetInstance()->GetTime();
		
		for (int i = 0;i < m_nDigits;i++) {
			f /= 10;
		}
		if (f < 1) {
			if (SceneGame::GetInstance()->GetTime() <= 0) {
				SceneGame::GetInstance()->SetTime();
			}

			draw->SetAnimNum(0);
			return;
		}

		int timer = SceneGame::GetInstance()->GetTime();

		for (int i = 0;i < m_nDigits;i++) {
			timer /= 10;
		}
		draw->SetAnimNum(timer%10);


		m_nSixCnt = 0;

		if (m_bSetTime) {		
			SceneGame::GetInstance()->SetTime();
		}

	}
}

/**
* @fn		CTime::SetDigits
* @brief	�����̐ݒ�
* @param	(int)	�����������ڂ���ݒ肷��
*/
void CTime::SetDigits(int digit) {
	m_nDigits = digit;
}

/**
* @fn		CTime::SetTime
* @brief	�t���O�𗧂Ă��Ⴄ��[��
* @param	(bool)	true�Ȃ�ς��� false�Ȃ�ς��Ȃ�
*/
void CTime::SetTime(bool main) {
	m_bSetTime = main;
}