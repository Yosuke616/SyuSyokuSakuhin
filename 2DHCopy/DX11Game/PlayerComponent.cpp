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
#include "SeeCollComponent.h"
#include "AttackComponent.h"

//定数定義
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)	//プレイヤーに加わる重力の最大値	
#define VALUE_JUMP_SPEED		(8.0f)	//ジャンプの速さ
#define VALUE_MOVE_SPEED		(2.0f)	//移動スピード
#define STAR_TIME				(180)	//無敵時間
#define HIT_SPEED				(8.0f)	//ぶっ飛ばされた時の勢い
#define KNOCK_BACK_PLAYER		(0.05f)	//戻す時間

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
	m_pAttackObj(nullptr),					//攻撃判定用のオブジェクトを指せるポインタ
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
	//敵にヒットしたときに加える力の処理先の変数
	m_bHitFlg = false;
	//攻撃フラグはオフにしておく
	m_bAttack = false;
	//攻撃時のクールタイム
	m_nCoolTime = 0;
	//エアダッシュの時間は0にしておく
	m_nDushCnt = 0;
	//エアダッシュのクールタイムは0にしておく
	m_nDushCoolTime = 0;
	//空中フラグはオフにしておく
	m_bAirDushFlg = false;
	//敵により死亡したか
	m_bAirDead = false;
	//ミスカウントは0にしておく
	m_nMissCnt = 0;
}

/**
* @fn		CPlayer::Update
* @brief	更新処理
*/
void CPlayer::Update() {
	m_OldPos = m_pCollider->GetCenterPos();

	//ステータスの情報を更新する
	m_ePlayer = g_ePlayer;

	//左右移動のボタンが押されていた場合左右のフラグを入れ替える
	//エアダッシュフラグしている最中は変わらないようにする
	if (!(m_ePlayer == DUSH_PLAYER)) {
		if (m_pInput->GetKeyPress(DIK_D)) {
			//右向き
			m_bROL = true;
		}
		if (m_pInput->GetKeyPress(DIK_A)) {
			//左向き
			m_bROL = false;
		}	
	}

	//消す
	bHitObj = false;


	//無敵状態だったら半透明にする
	if (m_nStar_Time > 0) {
		m_pDraw2D->SetColor(1.0f,0.0f,0.0f);
		m_nStar_Time--;
	}
	else
	{
		m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
	}

	//落下速度を0にする
#ifdef _DEBUG
	if (m_pInput->GetKeyTrigger(DIK_0)) {
		m_pPlayer->Vel.y = 0;
	}
#endif


	switch (m_ePlayer) {
	case IDLE_PLAYER:
#pragma region ---待機
		//特別なアクションを起こした場合、プレイヤーの状態を変える

		//横移動はなしにする,縦は重力がかかる
		m_pPlayer->Vel.x = 0.0f;

		if (m_pInput->GetKeyPress(DIK_D)) {
			//右移動
			m_ePlayer = RUN_PLAYER;
			ChangeTexture();
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//左移動
			m_ePlayer = RUN_PLAYER;
			ChangeTexture();
		}

		//エアダッシュ
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			m_ePlayer = DUSH_PLAYER;
			ChangeTexture();
			//重力の機能をオフにする
			Parent->GetComponent<CGravity>()->SetUse(false);
		}

		//ジャンピング
		if (m_ePlayer == IDLE_PLAYER) {
			if (m_pInput->GetKeyPress(DIK_W)) {
				//ステータスをジャンプに変える
				m_ePlayer = JUMP_PLAYER;
				ChangeTexture();
			}
		}

		//攻撃する
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//プレイヤーの状態を攻撃に変更する
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}
		break;
#pragma endregion
	case RUN_PLAYER:
#pragma region ---移動
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
			ChangeTexture();
		}

		//速度でテクスチャを変える
		if(m_pPlayer->Vel.x >= 0) {
			m_bROL = true;
			ChangeTexture();
		}
		else {
			m_bROL = false;
			ChangeTexture();
		}

		//エアダッシュ
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			m_ePlayer = DUSH_PLAYER;
			ChangeTexture();
			//重力の機能をオフにする
			Parent->GetComponent<CGravity>()->SetUse(false);
		}

		//ジャンピング	
		if (m_pInput->GetKeyPress(DIK_W)) {
			//ステータスをジャンプに変える
			m_ePlayer = JUMP_PLAYER;
			ChangeTexture();
		}
		//攻撃する
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//プレイヤーの状態を攻撃に変更する
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}
	
		break;

