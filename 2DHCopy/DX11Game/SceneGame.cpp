//インクルード部
#include "sceneGame.h"
#include "SceneSelect.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "FileManager.h"
#include "StageManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "Camera.h"
#include "imgui.h"
#include "Sound.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TexScrollComponent.h"
#include "Draw3dComponent.h"
#include "PlayerComponent.h"
#include "SeeCollComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "GravityComponent.h"
#include "ScoreComponent.h"
#include "TimeComponent.h"

#include "Load.h"

//ステージ関係のインクルード
#include "SceneStage_1.h"
#include "SceneStage_1_Re.h"

//静的メンバ変数
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	コンストラクタ
* @detail	追加したいステージ等はここで追加していく
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) 
	,m_pStageManager(nullptr){

	//ここでステージをリストに追加していく
	m_GameSceneList.push_back(SceneStage_1::GetInstance());
	m_GameSceneList.push_back(SceneStage_1_Re::GetInstance());

	//最初のステージの設定
	m_eStageState = STAGE_1;
	m_CurrentScene = m_GameSceneList[m_eStageState];
}

/**
* @fn		SceneGame::~SceneGame
* @brief	デストラクタでステージをぶっこわーす
*/
SceneGame::~SceneGame() {
	//シーンリストを削除する
	for (auto scene : m_GameSceneList) {
		delete scene;
	}

	//リストを空にする
	m_GameSceneList.clear();

}

