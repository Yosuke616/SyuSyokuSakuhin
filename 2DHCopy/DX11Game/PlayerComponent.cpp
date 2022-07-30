//インクルード部
#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Camera.h"

//定数定義
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)
#define VALUE_JUMP_SPEED		(8.0f)
#define VALUE_MOVE_SPEED		(1.5f)

/**グローバル変数**/
PLAYER_STATE g_ePlayer;

/**
* @fn		CPlayer::CPlayer
* @brief	俺の名前はコンストラクタ　ただのコンストラクタ
* @detail	トランスフォームや描画情報をイニシャライズでnullにする
*/
CPlayer::CPlayer() :
	m_pPlayer(nullptr),m_pDraw2D(nullptr),m_pInput(nullptr),
	m_OldPos(0.0f,0.0f),m_bGround(false),
	m_bJump(false),
	m_ePlayer(IDLE_PLAYER){
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
	g_ePlayer = m_ePlayer;
}

/**
* @fn		CPlayer::Update
* @brief	更新処理
*/
void CPlayer::Update() {
	m_OldPos = m_pCollider->GetCenterPos();

	//ステータスの情報を更新する
	m_ePlayer = g_ePlayer;

#if _DEBUG
	if (m_pInput->GetKeyPress(DIK_UPARROW)) {
		//上移動
		m_pPlayer->Pos.y += 10.0f;
	}	
	if (m_pInput->GetKeyPress(DIK_DOWNARROW)) {
		//下移動
		m_pPlayer->Pos.y += -10.0f;
	}
	if (m_pInput->GetKeyPress(DIK_RIGHTARROW)) {
		//右移動
		m_pPlayer->Pos.x += 5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_LEFTARROW)) {
		//右移動
		m_pPlayer->Pos.x += -5.0f;
	}
#endif

	switch (m_ePlayer) {
	case IDLE_PLAYER:
		//特別なアクションを起こした場合、プレイヤーの状態を変える

		//横移動はなしにする,縦は重力がかかる
		m_pPlayer->Vel.x = 0.0f;

		if (m_pInput->GetKeyPress(DIK_D)) {
			//右移動
			m_ePlayer = RUN_PLAYER;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//左移動
			m_ePlayer = RUN_PLAYER;
		}

		//ジャンピング
		if (m_ePlayer == IDLE_PLAYER) {
			if (m_pInput->GetKeyPress(DIK_W)) {
				//ステータスをジャンプに変える
				m_ePlayer = JUMP_PLAYER;
			}
		}
		break;

	case RUN_PLAYER:
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//左移動
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_ePlayer = IDLE_PLAYER;
		}

		//ジャンピング	
		if (m_pInput->GetKeyPress(DIK_W)) {
			//ステータスをジャンプに変える
			m_ePlayer = JUMP_PLAYER;
		}
	
		break;

	case JUMP_PLAYER:
		////ある程度上げたらくわえた力を消す

		//初速を加える
		if (!m_bJump) {
			//重力のコンポーネントの機能をオフにする
			Parent->GetComponent<CGravity>()->SetUse(false);
			m_pPlayer->Vel.y = VALUE_JUMP_SPEED;
			m_bJump = true;
		}
		else {
			m_pPlayer->Vel.y -= GRAVITY;
			if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
				Parent->GetComponent<CGravity>()->SetUse(true);
			}
		}

		//左右移動はできるようにしておく
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//左移動
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		break;

	case FALL_PLAYER:
		//落下速度が無くなったらプレイヤーの状態を待機状態に戻す
		if (m_pPlayer->Vel.y >= 0) {
			m_ePlayer = IDLE_PLAYER;
		}

		//左右移動はできるようにしておく
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//左移動
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		//落下フラグはオンにする
		m_bJump = true;

		break;
	default: break;
	}

	//プレイヤーの最新状態を保存する
	g_ePlayer = m_ePlayer;

	//速度の上限を決める
	if (m_pPlayer->Vel.x > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.x < -VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
		m_pPlayer->Vel.y = -MAX_VELOCITY;
	}

	/*if (m_pPlayer->Vel.y > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}*/
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
	Text("P_STATE : %d",g_ePlayer);
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
	if(pObject->GetName() == BLOCK_NAME){
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
				m_pPlayer->Vel.y = 0.0f;

				//座標補正
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - PlayerOffset.y;	

				//プレイヤーの状態を地面に立っている状態に変える
				m_ePlayer = IDLE_PLAYER;

				//プレイヤーの最新状態を保存する
				if (m_bJump == true) {
					g_ePlayer = m_ePlayer;
					m_bJump = false;
				}
			}
			/** @brief 頭ごっつんこ状態である*/
			else if (BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//前フレームはめり込んでいない
				PlayerPos.y + PlayerHalhSize.y > BlockDownLine) {		//現在のフレームはめり込んでいる
				   //yの速度をなくす
				m_pPlayer->Vel.y = 0.0f;
				//座標補正
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - PlayerOffset.y;
			}
		}

		//落下速度が一定を超えたら落下判定にする
		if (m_pPlayer->Vel.y == -MAX_VELOCITY) {
			int i = 1;
		}
		else {
			int i = 1;
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

/**
* @fn		CPlayer::SetPlayerStatus
* @brief	プレイヤーの状態をセットする
* @detail	イベントの時にプレイヤーをストップしたり再開したり
* @param	(PLAYER_STATUS)	指定したいプレイヤーの状態
*/
void CPlayer::SetPlayerState(PLAYER_STATE PlayerSta) {
	m_ePlayer = g_ePlayer = PlayerSta;
}