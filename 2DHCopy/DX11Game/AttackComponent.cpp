/**インクルード部**/
#include "AttackComponent.h"
#include "TransformComponent.h"
#include "PlayerComponent.h"
#include "SeeCollComponent.h"

/**定数定義**/
#define ATTACK_FRAME			(60)	//攻撃持続時間
#define ATTACK_COOL_TIME		(10)	//次の攻撃に移れるまでの時間(プレイヤー)

/**
* @fn		CAttack::CAttack
* @brief	コンストラクタは命より重い！！！
*/
CAttack::CAttack() 
	:m_pTrans(nullptr),m_pObj(nullptr)
	,m_eAttack(NONE_ATTACK){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CAttack::~CAttack
* @brief	デストラクタやめられないんだけど
*/
CAttack::~CAttack() {
	m_pObj = nullptr;
	m_eAttack = NONE_ATTACK;
}

/**
* @fn		CAttack::Start
* @brief	初期化処理
*/
void CAttack::Start() {
	m_pTrans = Parent->GetComponent<CTransform>();
	//どのオブジェクトにより攻撃かを場合分けする
	switch (m_eAttack)
	{
	case PLAYER_ATTACK:
		//攻撃がどっち向いているかを取得
		m_nAttackCnt = ATTACK_FRAME;
		break;

	default:break;
	}
}

/** 
* @fn		CAttack::Update
* @brief	更新処理
* @detail	どのオブジェクトが攻撃しているかで処理を変える
*/
void CAttack::Update() {
	switch (m_eAttack)
	{
	case PLAYER_ATTACK:
		//プレイヤーに合わせた速度を足す
		Parent->GetComponent<CTransform>()->Vel = m_pObj->GetComponent<CTransform>()->Vel;

		//オブジェクトを下に下げていく
		Parent->GetComponent<CTransform>()->Pos.y -= 4.0f;

		//攻撃用当たり判定のをゆっくり消していく
		m_nAttackCnt -= 4;
		//当たり判定を削除する
		if (m_nAttackCnt <= 0) {
			//当たり判定の削除
			Parent->Delete();

			//メッシュの削除
			Parent->GetComponent<CSeeColl>()->DeleteCollBox();

			//プレイヤーの攻撃状態を変えるためのやつら
			//クールタイムの設定
			m_pObj->GetComponent<CPlayer>()->SetCoolTime(ATTACK_COOL_TIME);

			//再び攻撃用当たり判定を生成できるようにする
			m_pObj->GetComponent<CPlayer>()->SetAttackFlg(false);

			//プレイヤーの状態を元に戻す
			//空中かどうかでステータスを変える
			if (m_pObj->GetComponent<CPlayer>()->GetPlayerJump()) {
				m_pObj->GetComponent<CPlayer>()->SetPlayerState(JUMP_PLAYER);
				m_pObj->GetComponent<CPlayer>()->ChangeTexture();
			}
			else {
				m_pObj->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
				m_pObj->GetComponent<CPlayer>()->ChangeTexture();
			}

		}
		break;

	default:break;
	}
}

/**
* @fn		CAttack::GetAttackRoL
* @brief	攻撃がどちらからされたかのフラグを取得する
* @return	(bool)	右ならtrue、左ならfalseを返す
*/
bool CAttack::GetAttackRoL() {
	return m_bRoL;
}

/**
* @fn		CAttack::SetAttackType
* @brief	誰が攻撃したかを判別する
* @param	(WHO_ATTACK)	誰が攻撃したかを設定する
*/
void CAttack::SetAttackType(WHO_ATTACK who) {
	m_eAttack = who;
}

/**
* @fn		CAttack::SetSetRoL
* @brief	右か左かセットする
* @param	(bool)	右ならtrue、左ならfalse
*/
void CAttack::SetRoL(bool RoL) {
	m_bRoL = RoL;
}

/**
* @fn		CAttack::SetObj
* @brief	攻撃したオブジェクトの情報の入ったポインタをセットする
* @param	(Object*) Objectのポインタ
*/
void CAttack::SetObject(Object* pObj) {
	m_pObj = pObj;
}