/**�C���N���[�h��**/
#include "ScoreComponent.h"
#include "Draw2DComponent.h"
#include "sceneGame.h"

/**
* @fn		CScore::CScore
* @brief	�R���X�g���N�^�͊w�Z����Ȃ�
*/
CScore::CScore() 
	{
	this->m_eUpdateOrder = COM_UPDATE_1;
	m_nDigits = 0;
}

/**
* @fn		CScore::~CScore
* @brief	�����A�f�X�g���N�^����
*/
CScore::~CScore() {

}

/**
* @fn		CScore::Start
* @brief	���������邺
*/
void CScore::Start() {
	
}

/**
* @fn		CScore::Update
* @brief	�X�V����
*/
void CScore::Update() {
	auto draw = Parent->GetComponent<CDraw2D>();
	//��O����
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

	//�������蓖�Ă邽�߂̕ϐ�
	int score = SceneGame::GetInstance()->GetScore();

	for (int i = 0;i < m_nDigits;i++) {
		score /= 10;
	}

	draw->SetAnimNum(score%10);

}

/**
* @fn		CScore::SetDigita
* @brief	�����̃Z�b�g
* @param	(int)	�����ڂ�
*/
void CScore::SetDigits(int digit) {
	m_nDigits = digit;
}