/**インクルード部**/
#include "ScoreComponent.h"
#include "Draw2DComponent.h"
#include "sceneGame.h"

/**
* @fn		CScore::CScore
* @brief	コンストラクタは学校来るなよ
*/
CScore::CScore() 
	{
	this->m_eUpdateOrder = COM_UPDATE_1;
	m_nDigits = 0;
}

/**
* @fn		CScore::~CScore
* @brief	くっ、デストラクタしろ
*/
CScore::~CScore() {

}

/**
* @fn		CScore::Start
* @brief	初期化するぜ
*/
void CScore::Start() {
	
}

/**
* @fn		CScore::Update
* @brief	更新処理
*/
void CScore::Update() {
	auto draw = Parent->GetComponent<CDraw2D>();
	//例外処理
	float f = SceneGame::GetInstance()->GetScore();

	if (f > 99999) {
		draw->SetAnimNum(10);
	}

	for (int i = 0;i < m_nDigits;i++) {
		f /= 10;
	}

	if (f < 1) {
		draw->SetAnimNum(0);
		return;
	}

	//数を割り当てるための変数
	int score = SceneGame::GetInstance()->GetScore();

	for (int i = 0;i < m_nDigits;i++) {
		score /= 10;
	}

	draw->SetAnimNum(score%10);

}

/**
* @fn		CScore::SetDigita
* @brief	桁数のセット
* @param	(int)	何桁目か
*/
void CScore::SetDigits(int digit) {
	m_nDigits = digit;
}