#pragma endregion
	case DUSH_PLAYER:
#pragma region ---エアダッシュ
		//クールタイムが明けたらできるようにする
		if (m_nDushCoolTime <= 0) {
			
			//空中にいる場合
			//プレイヤーに加わっている上下の力をなくす
			if (m_bJump) {
				m_pPlayer->Vel.y = 0.0f;
			}

			//エアダッシュし続ける時間の管理
			m_nDushCnt++;
			
			//左右の向きでどっちに速度を加えるかを決める
			if (m_bROL) {
				m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 2;
			}
			else {
				m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 2;
			}

			//エアダッシュが終わったときの処理
			//空中にいるかどうかでプレイヤーの状態は変える
			if (m_nDushCnt > 30) {
				if (m_bJump) {
					//空中の場合
					//プレイヤーの状態を落下状態にする
					m_ePlayer = FALL_PLAYER;
					//プレイヤーの速度を変える
					m_pPlayer->Vel.x = 0.0f;
					//プレイヤーの重力をオンにする
					Parent->GetComponent<CGravity>()->SetUse(true);
					//エアダッシュデフォルトに戻す(所謂初期化)
					m_nDushCnt = 0;
					//テクスチャの変更
					ChangeTexture();
					//クールタイムの設定
					m_nDushCoolTime = 15;
				}
				else {
					//地上の場合
					//プレイヤーの状態をデフォルトに変更する
					m_ePlayer = IDLE_PLAYER;
					//プレイヤーの速度を変える
					m_pPlayer->Vel.x = 0.0f;
					//エアダッシュ時間をデフォルトに戻す(所謂初期化)
					m_nDushCnt = 0;
					//テクスチャの変更
					ChangeTexture();
					//クールタイムの設定
					m_nDushCoolTime = 15;
					//プレイヤーの重力をオンにする
					Parent->GetComponent<CGravity>()->SetUse(true);
				}
			}	
		}
		else {
			//明けてない場合デフォルト状態に戻す
			m_ePlayer = IDLE_PLAYER;
		}


		break;
#pragma endregion
	case JUMP_PLAYER:
#pragma region ---ジャンプ
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

		//エアダッシュ
		//空中でのエアダッシュの処理は少し変える
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			if (!m_bAirDushFlg) {
				m_ePlayer = DUSH_PLAYER;
				ChangeTexture();
				//空中で二度使用できないようにする
				m_bAirDushFlg = true;
				//重力の機能をオフにする
				Parent->GetComponent<CGravity>()->SetUse(false);
			}
		}

		//攻撃する
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//プレイヤーの状態を攻撃に変更する
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}

		break;
#pragma endregion
	case ATTACK_PLAYER:
#pragma region ---攻撃
		//クールタイムの場合は入れない
		if(m_nCoolTime <= 0){
			//フラグがオフだった場合、
			//攻撃判定付きの見えないオブジェクトを生成する
			if (!m_bAttack) {
				CreateAttack();
				m_bAttack = true;
			}

			//空中で攻撃した場合速さはそのままにする
			if (m_bJump) {
				//ジャンプの処理の継続をする
				m_pPlayer->Vel.y -= GRAVITY;
				if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
					Parent->GetComponent<CGravity>()->SetUse(true);
				}

				//空中攻撃
				//左右移動をできるようにする(速さは遅い)
				if (m_pInput->GetKeyPress(DIK_D)) {
					m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
					//当たり判定も動かす
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = VALUE_MOVE_SPEED;
				}
				else if (m_pInput->GetKeyPress(DIK_A)) {
					m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = -VALUE_MOVE_SPEED;
				}
			}
			else {
				//左右移動をできるようにする(速さは遅い)
				if (m_pInput->GetKeyPress(DIK_D)) {
					m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 0.1;
					//当たり判定も動かす
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = VALUE_MOVE_SPEED * 0.1;
				}
				else if (m_pInput->GetKeyPress(DIK_A)) {
					m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 0.1;
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = -VALUE_MOVE_SPEED * 0.1;
				}					
			}
		}
		else {
			//プレイヤーがジャンプしているかどうかでステータスを変える
			if (m_bJump) {
				m_ePlayer = JUMP_PLAYER;
				ChangeTexture();
			}
			else {
				m_ePlayer = IDLE_PLAYER;
				ChangeTexture();			
			}
		}

		break;
