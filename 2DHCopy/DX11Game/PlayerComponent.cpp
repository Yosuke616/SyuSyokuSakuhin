//インクルード部
#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Camera.h"

//定数定義
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)
#define VALUE_JUMP_SPEED		(8.0f)
#define VALUE_MOVE_SPEED		(2.0f)
#define STAR_TIME				(180)

/**グローバル変数**/
PLAYER_STATE g_ePlayer;

/**
* @fn		CPlayer::CPlayer
* @brief	俺の名前はコンストラクタ　ただのコンストラクタ
* @detail	トランスフォームや描画情報をイニシャライズでnullにする
*/
CPlayer::CPlayer() :
	m_pPlayer(nullptr),m_pDraw2D(nullptr),m_pInput(nullptr),
	m_OldPos(0.0f,0.0f),					//過去座標
	m_nStar_Time(0),						//無敵時間
	m_bGround(false),						//地上にいるかどうか
	m_bJump(false),							//ジャンプしているかどうか
	m_bPawer_UP(true),						//パワーアップしているか
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
	m_pDraw2D = Parent->GetComponent<CAnimMesh>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pInput = InputManager::Instance();
	g_ePlayer = m_ePlayer;
	//パワーアップと無敵時間の初期化
	m_bPawer_UP = true;
	m_nStar_Time = 0;
	//初めは右を向いている
	m_bROL = true;
	//どちらからヒットしたかを判別する(0は何にも当たっていない)
	m_nHitVec = 0;

}

/**
* @fn		CPlayer::Update
* @brief	更新処理
*/
void CPlayer::Update() {
	m_OldPos = m_pCollider->GetCenterPos();

	//ステータスの情報を更新する
	m_ePlayer = g_ePlayer;

	//無敵状態だったら半透明にする
	if (m_nStar_Time > 0) {
		m_pDraw2D->SetColor(1.0f,0.0f,0.0f);
		m_nStar_Time--;
	}
	else
	{
		m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
	}

	//敵にヒットした場合吹き飛ばすためにプレイヤーの状態を変更する
	if (m_nHitVec > 0) {
		m_ePlayer = HIT_PLAYER;
	}

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
			//右移動
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
			//右移動
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
			//右移動
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

	case HIT_PLAYER:
		//敵にぶつかって吹き飛ばされた時の判定
		//ぶつかった方向とは逆向きに速度を加える
		//1:下から、2:上から、3:右から、4:左から

		switch (m_nHitVec)
		{
		case 1:
			//上方向に力を加える

			break;
		case 2:break;
		case 3:break;
		case 4:break;

		default:break;
		}
		
		break;

	default: break;
	}

	//左右移動のボタンが押されていた場合左右のフラグを入れ替える
	if (m_pInput->GetKeyPress(DIK_D)) {
		//右向き
		m_bROL = true;
	}
	else if (m_pInput->GetKeyPress(DIK_A)) {
		//左向き
		m_bROL = false;
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
	Text("RoL	  : %d",m_bROL);
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
#pragma region ---BLOCK
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
#pragma endregion

#pragma region ---ENEMY
	//無敵時間の場合敵とは当たらない
	if (m_nStar_Time <= 0) {
		//敵と分類されるものに当たった場合
		if (pObject->GetName() == ENEMY_NAME) {
			//パワーアップ状態かそうでないかで処理内容を決める
			if (m_bPawer_UP) {
				//パワーアップしているということは強くなっているということである
				//無敵カウントを増やす
				m_nStar_Time = STAR_TIME;
				//パワーアップ状態を解除する
				m_bPawer_UP = false;
				//吹き飛ばす向きを決める
				m_nHitVec = CollEnemy(pObject);
			}
			else {
				//当たったらミス状態である
			}

			//やりたいこと
			//プレイヤーの状態で決める(パワーアップ状態かそうでないか)
			//パワーアップ状態で無かったらミス判定
			//そうでない場合は無敵時間を作りパワーダウンさせる

			//ミス判定　プレイヤー以外のオブジェクトの更新を止めてミスのテクスチャを流す
			//テクスチャを流し終えたらメニュー画面の表示(ゲームオーバーメニュー)
			
		
		
		}
	}

#pragma endregion
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

/**
* @fn		Cplayer::CollEnemy
* @brief	敵とどの向きから当たったかで吹き飛ばす方向を決める
* @detail	1:下から、2:上から、3:右から、4:左から
* @param	(Object*)	当たった敵の情報がはいいているポインタ
* @return	(int)		どっちからあったかを返す
*/
int CPlayer::CollEnemy(Object* pObject) {
	//プレイヤーの情報を取得
	auto Player = Parent->GetComponent<CCollider>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffset = Player->GetOffSet();

	//ぶつかったブロックの情報
	auto Enemy = pObject->GetComponent<CCollider>();
	auto EnemyPos = Enemy->GetCenterPos();
	auto EnemySize = Enemy->GetColliderSize();
	auto EnemyOffset = Enemy->GetOffSet();

	//それぞれ半分の大きさを保存
	XMFLOAT2 PlayerHalhSize = XMFLOAT2(PlayerSize.x * 0.5f, PlayerSize.y *0.5f);
	XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y * 0.5f);

	//ブロックの当たり判定の線
	float EnemyLeftLine = EnemyPos.x - EnemyHalhSize.x;	//左端
	float EnemyRightLine = EnemyPos.x + EnemyHalhSize.x;	//右端
	float EnemyUpLine = EnemyPos.y + EnemyHalhSize.y;	//上端
	float EnemyDownLine = EnemyPos.y - EnemyHalhSize.y;	//下端

	/**
	* @brief プレイヤーがブロックの中(左端と右端の中)にいた場合の処理
	*/
	if ((EnemyLeftLine < PlayerPos.x && PlayerPos.x < EnemyRightLine) ||
		(EnemyLeftLine < PlayerPos.x - PlayerHalhSize.x && PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) ||
		(EnemyLeftLine < PlayerPos.x + PlayerHalhSize.x && PlayerPos.x + PlayerHalhSize.x < EnemyRightLine)) {

		/** @brief プレイヤーが上に乗ったら*/
		if (EnemyUpLine <= m_OldPos.y - PlayerHalhSize.y &&					//前フレームはめり込んでいない
			PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) {					//現在のフレームはめり込んでいる

			return 1;
		}
		/** @brief 頭ごっつんこ状態である*/
		else if (EnemyDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//前フレームはめり込んでいない
			PlayerPos.y + PlayerHalhSize.y > EnemyDownLine) {		//現在のフレームはめり込んでいる

			return 2;
		}
	}

	//中心座標のセット
	PlayerPos = Player->GetCenterPos();

	/** @brief プレイヤーがブロックの中(上端と下端の中)にいた場合の処理*/
	if ((EnemyDownLine < PlayerPos.y && PlayerPos.y < EnemyUpLine) ||
		(EnemyDownLine < PlayerPos.y - PlayerHalhSize.y && PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) ||
		(EnemyDownLine < PlayerPos.y + PlayerHalhSize.y && PlayerPos.y + PlayerHalhSize.y < EnemyUpLine)) {
		//プレイヤーが右のブロックに当たった場合
		if (EnemyLeftLine >= m_OldPos.x + PlayerHalhSize.x &&		//前フレームはめり込んでいない
			PlayerPos.x + PlayerHalhSize.x > EnemyLeftLine) {		//現在のフレームはめり込んでいる

			return 3;
		}
		else if (EnemyRightLine <= m_OldPos.x - PlayerHalhSize.x &&
			PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) {

			return 4;
		}
	}
	return 1;
}