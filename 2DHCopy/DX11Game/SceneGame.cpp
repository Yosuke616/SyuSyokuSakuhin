//�C���N���[�h��
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

//�X�e�[�W�֌W�̃C���N���[�h
#include "SceneStage_1.h"
#include "SceneStage_1_Re.h"

//�ÓI�����o�ϐ�
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	�R���X�g���N�^
* @detail	�ǉ��������X�e�[�W���͂����Œǉ����Ă���
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) 
	,m_pStageManager(nullptr){

	//�����ŃX�e�[�W�����X�g�ɒǉ����Ă���
	m_GameSceneList.push_back(SceneStage_1::GetInstance());
	m_GameSceneList.push_back(SceneStage_1_Re::GetInstance());

	//�ŏ��̃X�e�[�W�̐ݒ�
	m_eStageState = STAGE_1;
	m_CurrentScene = m_GameSceneList[m_eStageState];
}

/**
* @fn		SceneGame::~SceneGame
* @brief	�f�X�g���N�^�ŃX�e�[�W���Ԃ�����[��
*/
SceneGame::~SceneGame() {
	//�V�[�����X�g���폜����
	for (auto scene : m_GameSceneList) {
		delete scene;
	}

	//���X�g����ɂ���
	m_GameSceneList.clear();

}