#pragma endregion
	case FALL_PLAYER:
#pragma region ---落下
		//落下速度が無くなったらプレイヤーの状態を待機状態に戻す
		if (m_pPlayer->Vel.y >= 0) {
			m_ePlayer = IDLE_PLAYER;
			ChangeTexture();
		}

		//落下フラグはオンにする
		m_bJump = true;

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

		//エアダッシュ
		//こちらも空中でのエアダッシュなので処理を変える
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			if (!m_bAirDushFlg) {
				m_ePlayer = DUSH_PLAYER;
				ChangeTexture();
				m_bAirDushFlg = true;
				//重力の機能をオフにする
				Parent->GetComponent<CGravity>()->SetUse(false);			
			}
		}

		//攻撃する
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//プレイヤーの状態を攻撃に変更する
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}

		break;
#pragma endregion
	case HIT_PLAYER:
#pragma region ---ノックバック
		//フラグがtrueなら飛ばす方向に向けて力を加える
		if (!m_bHitFlg) {
			//敵にぶつかって吹き飛ばされた時の判定
			//ぶつかった方向とは逆向きに速度を加える
			//1:下から、2:上から、3:右から、4:左から
			switch (m_nHitVec)
			{
			case 1:
				//上方向に力を加える
				m_pPlayer->Vel.y = HIT_SPEED;
				break;
			case 2:
				//下方向に力を加える
				m_pPlayer->Vel.y = -HIT_SPEED;
				break;
			case 3:
				//左方向に力を加える
				m_pPlayer->Vel.x = -HIT_SPEED;
				break;
			case 4:
				//右方向に力を加える
				m_pPlayer->Vel.x = HIT_SPEED;
				break;

			default:break;
			}	
			//力を減らしていく処理に切り替える
			m_bHitFlg = true;
		}
		//飛ばした方向とは逆方向に力を加えて速度を落としていく
		else if (m_bHitFlg) {
			switch (m_nHitVec) {
			case 1:
				m_pPlayer->Vel.y -= 0.1;
				if (m_pPlayer->Vel.y <= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			case 2:
				m_pPlayer->Vel.y += KNOCK_BACK_PLAYER;
				if (m_pPlayer->Vel.y >= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			case 3:
				m_pPlayer->Vel.x += KNOCK_BACK_PLAYER*5;
				if (m_pPlayer->Vel.x >= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
					//プレイヤーがパワーアップ状態ならパワーダウンさせる
					if (m_bPawer_UP) {
						//パワーアップ状態を解除する
						m_bPawer_UP = false;					
					}
					else {
						//ミス状態にする
						m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);
						m_ePlayer = MISS_PLAYER;
						ChangeTexture();
					}
					
				}
				break;
			case 4:
				m_pPlayer->Vel.x -= KNOCK_BACK_PLAYER;
				if (m_pPlayer->Vel.x <= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			default:break;
			}
		}
		
		break;
#pragma endregion
	case MISS_PLAYER:
#pragma region ---ミス
		//プレイヤー以外のオブジェクトをストップする
		ObjectManager::GetInstance()->NoFunction();
		Parent->Use();
		for (auto&& block:ObjectManager::GetInstance()->GetUpdateList()) {
			if (block->GetName() == BLOCK_NAME) {
				block->Use();
			}
		}

		for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
			if (block->GetName() == ENEMY_NAME) {
				block->StopUpdate();
			}
		}

		//落下死か敵により死亡か変更する
		//trueで落下死
		if (m_bAirDead) {
			//落下死
			//じかんがある程度たったらゲームオーバーのメニューを出現させる
			MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

			//プレイヤーの更新を止める
			Parent->StopUpdate();

			//シーンゲームのオプションフラグをオンにする
			SceneGame::GetInstance()->SetPauseOOO(true);
		}
		else {
			//敵により死亡			
			//ミスカウンターを増やし一定でゲームオーバーメニューを出す
			m_nMissCnt++;
			if (m_nMissCnt > 90) {
				//じかんがある程度たったらゲームオーバーのメニューを出現させる
				//テクスチャが流し終わったら
				MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

				//プレイヤーの更新を止める
				Parent->StopUpdate();

				//シーンゲームのオプションフラグをオンにする
				SceneGame::GetInstance()->SetPauseOOO(true);
			}
		}
#pragma endregion
	default: break;
	}

	//攻撃のクールタイムはマイナスしておく
	if (m_nCoolTime > 0) {
		m_nCoolTime--;
	}
	//ダッシュのクールタイムもマイナスしておく
	if (m_nDushCoolTime > 0) {
		m_nDushCoolTime--;
	}

	//プレイヤーの最新状態を保存する
	g_ePlayer = m_ePlayer;

	//速度の上限を決める
	if (m_pPlayer->Vel.x > VALUE_MOVE_SPEED * 2) {
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 2;
	}
	if (m_pPlayer->Vel.x < -VALUE_MOVE_SPEED * 2) {
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 2;
	}
	if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
		m_pPlayer->Vel.y = -MAX_VELOCITY;
	}

	/*if (m_pPlayer->Vel.y > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}*/
