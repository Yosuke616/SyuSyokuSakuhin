//インクルード部
#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "Camera.h"

//定数定義
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)
#define VALUE_JUMP_SPEED		(1.5f)
#define VALUE_MOVE_SPEED		(1.0f)

/**
* @fn		CPlayer::CPlayer
* @brief	俺の名前はコンストラクタ　ただのコンストラクタ
* @detail	トランスフォームや描画情報をイニシャライズでnullにする
*/
CPlayer::CPlayer() :
	m_pPlayer(nullptr),m_pDraw2D(nullptr),m_pInput(nullptr),
	m_OldPos(0.0f,0.0f),m_bGround(false){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CPlayer::~CPlayer()
* @brief	デストラクタしたのか　俺以外のやつと
*/
CPlayer::~CPlayer() {

}

/**
* @fn		CPlayer::Start
* @brief	開始処理
*/
void CPlayer::Start() {
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pInput = InputManager::Instance();
}

/**
* @fn		CPlayer::Update
* @brief	更新処理
*/
void CPlayer::Update() {
	m_pPlayer->Vel = XMFLOAT3(0.0f,0.0f,0.0f);
	m_OldPos = m_pCollider->GetCenterPos();

	//キー入力で移動する
	if (m_pInput->GetKeyPress(DIK_D)) {
		//右移動
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
	}
	
	if (m_pInput->GetKeyPress(DIK_A)) {
		//左移動
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
	}

	if (m_pInput->GetKeyPress(DIK_W)) {
		//上移動
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}

	if (m_pInput->GetKeyPress(DIK_S)) {
		//下移動
		m_pPlayer->Vel.y = -VALUE_MOVE_SPEED;
	}

	//速度の上限を決める
	if (m_pPlayer->Vel.x > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.x < -VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.y > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.y < -VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = -VALUE_MOVE_SPEED;
	}
}

/**
* @fn		CPlayer::Draw
* @brief	プレイヤーをデバッグ表示
*/
void CPlayer::Draw() {
#ifdef _DEBUG
	//	debug
	XMFLOAT2 a = Parent->GetComponent<CCollider>()->GetCenterPos();

	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Player");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pPlayer->Pos.x, m_pPlayer->Pos.y, m_pPlayer->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pPlayer->Vel.x, m_pPlayer->Vel.y);
	End();
#endif // _DEBUG
}

/**
* @fn		CPlayer::OnCollisionEnter
* @brief	プレイヤーが何かに当たったときの処理
* @param	(Object*)	相手方のオブジェクトのポインタ
*/
void CPlayer::OnCollisionEnter(Object* pObject) {
	//仮(ブロック)
	if(pObject->GetName() == BLOCK_KARI){
		//プレイヤーの情報を取得
		auto Player = Parent->GetComponent<CCollider>();
		auto PlayerPos = Player->GetCenterPos();
		auto PlayerSize = Player->GetColliderSize();
		auto PlayerOffset = Player->GetOffSet();

		//ぶつかったブロックの情報
		auto Block = pObject->GetComponent<CCollider>();
		auto BlockPos = Block->GetCenterPos();
		auto BlockSize = Block->GetColliderSize();
		auto BlockOffset = Block->GetOffSet();

		//それぞれ半分の大きさを保存
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(PlayerSize.x * 0.5f,PlayerSize.y *0.5f);
		XMFLOAT2 BlockHalhSize = XMFLOAT2(BlockSize.x * 0.5f,BlockSize.y * 0.5f);

		//ブロックの当たり判定の線
		float BlockLeftLine  = BlockPos.x - BlockHalhSize.x;	//左端
		float BlockRightLine = BlockPos.x + BlockHalhSize.x;	//右端
		float BlockUpLine	 = BlockPos.y + BlockHalhSize.y;	//上端
		float BlockDownLine  = BlockPos.y - BlockHalhSize.y;	//下端

		/**
		* @brief プレイヤーがブロックの中(左端と右端の中)にいた場合の処理
		*/
		if ((BlockLeftLine < PlayerPos.x && PlayerPos.x < BlockRightLine) ||
			(BlockLeftLine < PlayerPos.x - PlayerHalhSize.x && PlayerPos.x - PlayerHalhSize.x < BlockRightLine) ||
			(BlockLeftLine < PlayerPos.x + PlayerHalhSize.x && PlayerPos.x + PlayerHalhSize.x < BlockRightLine)) {
			
			/** @brief プレイヤーが上に乗ったら*/
			if (BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&					//前フレームはめり込んでいない
							   PlayerPos.y - PlayerHalhSize.y < BlockUpLine) {	//現在のフレームはめり込んでいる
				//yの速度をなくす
				m_pPlayer->Pos.y = 0.0f;

				//座標補正
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - PlayerOffset.y;				
			}
			else if (BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//前フレームはめり込んでいない
				PlayerPos.y + PlayerHalhSize.y > BlockDownLine) {		//現在のフレームはめり込んでいる
				   //yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;
				//座標補正
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - PlayerOffset.y;
			}
		}

		//中心座標のセット
		PlayerPos = Player->GetCenterPos();

		/** @brief プレイヤーがブロックの中(上端と下端の中)にいた場合の処理*/
		if ((BlockDownLine < PlayerPos.y && PlayerPos.y < BlockUpLine) ||
			(BlockDownLine < PlayerPos.y - PlayerHalhSize.y && PlayerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < PlayerPos.y + PlayerHalhSize.y && PlayerPos.y + PlayerHalhSize.y < BlockUpLine)) {
			//プレイヤーが右のブロックに当たった場合
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&		//前フレームはめり込んでいない
				PlayerPos.x + PlayerHalhSize.x > BlockLeftLine) {		//現在のフレームはめり込んでいる
				//移動量を削除する
				m_pPlayer->Vel.x = 0.0f;

				//座標補正
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - PlayerOffset.x;
			}
			else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
				PlayerPos.x - PlayerHalhSize.x < BlockRightLine) {
				//移動量を削除する
				m_pPlayer->Vel.x = 0.0f;

				//座標の補正
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - PlayerOffset.x;
			}
		}
		return;
	}	
}