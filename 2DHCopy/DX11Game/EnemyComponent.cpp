/**インクルード部**/
#include "EnemyComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "imgui.h"
#include "AttackComponent.h"
#include "SeeCollComponent.h"
#include "sceneGame.h"

/**定数定義**/
#define MAX_ENEMY_GRAVITY	(1.0f)
#define VALUE_MOVE_ENEMY	(1.5f)

/**
* @fn		CEnemy::CEnemy
* @brief	シンジ、コンストラクタに乗れ
* @detail	トランスフォーム・描画・当たり判定等のポインタをヌルにする
*/
CEnemy::CEnemy() 
	:m_pTransform(nullptr),m_pDraw(nullptr),m_pCollider(nullptr)
	,m_OldPos(0.0f,0.0f)
	,m_bStopCom(false),m_bRoL(false),m_bStopVel(false)
	,m_eEnemy_Type(ENEMY_WALK),m_eEnemy_State(ENEMY_DEFAULT){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CEnemy::~CEnemy
* @brief	デストラクタする・・・それが私の生まれた意味！
*/
CEnemy::~CEnemy() {
	TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
	pMaterial->Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

/**
* @fn		CEnemy::Start
* @brief	開始処理
*/
void CEnemy::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw3D>();
	m_pCollider	 = Parent->GetComponent<CCollider>();
	m_bRightorLeft = true;
	m_eEnemy_State = ENEMY_DEFAULT;
	m_bStopCom = false;
	m_bStopVel = false;
	m_nBlink = 0;
	m_nDeleteFlg = 0;
}

/**
* @fn		CEnemy::Update
* @brief	更新処理
*/
void CEnemy::Update() {
	//過去座標を更新する
	m_OldPos = m_pCollider->GetCenterPos();
	
	switch (m_eEnemy_State)
	{
	case ENEMY_DEFAULT:
		//敵の種類によって挙動を変えていく
		switch (m_eEnemy_Type) {
		case ENEMY_WALK:
			//trueだったら左へ
			if (m_bRightorLeft) {
				//一生左に動き続ける人生
				m_pTransform->Vel.x = -VALUE_MOVE_ENEMY;
				m_pTransform->SetRotate(0.0f,90.0f,0.0f);
			}
			else {
				//右にも移動できる知能はあったらしい
				m_pTransform->Vel.x = VALUE_MOVE_ENEMY;
				m_pTransform->SetRotate(0.0f, -90.0f, 0.0f);
			}
			break;

		default:break;
		}
		break;

	case ENEMY_DELETE:
		if (!m_bStopCom) {

			//全てのコンポーネントの動きを停止する
			//ただしCTransform、おめぇはだめだ
			//Draw3D、おめーもだめだ
			//このコンポーネントもだ
			for (auto&& object : Parent->GetComponentList()) {
				object->m_bUpdateFlag = false;
			}
			Parent->GetComponent<CTransform>()->m_bUpdateFlag = true;
			Parent->GetComponent<CDraw3D>()->m_bUpdateFlag = true;
			Parent->GetComponent<CEnemy>()->m_bUpdateFlag = true;

			m_bStopCom = true;
			
			//急に吹き飛ばされた感を出すために速度は0にする
			m_pTransform->Vel = XMFLOAT3(0.0f,0.0f,0.0f);
		}

		if (!m_bStopVel) {
			//ポジションを変える(速度を変えた方が自然かも)
			//プレイヤーの左右の向きを取得してプレイヤーの向いている方向に飛ばす
			if (m_bRoL) {
				//右から攻撃された場合
				m_pTransform->Vel.x += 1.75f;
				m_pTransform->Vel.y += 2.0f;

				//速度が一定数超えたらフラグを変える
				if (m_pTransform->Vel.y > 10.0f) {
					m_bStopVel = true;
					m_pTransform->Vel.x = 0.0f;
					m_pTransform->Vel.y = 0.0f;
				}

			}
			else {
				//左から攻撃された場合
				m_pTransform->Vel.x -= 1.75f;
				m_pTransform->Vel.y += 2.0f;

				//速度が一定数超えたらフラグを変える
				if (m_pTransform->Vel.y > 10.0f) {
					m_bStopVel = true;
					m_pTransform->Vel.x = 0.0f;
					m_pTransform->Vel.y = 0.0f;
				}
			}		

		}

		//全てのてきが点滅するから変えないといけない

		

		//削除フラグがたったら消す
		if (m_nDeleteFlg > 5) {
			Parent->Delete();
			Parent->GetComponent<CSeeColl>()->DeleteCollBox();
			SceneGame::GetInstance()->SetScore(100);
		}

		break;
	default:break;
	}
}

/**
* @fn		CEnemy::Draw
* @brief	描画する(デバックで出したい情報を)
*/
void CEnemy::Draw() {
	//色を変える
	if (m_eEnemy_State == ENEMY_DELETE) {
		//点滅させてから消し去る
		m_nBlink++;
		//敵のYのスピードを2で割って偶数で表示
		if (m_nBlink < 5) {
			//消す
			//モデルの色を変える
			TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
			pMaterial->Kd = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (m_nBlink < 10) {
			//付ける
			TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
			pMaterial->Kd = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			m_nBlink = 0;
			m_nDeleteFlg++;
		}
	}
	else {
		TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
		pMaterial->Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Enemy");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pTransform->Pos.x, m_pTransform->Pos.y, m_pTransform->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pTransform->Vel.x, m_pTransform->Vel.y);
	End();
#endif
}

/**
* @fn		CEnemy::OnCollisionEnter
* @brief	敵が何か物に当たったときの処理
* @param	(Object*)	相手方のオブジェクトの情報が入ったポインタ
*/
void CEnemy::OnCollisionEnter(Object* pObject) {
	//仮(ブロック)
#pragma region ---ブロック
	if (pObject->GetName() == BLOCK_NAME) {
		//敵の情報を取得
		auto Enemy = Parent->GetComponent<CCollider>();
		auto EnemyPos = Enemy->GetCenterPos();
		auto EnemySize = Enemy->GetColliderSize();
		auto EnemyOffset = Enemy->GetOffSet();

		//ぶつかったブロックの情報
		auto Block = pObject->GetComponent<CCollider>();
		auto BlockPos = Block->GetCenterPos();
		auto BlockSize = Block->GetColliderSize();
		auto BlockOffset = Block->GetOffSet();

		//それぞれ半分の大きさを保存
		XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y *0.5f);
		XMFLOAT2 BlockHalhSize = XMFLOAT2(BlockSize.x * 0.5f, BlockSize.y * 0.5f);

		//ブロックの当たり判定の線
		float BlockLeftLine = BlockPos.x - BlockHalhSize.x;	//左端
		float BlockRightLine = BlockPos.x + BlockHalhSize.x;	//右端
		float BlockUpLine = BlockPos.y + BlockHalhSize.y;	//上端
		float BlockDownLine = BlockPos.y - BlockHalhSize.y;	//下端

		/**
		* @brief エネミーがブロックの中(左端と右端の中)にいた場合の処理
		*/
		if ((BlockLeftLine < EnemyPos.x && EnemyPos.x < BlockRightLine) ||
			(BlockLeftLine < EnemyPos.x - EnemyHalhSize.x && EnemyPos.x - EnemyHalhSize.x < BlockRightLine) ||
			(BlockLeftLine < EnemyPos.x + EnemyHalhSize.x && EnemyPos.x + EnemyHalhSize.x < BlockRightLine)) {

			/** @brief 敵が上に乗ったら*/
			if (BlockUpLine <= m_OldPos.y - EnemyHalhSize.y &&					//前フレームはめり込んでいない
				EnemyPos.y - EnemyHalhSize.y < BlockUpLine) {	//現在のフレームはめり込んでいる
 //yの速度をなくす
				m_pTransform->Vel.y = 0.0f;

				//座標補正
				m_pTransform->Pos.y = BlockUpLine + EnemyHalhSize.y - EnemyOffset.y;
			}
			/** @brief 頭ごっつんこ状態である*/
			else if (BlockDownLine >= m_OldPos.y + EnemyHalhSize.y &&	//前フレームはめり込んでいない
				EnemyPos.y + EnemyHalhSize.y > BlockDownLine) {		//現在のフレームはめり込んでいる
				   //yの速度をなくす
				m_pTransform->Vel.y = 0.0f;
				//座標補正
				m_pTransform->Pos.y = BlockDownLine - EnemyHalhSize.y - EnemyOffset.y;
			}
		}

		//落下速度が一定を超えたら落下判定にする
		if (m_pTransform->Vel.y == -MAX_VELOCITY) {
			int i = 1;
		}
		else {
			int i = 1;
		}

		//中心座標のセット
		EnemyPos = Enemy->GetCenterPos();

		/** @brief 敵がブロックの中(上端と下端の中)にいた場合の処理*/
		if ((BlockDownLine < EnemyPos.y && EnemyPos.y < BlockUpLine) ||
			(BlockDownLine < EnemyPos.y - EnemyHalhSize.y && EnemyPos.y - EnemyHalhSize.y < BlockUpLine) ||
			(BlockDownLine < EnemyPos.y + EnemyHalhSize.y && EnemyPos.y + EnemyHalhSize.y < BlockUpLine)) {
			//敵が右のブロックに当たった場合
			if (BlockLeftLine >= m_OldPos.x + EnemyHalhSize.x &&		//前フレームはめり込んでいない
				EnemyPos.x + EnemyHalhSize.x > BlockLeftLine) {		//現在のフレームはめり込んでいる
				//移動量を削除する
				m_pTransform->Vel.x = 0.0f;

				//座標補正
				m_pTransform->Pos.x = BlockLeftLine - EnemyHalhSize.x - EnemyOffset.x;
				//反転させる
				m_bRightorLeft = true;
			}
			else if (BlockRightLine <= m_OldPos.x - EnemyHalhSize.x &&
				EnemyPos.x - EnemyHalhSize.x < BlockRightLine) {
				//移動量を削除する
				m_pTransform->Vel.x = 0.0f;

				//座標の補正
				m_pTransform->Pos.x = BlockRightLine + EnemyHalhSize.x - EnemyOffset.x;
				//反転させる
				m_bRightorLeft = false;
			}
		}
	}
#pragma endregion
	//プレイヤーの攻撃
	if (pObject->GetName() == ATTACK_NAME) {
		//敵の情報を取得
		auto Enemy = Parent->GetComponent<CCollider>();
		auto EnemyPos = Enemy->GetCenterPos();
		auto EnemySize = Enemy->GetColliderSize();
		auto EnemyOffset = Enemy->GetOffSet();
		//半分の大きさ
		XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y *0.5f);

		//当たり判定の情報
		auto TAttack = pObject->GetComponent<CTransform>();
		auto cAttack = pObject->GetComponent<CCollider>();
		//半分の大きさ
		XMFLOAT2 AttackHalfSize = XMFLOAT2(cAttack->GetColliderSize().x/2.0f,cAttack->GetColliderSize().y/2.0f);

		if (cAttack->GetCenterPos().x - AttackHalfSize.x + TAttack->Vel.x < m_pCollider->GetCenterPos().x + EnemyHalhSize.x + m_pTransform->Vel.x &&
			m_pCollider->GetCenterPos().x - EnemyHalhSize.x + m_pTransform->Vel.x < cAttack->GetCenterPos().x + AttackHalfSize.x + TAttack->Vel.x) {
			if (cAttack->GetCenterPos().y - AttackHalfSize.y + TAttack->Vel.y < m_pCollider->GetCenterPos().y + EnemyHalhSize.y + m_pTransform->Vel.y &&
				m_pCollider->GetCenterPos().y - EnemyHalhSize.y + m_pTransform->Vel.y < cAttack->GetCenterPos().y + AttackHalfSize.y + TAttack->Vel.y) {
				//攻撃がヒットしたので敵オブジェクトを削除する
				m_eEnemy_State = ENEMY_DELETE;
				m_bRoL = pObject->GetComponent<CAttack>()->GetAttackRoL();
			}
		}
	}
}

/**
* @fn		CEnemy::SetEnemyType
* @brief	敵の種類を決める関数(3種類くらい？)
* @param	(ENEMY_TYPE) 敵の種類を決めてもらう
*/
void CEnemy::SetEnemyType(ENEMY_TYPE type) {
	m_eEnemy_Type = type;
}