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
	,m_OldPos(0.0f,0.0f){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CEnemy::~CEnemy
* @brief	デストラクタする・・・それが私の生まれた意味！
*/
CEnemy::~CEnemy() {

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
}

/**
* @fn		CEnemy::Update
* @brief	更新処理
*/
void CEnemy::Update() {
	//過去座標を更新する
	m_OldPos = m_pCollider->GetCenterPos();

	//trueだったら左へ
	if (m_bRightorLeft) {
		//一生左に動き続ける人生
		m_pTransform->Vel.x = -VALUE_MOVE_ENEMY;
	}else{
		//右にも移動できる知能はあったらしい
		m_pTransform->Vel.x = VALUE_MOVE_ENEMY;
	}

}

/**
* @fn		CEnemy::Draw
* @brief	描画する(デバックで出したい情報を)
*/
void CEnemy::Draw() {
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
		return;
	}
}