/** 
* @fn		SceneGame::Init
* @brief	初期化をする
*/
void SceneGame::Init() {
	//ロード画面のスレッドを作る
	Load::Begin();

	//裏か表を判別する為の変数の設定
	switch (m_eStageState)
	{
	case STAGE_1:
		m_bLoveandLovers = true;
		break;

	case STAGE_1_RE:
		m_bLoveandLovers = false;
		break;
	default:break;
	}

	//カメラを初期化する
	CCamera::Get()->Init();

	//編集モードとポーズフラグをオフにする
	m_bEditMode = false;
	m_bPauseMode = false;

	//ステージセレクト画面に戻す時間
	m_nSelectCnt = 0;

	//スコアを0にする
	m_nScore = 0;
	//タイマーを500秒にする
	m_nTimer = STAGE_TIME;

	//ゲームオブジェクトの初期化

	//管理クラスインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();
	m_pStageManager = StageManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	pModelManager->AddModel(PATH_WALK_ENEMY, WALK_ENEMY_MODEL_NUM);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);
	pModelManager->AddModel(PATH_RARD_BLOCK, RARD_BLOCK_NUM);
	pModelManager->AddModel(PATH_GRASS_RE_BLOCK, GRASS_BLOCK_RE_NUM);
	pModelManager->AddModel(PATH_RARD_RE_BLOCK, RARD_BLOCK_RE_NUM);
	pModelManager->AddModel(PATH_KOBAN,KOBAN_MODEL_NUM);
	pModelManager->AddModel(PATH_MAGA,MAGA_MODEL_NUM);
	pModelManager->AddModel(PATH_OHUDA, OHUDA_MODEL_NUM);
	//テクスチャの読込
	TextureManager* pTexManager = TextureManager::GetInstance();

	//仮
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_JUMP, DXCHAN_JUMP_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCAHN_FALL, DXCHAN_FALL_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);
	pTexManager->AddTexture(PATH_TEX_FAID_OUT, FEAD_OUT_NUM);
	//UIの表示
	pTexManager->AddTexture(PATH_TEX_SCORE, SCORE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_MAX_SCORE, MAX_SCORE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_NUMBER, NUMBER_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_PAUSE, PAUSE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TIME, TIME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_RESUME, RESUME_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_OPTION, OPTION_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_STAGE_SELECT, STAGE_SELECT_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_MOSAIC, MOSAIC_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CIRCLE, CIRCLE_TEX_NUM);

	//矢印
	pTexManager->AddTexture(PATH_ARROW, ARROW_NUM);

	//オブジェクトの作成
	//プレイヤー
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	Box->AddComponent<CGravity>();
	Box->AddComponent<CSeeColl>();


	//設定仮
	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	TransBox->SetPosition(-250.0f, 100.0f);
	CollBox->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
	CollBox->SetOffset(0.0f, 0.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
	DrawBox->SetUVsize(XMFLOAT2(-1.0f,1.0f));
	DrawBox->SetAnimSplit(3,3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);	
	//ステージごとの設定をする
	m_CurrentScene->Init();

	//UI関係の表示
	//スコア
	Object* Score = new Object(UI_SCORE_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Score = Score->AddComponent<CTransform>();
	auto D_Score = Score->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Score->SetPosition(UI_SCORE_POS_X, UI_SCORE_POS_Y);
	D_Score->SetTexture(TextureManager::GetInstance()->GetTexture(SCORE_TEX_NUM));
	D_Score->SetSize(UI_SCORE_SIZE_X, UI_SCORE_SIZE_Y);
	//追加
	m_pObjectManager->AddObject(Score);

	//スコア(数字)
	for (int i = 0;i < MAX_SCORE;i++) {
		Object* Score_Num = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントの追加
		auto T_Score_Num = Score_Num->AddComponent<CTransform>();
		auto D_Score_Num = Score_Num->AddComponent<CDraw2D>();
		auto S_Score_NUm = Score_Num->AddComponent<CScore>();
		//オブジェクトの設定
		T_Score_Num->SetPosition(UI_NUMBER_POS_X_SCORE + ((MAX_SCORE-1)-i) * UI_NUMBER_SIZE_X_SCORE, UI_NUMBER_POS_Y_SCORE);
		D_Score_Num->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Score_Num->SetSize(UI_NUMBER_SIZE_X_SCORE, UI_NUMBER_SIZE_Y_SCORE);
		D_Score_Num->SetAnimSplit(5, 2);
		D_Score_Num->SetAnimNum(i+1);
		S_Score_NUm->SetDigits(i);
		//追加
		m_pObjectManager->AddObject(Score_Num);
	}

	//ハイスコア
	Object* Max_Score = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
	//コンポーネントの追加
	auto T_MaxScore = Max_Score->AddComponent<CTransform>();
	auto D_MaxScore = Max_Score->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_MaxScore->SetPosition(UI_MAX_SCORE_POS_X, UI_MAX_SCORE_POS_Y);
	D_MaxScore->SetTexture(TextureManager::GetInstance()->GetTexture(MAX_SCORE_TEX_NUM));
	D_MaxScore->SetSize(UI_MAX_SCORE_SIZE_X, UI_MAX_SCORE_SIZE_Y);
	//追加
	m_pObjectManager->AddObject(Max_Score);
	

	//ハイスコア(数字)
	for (int i = 0; i < MAX_SCORE; i++) {
		Object* Score_Max = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントの追加
		auto T_Score_Max = Score_Max->AddComponent<CTransform>();
		auto D_Score_Max = Score_Max->AddComponent<CDraw2D>();
		//オブジェクトの設定
		T_Score_Max->SetPosition(UI_NUMBER_POS_X_MAX + ((MAX_SCORE - 1) - i) * UI_NUMBER_SIZE_X_MAX, UI_NUMBER_POS_Y_MAX);
		D_Score_Max->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Score_Max->SetSize(UI_NUMBER_SIZE_X_MAX, UI_NUMBER_SIZE_Y_MAX);
		D_Score_Max->SetAnimSplit(5, 2);
		D_Score_Max->SetAnimNum(0);
		//追加
		m_pObjectManager->AddObject(Score_Max);
	}

	//タイム
	Object* Timer = new Object(UI_TIMA_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Timer = Timer->AddComponent<CTransform>();
	auto D_Timer = Timer->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Timer->SetPosition(UI_TIME_POS_X, UI_TIME_POS_Y);
	D_Timer->SetTexture(TextureManager::GetInstance()->GetTexture(TIME_TEX_NUM));
	D_Timer->SetSize(UI_TIME_SIZE_X, UI_TIME_SIZE_Y);
	//追加
	m_pObjectManager->AddObject(Timer);

	//タイム(数字)
	for (int i = 0;i< 3;i++) {
		Object* Num_Timer = new Object(UI_NUMBER_NAME, UPDATE_UI, DRAW_UI);
		//コンポーネントの追加
		auto T_Timer_Num = Num_Timer->AddComponent<CTransform>();
		auto D_Timer_Num = Num_Timer->AddComponent<CDraw2D>();
		//オブジェクトの設定
		auto Timer_Num = Num_Timer->AddComponent<CTime>();
		T_Timer_Num->SetPosition(UI_NUNBER_POS_X_TIME + (2 - i) * UI_NUMBER_SIZE_X_TIME, UI_NUNBER_POS_Y_TIME);
		D_Timer_Num->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Timer_Num->SetSize(UI_NUMBER_SIZE_X_TIME, UI_NUMBER_SIZE_Y_TIME);
		D_Timer_Num->SetAnimSplit(5,2);
		D_Timer_Num->SetAnimNum(0);
		Timer_Num->SetDigits(i);
		//時間を変えるフラグをセット(1つだけtrue)
		if (i == 0) {
			Timer_Num->SetTime(true);
		}
		else {
			Timer_Num->SetTime(false);
		}

		//追加
		m_pObjectManager->AddObject(Num_Timer);
	}

	//ポーズ斡旋
	Object* Pause = new Object(UI_PAUSE_NAME,UPDATE_UI,DRAW_UI);
	//コンポーネントの追加
	auto T_Pause = Pause->AddComponent<CTransform>();
	auto D_Pause = Pause->AddComponent<CDraw2D>();
	//オブジェクトの設定
	T_Pause->SetPosition(UI_PAUSE_POS_X, UI_PAUSE_POS_Y);
	D_Pause->SetTexture(TextureManager::GetInstance()->GetTexture(PAUSE_TEX_NUM));
	D_Pause->SetSize(UI_PAUSE_SIZE_X, UI_PAUSE_SIZE_Y);
	//追加
	m_pObjectManager->AddObject(Pause);

	//ステージを作る
	m_pStageManager->CreateStage(m_eStageState);

	//ロードの終了
	Load::End();
}

/**
* @fn		SceneGame::Uninti
* @brief	終了処理
*/
void SceneGame::Uninit() {
	//オブジェクトの終了
	m_pObjectManager->Uninit();
}

/**
* @fn		SceneGame::Update
* @brief	更新処理
*/
void SceneGame::Update() {
	//カメラの更新処理
	CCamera::Get()->Update();

	//ステージ更新
	m_pStageManager->Update();

	//メニューの更新
	//m_pMenuManager->Update();

	//ポーズメニューを呼び出す
	if (InputManager::Instance()->GetKeyTrigger(DIK_Q)) {
		//ポーズメニューーを作ったり破棄したりする
		if (m_bPauseMode) {
			//オンオフをひっくり返す
			m_bPauseMode = false;

			//オプションのオブジェクトが存在したらオプションフラグをオフにする
			for (auto&& obj:m_pMenuManager->GetMenuList()) {
				if (obj->GetUpdateOrder() == UPDATE_OPTION) {
					m_pMenuManager->m_bOption = false;
					break;
				}
			}

			//メニューを破棄する
			m_pMenuManager->DeleteMenu();

			//オブジェクトを全て再開する
			for (auto&& obj : m_pObjectManager->GetUpdateList()) {
				obj->Use();
			}
			//カメラを再開する

		}
		else {
			//オンオフをひっくり返す
			m_bPauseMode = true;
			//メニューの作成
			m_pMenuManager->CreatePauseMenu();
			//オブジェクトを全て停止する
			for (auto&& obj:m_pObjectManager->GetUpdateList()) {
				obj->StopUpdate();
			}
			//カメラも停止させる

		}
		//m_bPauseMode != m_bPauseMode;
	}

	//ゲームモード
	if (m_bEditMode == false) {
		if (m_bPauseMode == false) {
			//ゲームオブジェクト更新
			m_pObjectManager->Update();

			//ステージごとの更新
			m_CurrentScene->Update();
		}
		else {		
			m_pMenuManager->Update();
		}

	}
	else if (m_bEditMode == true) {
		m_pObjectManager->UpdateEdit();
	}

	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		return;
	}

	//ここでプレイヤーの速度を見る
	//下向きの速度がかかっていた場合落下判定とする
	if (m_pObjectManager->GetGameObject(PLAYER_NAME)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		//プレイヤーの状態を見て既にジャンプか落下だったら変更しない
		if ((Player->GetComponent<CPlayer>()->GetPlayerSta() == FALL_PLAYER  ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == JUMP_PLAYER   ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == ATTACK_PLAYER ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == DUSH_PLAYER	 ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == MISS_PLAYER	 ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == HIT_PLAYER    ||
			Player->GetComponent<CPlayer>()->GetPlayerSta() == TIMEOUT_PLAYER)
			) {
		
		}
		else {
			if (Player->GetComponent<CTransform>()->Vel.y <= -MAX_VELOCITY) {
				Player->GetComponent<CPlayer>()->SetPlayerState(FALL_PLAYER);
				Player->GetComponent<CPlayer>()->ChangeTexture();
			}		
		}
	}

	//クリアした場合の処理
	auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
	if (Player->GetComponent<CPlayer>()->GetClearFlg()) {
		//セレクト画面に移動するまでの時間を計測
		m_nSelectCnt++;
		//2秒で変更する
		if (m_nSelectCnt > 120) {
			//次のステージを解放する
			//ステージごとに内容を派生させる
			switch (m_eStageState) {
			case STAGE_1:
				//ここに入ることは無いから入ったらエラー

				break;
			case STAGE_1_RE:
				StageSelect::SaveClearInfo(m_eStageState-1,SceneStage_1_Re::GetInstance()->GetOhuda());				
				break;

			default:break;
			}
			SceneManager::Instance()->SetScene(SCENE_SELECT);
			m_nSelectCnt = 0;
		}
	}

#ifdef _DEBUG
	//何かしらの処理をしたらステータスを変える
	if (InputManager::Instance()->GetKeyTrigger(DIK_P)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		Player->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
	}

	//	編集モードの切り替え
	if (InputManager::Instance()->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// 切り替え

		//CSound::Play(SE_PAUSE);	// ←これ追加

		//	切り替えと同時にカメラの注視点の変更
		if (m_bEditMode == true)	CCamera::Get()->SetAxisX(nullptr);	//edit	プレイヤーから注視店を外す
		else						CCamera::Get()->SetAxisX(&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);	//game	プレイヤーを注視店に設定
	}

	//シーンのやり直し
	if (InputManager::Instance()->GetKeyTrigger(DIK_Z)) {
		//シーン遷移
		SceneManager::Instance()->SetScene(SCENE_TITLE);
	}
#endif

}

/** 
* @fn		SceneGame::Draw
* @brief	描画する関数
* @detail	教室が寒すぎる、上着を忘れるこれ即ち死を示す
*/
void SceneGame::Draw() {
	//ゲームオブジェクトの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

	//ポーズ画面描画
	if (m_bPauseMode) {
		m_pMenuManager->Draw();
	}

#ifdef _DEBUG
	//CCamera::Get()->Draw();

	using namespace ImGui;

	Begin("Camera");
	Text("Pos : %3.0f,%3.0f,%3.0f",CCamera::Get()->GetPos().x, CCamera::Get()->GetPos().y, CCamera::Get()->GetPos().z);
	End();

#endif
}

/**
* @fn		SceneGame::SetStage
* @brief	ステージの切り替え
* @param	(Stage)	次のステージ番号
*/
void SceneGame::SetStage(Stage NextStage) {
	//例外処理
	if (NextStage >= MAX_STAGE) {
		MessageBox(GetMainWnd(), _T("ステージ切り替え失敗"), NULL, MB_OK);
		return;
	}

	//現在のステージの終了
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//ステージの切替
	m_CurrentScene = m_GameSceneList[NextStage];
	m_eStageState = NextStage;

}

/** 
* @fn		SceneGame::GetStage	
* @brief	ステージの情報を取得する
* @return	(Stage)	現在のステージ番号を返す
*/
Stage SceneGame::GetStage() {
	return m_eStageState;
}

/**
* @fn		SceneGame::GetInstance
* @brief	インスタンス取得
* @return	(SceneGame*)	インスタンスを返す
*/
SceneGame* SceneGame::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneGame();
	}
	return m_pInstance;
}