#if _DEBUG
	if (m_pInput->GetKeyPress(DIK_UPARROW)) {
		//上移動
		m_pPlayer->Pos.y += 5.0f;
	}	
	if (m_pInput->GetKeyPress(DIK_DOWNARROW)) {
		//下移動
		m_pPlayer->Pos.y += -5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_RIGHTARROW)) {
		//右移動
		m_pPlayer->Pos.x += 5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_LEFTARROW)) {
		//右移動
		m_pPlayer->Pos.x += -5.0f;
	}

	//テクスチャの変更
	if (m_pInput->GetKeyTrigger(DIK_1)) {
		//デフォルトたち
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
		m_pDraw2D->SetAnimSplit(3,3);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y,DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(0.0f,0.0f);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(),m_pCollider->GetOffSet());
	}
	if (m_pInput->GetKeyTrigger(DIK_2)) {
		//デフォルト走り
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_RUN_Y);
		m_pDraw2D->SetAnimSplit(7,2);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_RUN_X, DXCHAN_COLL_SIZE_Y,DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(DXCHAN_COLL_OFFSET_RUN_X, DXCHAN_COLL_OFFSET_RUN_Y);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(),m_pCollider->GetOffSet());
	}
	if (m_pInput->GetKeyTrigger(DIK_P)) {
		g_ePlayer = IDLE_PLAYER;
		m_nHitVec = 0;
		m_bPawer_UP = true;
	}

	//重力の有無
	if (m_pInput->GetKeyTrigger(DIK_G)) {
		Parent->GetComponent<CGravity>()->m_bUpdateFlag = false;
	}
	if (m_pInput->GetKeyTrigger(DIK_H)) {
		Parent->GetComponent<CGravity>()->m_bUpdateFlag = true;
	}
#endif
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
	Text("DCool   : %d",m_nDushCoolTime);
	Text("ADFlg   : %d",m_bAirDushFlg);
	End();
#endif // _DEBUG
}

