/**インクルード部**/
#include "TimeComponent.h"
#include "sceneGame.h"
#include "Draw2DComponent.h"
#include <chrono>
#include "PlayerComponent.h"
#include "ObjectManager.h"

/**
* @fn		CTime::CTime
* @brief	コンストラクタすぎんだろ
*/
CTime::CTime() {
	this->m_eUpdateOrder = COM_UPDATE_1;
	m_nDigits = 0;
	m_bSetTime = false;
}

/**
* @fn		CTime::~CTime
* @brief	デストラクタは砕けない
*/
CTime::~CTime() {

}

 /**
 * @fn		CTime::Start
 * @brief	初期化
 */
void CTime::Start() {
	m_nSixCnt = 0;
	m_nTimeCnt = 0;
}

/**
* @fn		CTime::Update
* @brief	更新処理
*/
void CTime::Update() {
	m_nSixCnt++;
	//std::chrono;

	//一秒に一回更新する
	if (m_nSixCnt > 60) {
		if (m_nTimeCnt > (STAGE_TIME + 2)) {
			auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			Player->GetComponent<CPlayer>()->SetPlayerState(MISS_PLAYER);
		}

		if (m_bSetTime) {
			m_nTimeCnt++;
		}

		auto draw = Parent->GetComponent<CDraw2D>();

		//例外処理
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
* @brief	桁数の設定
* @param	(int)	自分が何桁目かを設定する
*/
void CTime::SetDigits(int digit) {
	m_nDigits = digit;
}

/**
* @fn		CTime::SetTime
* @brief	フラグを立てちゃうよーん
* @param	(bool)	trueなら変える falseなら変えない
*/
void CTime::SetTime(bool main) {
	m_bSetTime = main;
}