/**
* @fn		SceneGame::Destory
* @brief	インスタンス破棄
*/
void SceneGame::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/** 
* @fn		SceneGame::SetPauseOOO
* @brief	ポーズのオンオフを外部から決める
* @param	(bool)	ポーズメニューを更新するかどうか
*/
void SceneGame::SetPauseOOO(bool ooo) {
	m_bPauseMode = ooo;
}

/**
* @fn		SceneGame::GetScore
* @brief	スコアを取得する
* @return	(int)	ゲームシーンが管理しているスコア
*/
int SceneGame::GetScore() {
	return m_nScore;
}

/**
* @fn		SceneGame::SetScore
* @brief	スコアを加算
* @param	(int)	加算したい数値
*/
void SceneGame::SetScore(int num) {
	m_nScore += num;
}

/**
* @fn		SceneGame::GetTime
* @brief	タイムの取得
* @return	(int)	残り時間
*/
int SceneGame::GetTime() {
	return m_nTimer;
}

/**
* @fn		SceneGame::SetTime
* @brief	残り時間の設定
* @param	(int)	変えたい数値に変える
*/
void SceneGame::SetTime() {
	m_nTimer--;
}

/**
* @fn		SceneGame::GetCurrentStage
* @brief	ステージの情報が入ったベクターを返す
* @return	(vector<Scene*>)	リスト
*/
std::vector<Scene*> SceneGame::GetCurrentStage() {
	return m_GameSceneList;
}

/** 
* @fn		SceneGame::GetLaL
* @brief	裏か表かを取得する関数
* @return	(bool)	trueで表、falseで裏
*/
bool SceneGame::GetLaL() {
	return m_bLoveandLovers;
}

/**
* @fn		SceneGame::SetLaL
* @brief	ステージ途中で裏表の逆転があったら使う
* @param	(bool)	変化させたい方(trueで表 falseで裏)
*/
void SceneGame::SetLaL(bool lal) {
	m_bLoveandLovers = lal;
}

/**
* @fn		SceneGame::GetStageCurrent
* @brief	ステージごとの関数を使えるようにする
* @return	(Scene*)	現在のステージの取得
*/
Scene* SceneGame::GetStageCurrent() {
	return m_CurrentScene;
}