/**
* @fn		CPlayer::OnCollisionEnter
* @brief	プレイヤーが何かに当たったときの処理
* @param	(Object*)	相手方のオブジェクトのポインタ
*/
void CPlayer::OnCollisionEnter(Object* pObject) {
	//消す
	bHitObj = true;
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

				//敵の攻撃にヒットしている場合状態は変えない
				if (m_ePlayer == HIT_PLAYER||m_ePlayer == MISS_PLAYER) {
				
				}
				else {
					//プレイヤーの状態を地面に立っている状態に変える
					m_ePlayer = IDLE_PLAYER;				
				}
				
				//プレイヤーの状態がミスになっている場合ゲームオーバーにする
				if (m_ePlayer == MISS_PLAYER) {
					//プレイヤー以外のオブジェクトをストップする
					ObjectManager::GetInstance()->NoFunction();
					Parent->Use();
					for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
						if (block->GetName() == BLOCK_NAME) {
							block->Use();
						}
					}

					for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
						if (block->GetName() == ENEMY_NAME) {
							block->StopUpdate();
						}
					}

					m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);

					//じかんがある程度たったらゲームオーバーのメニューを出現させる
					//テクスチャが流し終わったら
					MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

					//プレイヤーの更新を止める
					Parent->StopUpdate();

					//シーンゲームのオプションフラグをオンにする
					SceneGame::GetInstance()->SetPauseOOO(true);

				}

				//プレイヤーの最新状態を保存する
				if (m_bJump == true) {
					g_ePlayer = m_ePlayer;
					m_bJump = false;
					m_bAirDushFlg = false;
					Parent->GetComponent<CGravity>()->SetUse(true);
					ChangeTexture();
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
		//if (m_pPlayer->Vel.y == -MAX_VELOCITY) {
		//	int i = 1;
		//}
		//else {
		//	int i = 1;
		//}

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
	}
#pragma endregion

#pragma region ---ENEMY
	//無敵時間の場合敵とは当たらない
	if (m_nStar_Time <= 0) {
		//敵と分類されるものに当たった場合
		if (pObject->GetName() == ENEMY_NAME) {
			//パワーアップ状態かそうでないかで処理内容を決める
			if (m_bPawer_UP) {
				//吹き飛ばす向きを決める
				m_nHitVec = CollEnemy(pObject);

				if (m_nHitVec) {
					////パワーアップしているということは強くなっているということである
					//無敵カウントを増やす
					m_nStar_Time = STAR_TIME;	
					//プレイヤーのステータスを変える
					//テクスチャも変える
					m_ePlayer = HIT_PLAYER;
					g_ePlayer = m_ePlayer;
					ChangeTexture();
				}
			}
			else {
				//当たったらミス状態である
				if (CollEnemy(pObject)) {
					//吹き飛ばす向きを決める
					m_nHitVec = CollEnemy(pObject);
					m_ePlayer = HIT_PLAYER;
					g_ePlayer = m_ePlayer;
					//無敵カウントを増やす
					m_nStar_Time = 999;
					//テクスチャを変える
					ChangeTexture();
				}
			}

			//やりたいこと
			//パワーアップ状態で無かったらミス判定

			//ミス判定　プレイヤー以外のオブジェクトの更新を止めてミスのテクスチャを流す
			//テクスチャを流し終えたらメニュー画面の表示(ゲームオーバーメニュー)
			
		
		
		}
	}
	
#pragma endregion