/** 
* @fn		SceneGame::Init
* @brief	������������
*/
void SceneGame::Init() {
	//���[�h��ʂ̃X���b�h�����
	Load::Begin();

	//�����\�𔻕ʂ���ׂ̕ϐ��̐ݒ�
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

	//�J����������������
	CCamera::Get()->Init();

	//�ҏW���[�h�ƃ|�[�Y�t���O���I�t�ɂ���
	m_bEditMode = false;
	m_bPauseMode = false;

	//�X�e�[�W�Z���N�g��ʂɖ߂�����
	m_nSelectCnt = 0;

	//�X�R�A��0�ɂ���
	m_nScore = 0;
	//�^�C�}�[��500�b�ɂ���
	m_nTimer = STAGE_TIME;

	//�Q�[���I�u�W�F�N�g�̏�����

	//�Ǘ��N���X�C���X�^���X�擾
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
	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	//��
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_JUMP, DXCHAN_JUMP_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCAHN_FALL, DXCHAN_FALL_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);
	pTexManager->AddTexture(PATH_TEX_FAID_OUT, FEAD_OUT_NUM);
	//UI�̕\��
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

	//���
	pTexManager->AddTexture(PATH_ARROW, ARROW_NUM);

	//�I�u�W�F�N�g�̍쐬
	//�v���C���[
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	Box->AddComponent<CGravity>();
	Box->AddComponent<CSeeColl>();


	//�ݒ艼
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
	//�X�e�[�W���Ƃ̐ݒ������
	m_CurrentScene->Init();

	//UI�֌W�̕\��
	//�X�R�A
	Object* Score = new Object(UI_SCORE_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Score = Score->AddComponent<CTransform>();
	auto D_Score = Score->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Score->SetPosition(UI_SCORE_POS_X, UI_SCORE_POS_Y);
	D_Score->SetTexture(TextureManager::GetInstance()->GetTexture(SCORE_TEX_NUM));
	D_Score->SetSize(UI_SCORE_SIZE_X, UI_SCORE_SIZE_Y);
	//�ǉ�
	m_pObjectManager->AddObject(Score);

	//�X�R�A(����)
	for (int i = 0;i < MAX_SCORE;i++) {
		Object* Score_Num = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g�̒ǉ�
		auto T_Score_Num = Score_Num->AddComponent<CTransform>();
		auto D_Score_Num = Score_Num->AddComponent<CDraw2D>();
		auto S_Score_NUm = Score_Num->AddComponent<CScore>();
		//�I�u�W�F�N�g�̐ݒ�
		T_Score_Num->SetPosition(UI_NUMBER_POS_X_SCORE + ((MAX_SCORE-1)-i) * UI_NUMBER_SIZE_X_SCORE, UI_NUMBER_POS_Y_SCORE);
		D_Score_Num->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Score_Num->SetSize(UI_NUMBER_SIZE_X_SCORE, UI_NUMBER_SIZE_Y_SCORE);
		D_Score_Num->SetAnimSplit(5, 2);
		D_Score_Num->SetAnimNum(i+1);
		S_Score_NUm->SetDigits(i);
		//�ǉ�
		m_pObjectManager->AddObject(Score_Num);
	}

	//�n�C�X�R�A
	Object* Max_Score = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_MaxScore = Max_Score->AddComponent<CTransform>();
	auto D_MaxScore = Max_Score->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_MaxScore->SetPosition(UI_MAX_SCORE_POS_X, UI_MAX_SCORE_POS_Y);
	D_MaxScore->SetTexture(TextureManager::GetInstance()->GetTexture(MAX_SCORE_TEX_NUM));
	D_MaxScore->SetSize(UI_MAX_SCORE_SIZE_X, UI_MAX_SCORE_SIZE_Y);
	//�ǉ�
	m_pObjectManager->AddObject(Max_Score);
	

	//�n�C�X�R�A(����)
	for (int i = 0; i < MAX_SCORE; i++) {
		Object* Score_Max = new Object(UI_MAX_SCORE_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g�̒ǉ�
		auto T_Score_Max = Score_Max->AddComponent<CTransform>();
		auto D_Score_Max = Score_Max->AddComponent<CDraw2D>();
		//�I�u�W�F�N�g�̐ݒ�
		T_Score_Max->SetPosition(UI_NUMBER_POS_X_MAX + ((MAX_SCORE - 1) - i) * UI_NUMBER_SIZE_X_MAX, UI_NUMBER_POS_Y_MAX);
		D_Score_Max->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Score_Max->SetSize(UI_NUMBER_SIZE_X_MAX, UI_NUMBER_SIZE_Y_MAX);
		D_Score_Max->SetAnimSplit(5, 2);
		D_Score_Max->SetAnimNum(0);
		//�ǉ�
		m_pObjectManager->AddObject(Score_Max);
	}

	//�^�C��
	Object* Timer = new Object(UI_TIMA_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Timer = Timer->AddComponent<CTransform>();
	auto D_Timer = Timer->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Timer->SetPosition(UI_TIME_POS_X, UI_TIME_POS_Y);
	D_Timer->SetTexture(TextureManager::GetInstance()->GetTexture(TIME_TEX_NUM));
	D_Timer->SetSize(UI_TIME_SIZE_X, UI_TIME_SIZE_Y);
	//�ǉ�
	m_pObjectManager->AddObject(Timer);

	//�^�C��(����)
	for (int i = 0;i< 3;i++) {
		Object* Num_Timer = new Object(UI_NUMBER_NAME, UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g�̒ǉ�
		auto T_Timer_Num = Num_Timer->AddComponent<CTransform>();
		auto D_Timer_Num = Num_Timer->AddComponent<CDraw2D>();
		//�I�u�W�F�N�g�̐ݒ�
		auto Timer_Num = Num_Timer->AddComponent<CTime>();
		T_Timer_Num->SetPosition(UI_NUNBER_POS_X_TIME + (2 - i) * UI_NUMBER_SIZE_X_TIME, UI_NUNBER_POS_Y_TIME);
		D_Timer_Num->SetTexture(TextureManager::GetInstance()->GetTexture(NUMBER_TEX_NUM));
		D_Timer_Num->SetSize(UI_NUMBER_SIZE_X_TIME, UI_NUMBER_SIZE_Y_TIME);
		D_Timer_Num->SetAnimSplit(5,2);
		D_Timer_Num->SetAnimNum(0);
		Timer_Num->SetDigits(i);
		//���Ԃ�ς���t���O���Z�b�g(1����true)
		if (i == 0) {
			Timer_Num->SetTime(true);
		}
		else {
			Timer_Num->SetTime(false);
		}

		//�ǉ�
		m_pObjectManager->AddObject(Num_Timer);
	}

	//�|�[�Y����
	Object* Pause = new Object(UI_PAUSE_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Pause = Pause->AddComponent<CTransform>();
	auto D_Pause = Pause->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Pause->SetPosition(UI_PAUSE_POS_X, UI_PAUSE_POS_Y);
	D_Pause->SetTexture(TextureManager::GetInstance()->GetTexture(PAUSE_TEX_NUM));
	D_Pause->SetSize(UI_PAUSE_SIZE_X, UI_PAUSE_SIZE_Y);
	//�ǉ�
	m_pObjectManager->AddObject(Pause);

	//�X�e�[�W�����
	m_pStageManager->CreateStage(m_eStageState);

	//���[�h�̏I��
	Load::End();
}

/**
* @fn		SceneGame::Uninti
* @brief	�I������
*/
void SceneGame::Uninit() {
	//�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();
}

/**
* @fn		SceneGame::Update
* @brief	�X�V����
*/
void SceneGame::Update() {
	//�J�����̍X�V����
	CCamera::Get()->Update();

	//�X�e�[�W�X�V
	m_pStageManager->Update();

	//���j���[�̍X�V
	//m_pMenuManager->Update();

	//�|�[�Y���j���[���Ăяo��
	if (InputManager::Instance()->GetKeyTrigger(DIK_Q)) {
		//�|�[�Y���j���[�[���������j�������肷��
		if (m_bPauseMode) {
			//�I���I�t���Ђ�����Ԃ�
			m_bPauseMode = false;

			//�I�v�V�����̃I�u�W�F�N�g�����݂�����I�v�V�����t���O���I�t�ɂ���
			for (auto&& obj:m_pMenuManager->GetMenuList()) {
				if (obj->GetUpdateOrder() == UPDATE_OPTION) {
					m_pMenuManager->m_bOption = false;
					break;
				}
			}

			//���j���[��j������
			m_pMenuManager->DeleteMenu();

			//�I�u�W�F�N�g��S�čĊJ����
			for (auto&& obj : m_pObjectManager->GetUpdateList()) {
				obj->Use();
			}
			//�J�������ĊJ����

		}
		else {
			//�I���I�t���Ђ�����Ԃ�
			m_bPauseMode = true;
			//���j���[�̍쐬
			m_pMenuManager->CreatePauseMenu();
			//�I�u�W�F�N�g��S�Ē�~����
			for (auto&& obj:m_pObjectManager->GetUpdateList()) {
				obj->StopUpdate();
			}
			//�J��������~������

		}
		//m_bPauseMode != m_bPauseMode;
	}

	//�Q�[�����[�h
	if (m_bEditMode == false) {
		if (m_bPauseMode == false) {
			//�Q�[���I�u�W�F�N�g�X�V
			m_pObjectManager->Update();

			//�X�e�[�W���Ƃ̍X�V
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

	//�����Ńv���C���[�̑��x������
	//�������̑��x���������Ă����ꍇ��������Ƃ���
	if (m_pObjectManager->GetGameObject(PLAYER_NAME)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		//�v���C���[�̏�Ԃ����Ċ��ɃW�����v��������������ύX���Ȃ�
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

	//�N���A�����ꍇ�̏���
	auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
	if (Player->GetComponent<CPlayer>()->GetClearFlg()) {
		//�Z���N�g��ʂɈړ�����܂ł̎��Ԃ��v��
		m_nSelectCnt++;
		//2�b�ŕύX����
		if (m_nSelectCnt > 120) {
			//���̃X�e�[�W���������
			//�X�e�[�W���Ƃɓ��e��h��������
			switch (m_eStageState) {
			case STAGE_1:
				//�����ɓ��邱�Ƃ͖��������������G���[

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
	//��������̏�����������X�e�[�^�X��ς���
	if (InputManager::Instance()->GetKeyTrigger(DIK_P)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		Player->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
	}

	//	�ҏW���[�h�̐؂�ւ�
	if (InputManager::Instance()->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// �؂�ւ�

		//CSound::Play(SE_PAUSE);	// ������ǉ�

		//	�؂�ւ��Ɠ����ɃJ�����̒����_�̕ύX
		if (m_bEditMode == true)	CCamera::Get()->SetAxisX(nullptr);	//edit	�v���C���[���璍���X���O��
		else						CCamera::Get()->SetAxisX(&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);	//game	�v���C���[�𒍎��X�ɐݒ�
	}

	//�V�[���̂�蒼��
	if (InputManager::Instance()->GetKeyTrigger(DIK_Z)) {
		//�V�[���J��
		SceneManager::Instance()->SetScene(SCENE_TITLE);
	}
#endif

}

/** 
* @fn		SceneGame::Draw
* @brief	�`�悷��֐�
* @detail	��������������A�㒅��Y��邱�ꑦ����������
*/
void SceneGame::Draw() {
	//�Q�[���I�u�W�F�N�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

	//�|�[�Y��ʕ`��
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
* @brief	�X�e�[�W�̐؂�ւ�
* @param	(Stage)	���̃X�e�[�W�ԍ�
*/
void SceneGame::SetStage(Stage NextStage) {
	//��O����
	if (NextStage >= MAX_STAGE) {
		MessageBox(GetMainWnd(), _T("�X�e�[�W�؂�ւ����s"), NULL, MB_OK);
		return;
	}

	//���݂̃X�e�[�W�̏I��
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//�X�e�[�W�̐ؑ�
	m_CurrentScene = m_GameSceneList[NextStage];
	m_eStageState = NextStage;

}

/** 
* @fn		SceneGame::GetStage	
* @brief	�X�e�[�W�̏����擾����
* @return	(Stage)	���݂̃X�e�[�W�ԍ���Ԃ�
*/
Stage SceneGame::GetStage() {
	return m_eStageState;
}

/**
* @fn		SceneGame::GetInstance
* @brief	�C���X�^���X�擾
* @return	(SceneGame*)	�C���X�^���X��Ԃ�
*/
SceneGame* SceneGame::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneGame();
	}
	return m_pInstance;
}

/**
* @fn		SceneGame::Destory
* @brief	�C���X�^���X�j��
*/
void SceneGame::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/** 
* @fn		SceneGame::SetPauseOOO
* @brief	�|�[�Y�̃I���I�t���O�����猈�߂�
* @param	(bool)	�|�[�Y���j���[���X�V���邩�ǂ���
*/
void SceneGame::SetPauseOOO(bool ooo) {
	m_bPauseMode = ooo;
}

/**
* @fn		SceneGame::GetScore
* @brief	�X�R�A���擾����
* @return	(int)	�Q�[���V�[�����Ǘ����Ă���X�R�A
*/
int SceneGame::GetScore() {
	return m_nScore;
}

/**
* @fn		SceneGame::SetScore
* @brief	�X�R�A�����Z
* @param	(int)	���Z���������l
*/
void SceneGame::SetScore(int num) {
	m_nScore += num;
}

/**
* @fn		SceneGame::GetTime
* @brief	�^�C���̎擾
* @return	(int)	�c�莞��
*/
int SceneGame::GetTime() {
	return m_nTimer;
}

/**
* @fn		SceneGame::SetTime
* @brief	�c�莞�Ԃ̐ݒ�
* @param	(int)	�ς��������l�ɕς���
*/
void SceneGame::SetTime() {
	m_nTimer--;
}

/**
* @fn		SceneGame::GetCurrentStage
* @brief	�X�e�[�W�̏�񂪓������x�N�^�[��Ԃ�
* @return	(vector<Scene*>)	���X�g
*/
std::vector<Scene*> SceneGame::GetCurrentStage() {
	return m_GameSceneList;
}

/** 
* @fn		SceneGame::GetLaL
* @brief	�����\�����擾����֐�
* @return	(bool)	true�ŕ\�Afalse�ŗ�
*/
bool SceneGame::GetLaL() {
	return m_bLoveandLovers;
}

/**
* @fn		SceneGame::SetLaL
* @brief	�X�e�[�W�r���ŗ��\�̋t�]����������g��
* @param	(bool)	�ω�����������(true�ŕ\ false�ŗ�)
*/
void SceneGame::SetLaL(bool lal) {
	m_bLoveandLovers = lal;
}

/**
* @fn		SceneGame::GetStageCurrent
* @brief	�X�e�[�W���Ƃ̊֐����g����悤�ɂ���
* @return	(Scene*)	���݂̃X�e�[�W�̎擾
*/
Scene* SceneGame::GetStageCurrent() {
	return m_CurrentScene;
}
