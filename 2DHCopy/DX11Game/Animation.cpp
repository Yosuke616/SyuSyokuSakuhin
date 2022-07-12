//インクルード部
#include "Animation.h"
#include "Draw3dComponent.h"
#include "InputManager.h"
#include <time.h>
#include "imgui.h"

//定数定義
#define DEFAULT_PLAY_SPEED	0.024

/**
* @fn		Animation::Animatio
* @brief	コンストラクタであるということは始めるということである
*/
Animation::Animation()
	:m_pDraw3D(nullptr),
	m_dStart(0), m_dEnd(0), m_dCurrentAnimTime(0),
	m_bLoop(0), m_bAnimation(false), m_bIdle(false) {

}

/**
* @fn		Animation::~Animation
* @brief	返事がない　ただのデストラクタのようだ
*/
Animation::~Animation() {

}

/**
* @fn		Animation::Update
* @brief	更新処理は...終わらねぇ！！！！
*/
void Animation::Update() {
#ifdef _DEBUG
	if (m_pDraw3D && m_pDraw3D->Parent->GetName() == PLAYER_NAME) {
		if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
			m_bAnimation = true;
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_V)) {
			m_dCurrentAnimTime = m_dStart[m_nCurrentAnimState];
			//アニメーションの反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_N))
		{
			m_dCurrentAnimTime -= DEFAULT_PLAY_SPEED;
			// アニメーション反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_M))
		{
			m_dCurrentAnimTime += DEFAULT_PLAY_SPEED;
			// アニメーション反映
			m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_B))
		{
			this->Stop();
		}
	}

#endif

	//アニメーションの再生
	if (m_bAnimation) {
		m_dCurrentAnimTime += m_dSpeed[m_nCurrentAnimState];

		//アニメーション反映
				// アニメーション反映
		m_pDraw3D->SetAnimTime(m_dCurrentAnimTime);

		//アニメーションが一周したとき
		if (m_dCurrentAnimTime > m_dEnd[m_nCurrentAnimState]) {
			if (m_bLoop[m_nCurrentAnimState]) {
				//アニメーションの始まり
				m_dCurrentAnimTime = m_dStart[m_nCurrentAnimState];
			}
			else {
				if (m_bIdle) {
					//終わったことを知らせる
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
* @brief	描画処理
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
* @brief	最初から再生
* @param	(int)	アニメーションの情報
*/
void Animation::Play(int AnimState) {
	//再生フラグON
	m_bAnimation = true;

	//現在のアニメと次のアニメが一緒の場合何もしない
	if (m_nCurrentAnimState == AnimState) {
		return;
	}

	//アニメ切替
	m_nCurrentAnimState = AnimState;

	//アニメーションを始まりのフレーム数に設定
	m_dCurrentAnimTime = m_dStart[AnimState];
}

/**
* @fn		Animation::Stop
* @brief	停止する
*/
void Animation::Stop() {
	//再生フラグON
	m_bAnimation = false;
}

/**
*@fn		Animation::SetLoop
*@brief		ループ再生		
*@param		(int)	アニメーションの番号
*@param		(bool)	ループさせるかどうか	
*/
void Animation::SetLoop(int AnimID,bool loop) {
	m_bLoop[AnimID] = loop;
}


/**
* @fn		Animation::SetModel
* @brief	モデルの設定
* @param	(CDraw3D*)	モデルのポインタ
*/
void Animation::SetModel(CDraw3D* model) {
	m_pDraw3D = model;
}

/**
* @fn		Animation::AddAnimation
* @brief	アニメーションを追加する
* @param	(int)		アニメーションの番号
* @param	(double)	アニメーションスタート番号		
* @param	(double)	アニメーション終わりの番号
* @param	(double)	ループが始まるフレーム
* @param	(bool)		ループさせるかどうか
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
* @brief	常に再生しているかどうか
* @param	(bool)	常にアニメーションを流すか
*/
void Animation::AlwaysAnimation(bool always) {
	m_bIdle = always;
}

/**
* @fn		Animation::SetPlaySpeed
* @brief	アニメーションの再生スピードの設定
* @param	(int)		アニメーションの番号
* @param	(double)	アニメーションのスピード
*/
void Animation::SetPlaySpeed(int AnimID,double speed) {
	m_dSpeed[AnimID] = speed;
}

/**
* @fn		Animation::AnimEnd
* @brief	アニメーションを終わらせる
* @param	(int)	どのモデルを終わらせるか
* @return	(bool)	終わらせれたかどうか
*/
bool Animation::AnimEnd(int AnimID) {
	if (m_bEnd[AnimID]) {
		//終わりフラグのリセット
		m_bEnd[AnimID] = false;

		return true;
	}
	else {
		return false;
	}
}