#pragma region ---ミス判定
	if (pObject->GetName() == "MISS_COLL") {
		//やらないといけないこと
		//ミス用のメニューを作成する
		m_ePlayer = MISS_PLAYER;
		g_ePlayer = m_ePlayer;
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
* @fn		CPlayer::CollEnemy
* @brief	敵とどの向きから当たったかで吹き飛ばす方向を決める
* @detail	1:下から、2:上から、3:右から、4:左から
* @param	(Object*)	当たった敵の情報がはいいているポインタ
* @return	(int)		どっちからあったかを返す
*/
int CPlayer::CollEnemy(Object* pObject) {
#pragma region ---上下左右4方向からの当たり判定
	////プレイヤーの情報を取得
	//auto Player = Parent->GetComponent<CCollider>();
	//auto PlayerPos = Player->GetCenterPos();
	//auto PlayerSize = Player->GetColliderSize();
	//auto PlayerOffset = Player->GetOffSet();

	////ぶつかったブロックの情報
	//auto Enemy = pObject->GetComponent<CCollider>();
	//auto EnemyPos = Enemy->GetCenterPos();
	//auto EnemySize = Enemy->GetColliderSize();
	//auto EnemyOffset = Enemy->GetOffSet();

	////それぞれ半分の大きさを保存
	//XMFLOAT2 PlayerHalhSize = XMFLOAT2(PlayerSize.x * 0.5f, PlayerSize.y *0.5f);
	//XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y * 0.5f);

	////ブロックの当たり判定の線
	//float EnemyLeftLine = EnemyPos.x - EnemyHalhSize.x;	//左端
	//float EnemyRightLine = EnemyPos.x + EnemyHalhSize.x;	//右端
	//float EnemyUpLine = EnemyPos.y + EnemyHalhSize.y;	//上端
	//float EnemyDownLine = EnemyPos.y - EnemyHalhSize.y;	//下端

	///** @brief プレイヤーがブロックの中(上端と下端の中)にいた場合の処理*/
	//if ((EnemyDownLine < PlayerPos.y && PlayerPos.y < EnemyUpLine) ||
	//	(EnemyDownLine < PlayerPos.y - PlayerHalhSize.y && PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) ||
	//	(EnemyDownLine < PlayerPos.y + PlayerHalhSize.y && PlayerPos.y + PlayerHalhSize.y < EnemyUpLine)) {
	//	//プレイヤーが右のブロックに当たった場合
	//	if (EnemyLeftLine >= m_OldPos.x + PlayerHalhSize.x &&		//前フレームはめり込んでいない
	//		PlayerPos.x + PlayerHalhSize.x > EnemyLeftLine) {		//現在のフレームはめり込んでいる

	//		return 3;
	//	}
	//	else if (EnemyRightLine <= m_OldPos.x - PlayerHalhSize.x &&
	//		PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) {

	//		return 4;
	//	}
	//}

	////中心座標のセット
	//PlayerPos = Player->GetCenterPos();

	///**
	//* @brief プレイヤーがブロックの中(左端と右端の中)にいた場合の処理
	//*/
	//if ((EnemyLeftLine < PlayerPos.x && PlayerPos.x < EnemyRightLine) ||
	//	(EnemyLeftLine < PlayerPos.x - PlayerHalhSize.x && PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) ||
	//	(EnemyLeftLine < PlayerPos.x + PlayerHalhSize.x && PlayerPos.x + PlayerHalhSize.x < EnemyRightLine)) {

	//	/** @brief プレイヤーが上に乗ったら*/
	//	if (EnemyUpLine <= m_OldPos.y - PlayerHalhSize.y &&					//前フレームはめり込んでいない
	//		PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) {					//現在のフレームはめり込んでいる

	//		return 1;
	//	}
	//	/** @brief 頭ごっつんこ状態である*/
	//	else if (EnemyDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//前フレームはめり込んでいない
	//		PlayerPos.y + PlayerHalhSize.y > EnemyDownLine) {		//現在のフレームはめり込んでいる

	//		return 2;
	//	}
	//}

	//return 0;
#pragma endregion

#pragma region ---ボックスでの当たり判定
	//プレイヤーの情報を取得
	auto Player = Parent->GetComponent<CCollider>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffset = Player->GetOffSet();

	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);
	//当たり判定の大きさを持ってくる
	auto Enemy = pObject->GetComponent<CCollider>();
	auto TEnemy = pObject->GetComponent<CTransform>();
	XMFLOAT2 EnemyHalfSize = XMFLOAT2(Enemy->GetColliderSize().x / 2.0f,Enemy->GetColliderSize().y / 2.0f);

	if (Enemy->GetCenterPos().x - EnemyHalfSize.x + TEnemy->Vel.x < m_pCollider->GetCenterPos().x + PlayerHalfSize.x + m_pPlayer->Vel.x &&
		m_pCollider->GetCenterPos().x - PlayerHalfSize.x + m_pPlayer->Vel.x < Enemy->GetCenterPos().x + EnemyHalfSize.x + TEnemy->Vel.x) {

		if (Enemy->GetCenterPos().y - EnemyHalfSize.y + TEnemy->Vel.y < m_pCollider->GetCenterPos().y + PlayerHalfSize.y + m_pPlayer->Vel.y &&
			m_pCollider->GetCenterPos().y - PlayerHalfSize.y + m_pPlayer->Vel.y < Enemy->GetCenterPos().y + EnemyHalfSize.y + TEnemy->Vel.y) {

			return 3;
		}
	}
	return 0;
#pragma endregion
}

/**
* @fn		CPlayer::ChangeTexture
* @brief	プレイヤーのステータスが変わったとき、テクスチャを変える関数
*/
void CPlayer::ChangeTexture() {
	//どのテクスチャに変更するかはプレイヤーの状態を見て決める
	switch (m_ePlayer) {
	case IDLE_PLAYER:
		//デフォルトたち
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
		m_pDraw2D->SetAnimSplit(3, 3);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(0.0f, 0.0f);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(), m_pCollider->GetOffSet());
		break;
	case RUN_PLAYER: 
		//左右でかえましょうね　
		//デフォルト走り
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_RUN_Y);
		m_pDraw2D->SetAnimSplit(7, 2);
		m_pDraw2D->SetVertex(m_bROL);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_RUN_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(DXCHAN_COLL_OFFSET_RUN_X, DXCHAN_COLL_OFFSET_RUN_Y);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(), m_pCollider->GetOffSet());
		break;
	case DUSH_PLAYER:m_ePlayer = m_ePlayer; break;
	case JUMP_PLAYER:m_ePlayer = m_ePlayer; break;
	case FALL_PLAYER:m_ePlayer = m_ePlayer; break;
	case HIT_PLAYER:
		break;
	case MISS_PLAYER:break;
	default:break;
	}	
}

/**
* @fn		CPlayer::CreateAttack
* @brief	攻撃判定用のオブジェクトを生成する
*/
void CPlayer::CreateAttack() {
	//オブジェクトの生成
	Object* obj = new Object(ATTACK_NAME,UPDATE_PLAYER,DRAW_PLAYER);
	//コンポーネントの追加
	auto trans = obj->AddComponent<CTransform>();
	auto draw = obj->AddComponent<CDrawMesh>();
	auto coll = obj->AddComponent<CCollider>();
	auto attack = obj->AddComponent<CAttack>();
	obj->AddComponent<CSeeColl>();
	//オブジェクトの設定　
	draw->SetTexture(TextureManager::GetInstance()->GetTexture(DEBUG_BLOCK_NUM));
	//誰の攻撃かを設定する
	attack->SetAttackType(PLAYER_ATTACK);
	attack->SetObject(ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME));
	//左右で作り出す場所を変更する
	if (m_bROL) {
		trans->SetPosition(m_pPlayer->Pos.x+ OFFSET_DX_X,m_pPlayer->Pos.y + OFFSET_DX_Y);	
	}
	else {
		trans->SetPosition(m_pPlayer->Pos.x - OFFSET_DX_X, m_pPlayer->Pos.y + OFFSET_DX_Y);
	}
	attack->SetRoL(m_bROL);

	coll->SetCollisionSize(ATTACK_COLL_X, ATTACK_COLL_Y);
	//オブジェクトマネージャーに追加
	ObjectManager::GetInstance()->AddObject(obj);

	//攻撃オブジェクトを指して置けるポインタで指しておく
	m_pAttackObj = obj;
}

/**
* @fn		CPlayer::GetPlayerROL
* @brief	プレイヤーが左右どちらを向いているかを取得する関数
* @return	(bool)	右ならばtrue、左ならfalseを返す
*/
bool CPlayer::GetPlayerROL() {
	return m_bROL;
}

/**
* @fn		CPlayer::SetCoolTime
* @brief	攻撃後のクールタイムの設定
* @param	(int)	どの位たったら次の攻撃に移れるか
*/
void CPlayer::SetCoolTime(int Cooltime) {
	m_nCoolTime = Cooltime;
}

/**
* @fn		CPlayer::SetAttackFlg
* @brief	攻撃が再びできるようにするフラグ
* @param	(bool)	攻撃していいよ
*/
void CPlayer::SetAttackFlg(bool Attack) {
	m_bAttack = Attack;
}

/** 
* @fn		CPlayer::GetPlayerJump
* @brief	ジャンプしているかを取得する
* @return	(bool)	
*/
bool CPlayer::GetPlayerJump() {
	return m_bJump;
}

/**
* @fn		CPlayer::GetPlayerSta
* @brief	プレイヤーの状態を取得する
* @return	(PLAYER_STATE)	現在のプレイヤーの状態を返す
*/
PLAYER_STATE CPlayer::GetPlayerSta() {
	return g_ePlayer;
}