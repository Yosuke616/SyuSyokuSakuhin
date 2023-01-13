/**�C���N���[�h��**/
#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TextureManager.h"
#include "sceneGame.h"
#include "imgui.h"
#include "SceneManager.h"
#include "AnimMeshComponent.h"
#include "ModelManager.h"
#include "Draw3dComponent.h"
#include "ObjInfo.h"
#include "PlayerComponent.h"
#include "ObjectManager.h"
#include "SceneSelect.h"
#include "MosaicRollComponent.h"

/** �X�e�[�W��ǂݍ���*/
#include "SceneStage_1.h"
#include "SceneStage_1_Re.h"
#include "SceneStage_2.h"

/**�萔��`**/
/** @brief �X�e�[�W�I����ʂ̃I�u�W�F�N�g���A
		   �ǂꂾ���̑��x�ňړ����邩*/
#define STAGEOBJ_VEL	(2.0f)
/** @brief �ǂꂾ���ړ��������𐧌䂷��萔*/
#define MAX_OBJ_MOVE	(150.0f)

/**�ÓI�����o�ϐ�**/
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

/**�O���[�o���ϐ�**/
/** @brief ���Ԗڂ̃X�e�[�W�ɓ��������ǂ�����ۑ����Ă����ϐ�*/
/**
* @detail �ԍ��łǂ̃X�e�[�W���𔻕ʂ���
*	0.	�X�e�[�W1
*	1.	�X�e�[�W2
*   2.	�X�e�[�W3
*	3.	�X�e�[�W4
*	4.	�X�e�[�W5
*/
int g_nSelectStageNum = 0;

/**
* @fn		MenuManager::MenuManager
* @brief	�L�m�R���̒j�@�ݽ�׸��
*/
MenuManager::MenuManager() 
	:m_nDelayCnt(-1),m_nCreateMenu(STATE_NONE){
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	�S�Ẵ��j���[�I�u�W�F�N�g�̉��
*/
MenuManager::~MenuManager() {
	//�I�u�W�F�N�g���X�g�̉��
	for (auto&& menuObject :m_MenuList) {
		delete menuObject;
	}
	m_MenuList.clear();

	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		//�I�u�W�F�N�g���X�g�̉��
		/*for (auto&& menuObject : m_StageObjList) {
			delete menuObject;
		}*/
		m_StageObjList.clear();
		m_SelectSub.clear();
	}
}

/**
* @fn		MenuManager::GetInstance
* @brief	�C���X�^���X��
*/
MenuManager* MenuManager::GetInsutance() {
	if (!m_pInstance) {
		m_pInstance = new MenuManager;
	}
	return m_pInstance;
}

/**
* @fn		MenuManager::Destroy
* @brief	�C���X�^���X�̔j��
*/
void MenuManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::Start
* @brief	������
*/
void MenuManager::Start() {
	//���j���[�̏�����
	for (auto&& MenuObject:m_MenuList) {
		MenuObject->Start();
	}
	//���j���[���X�g���̃I�u�W�F�N�g�̏��������I�������C�e���[�^��擪�Ɏ����Ă���
	m_itr_Menu = m_MenuList.begin();
	//�Q�[���V�[���Ńu���b�N�A�E�g�悤�������玟�Ɏ����Ă���
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		if ((*m_itr_Menu)->GetName() == "Black") {
			m_itr_Menu++;
		}
	}
	
	//�����e�N�X�`���͓�����Ԃɂ���
	m_fBlackAlpha = 0.0f;
	m_nChangeCnt = 100;

	//�X�e�[�W�I����ʂ̂ݏ���������
	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		m_bRestart = false;
		m_fMoveObj = 0.0f;
	}
}

/**
* @fn		MenuManager::Update
* @brief	���j���[�̃��X�g�̓����Ă���I�u�W�F�N�g�̍X�V
*/
void MenuManager::Update() {
	if (m_nDelayCnt > 0) {
		//���j���[�쐬��x�点��
		m_nDelayCnt--;

		if (m_nDelayCnt == 0) {
			switch (m_nCreateMenu)
			{
			case TITLE_STATE: CreateTitleMenu(); break;
			case PAUSE_STATE: CreatePauseMenu(); break;
			case SELECT_STATE:break;
			case OPTION_STATE: CreateOptionMenu(); break;
			case MISS_STATE:  CreateMissMenu(); break;
			case TIMEOUT_STATE:CreateTimeUpMenu(); break;
			case GAMECLEAR_STATE:break;
			case GAMEOVER_STATE:CreateGameOverMenu(); break;
			default:break;
			}
	
			m_nDelayCnt = -1;
		}
	}

	//���j���[�I�u�W�F�N�g�̍X�V
	for (auto&& object : m_MenuList) {
		object->Update();
	}


	//Collider�̍X�V
	CCollider::GetColliderList().remove_if([](Object* pObj){
		return pObj->GetDeleteFlag();
	});


	//�I�u�W�F�N�g�폜�̊m�F
	m_MenuList.remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});

	/**�V�[�����Ƃ̏�����ǉ����Ă���**/

	/** @brief �I�v�V�������J����Ă���ꍇ�D�悷��*/
	if (m_bOption) {
		//�ǂ̃{�^�����I������Ă��邩�̏������s��
		SelectButton();
		//�����艺�̏����͍s��Ȃ�
		return;
	}

	/** @brief �^�C�g���Ȃǎn�߂���{�^����I�Ԃ��*/
	//���}���u�Ƃ��ăV�[���œ���悤�ɂ���
	if (SceneManager::Instance()->GetScene() == SCENE_TITLE) {	
		//�����ɑI������Ă���{�^���Ȃǂ�I�Ԋ֐���ǉ�����
		//�I�΂�Ă���{�^���̐F��ς���Ȃ�
		if (m_nDelayCnt == -1) {	
			SelectButton();
		}	
	}
	/** @brief �X�e�[�W�Z���N�g��p�̏���*/
	if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		if (m_nDelayCnt == -1) {
			StageSelect();
		}
	}

	/** @brief �~�X���j���[��p�̏���*/
	if (SceneManager::Instance()->GetScene() == SCENE_GAME) {
		if (m_nDelayCnt == -1) {
			Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			if (obj->GetComponent<CPlayer>()->GetPlayerSta() == MISS_PLAYER) {
				MissMenu();
			}
			else if (obj->GetComponent<CPlayer>()->GetPlayerSta() == TIMEOUT_PLAYER) {
				TimeOutMenu();
			}
			else if (obj->GetComponent<CPlayer>()->GetPlayerSta() == EVENT_TIME_OUT) {
				//EventTimeOut();
			}else {
				//�������̂����悭�킩��Ȃ��@�ꍇ�ɂ���Ă͕���������
				//SelectButton();
			}
		}
	}
}

/**
* @fn		MenuManager::Draw
* @brief	���j���[�I�u�W�F�N�g�̕`��
*/
void MenuManager::Draw() {
	SetZBuffer(true);
	//���j���[�I�u�W�F�N�g�̕`��
	for (auto&& menuObject:m_MenuList) {
		menuObject->Draw();
	}

#ifdef _DEBUG /**imgui�Ńf�o�b�O�̂���o��**/
	using namespace ImGui;

	/*SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Button_State");
	Text("B_STATE : %d", m_MenuList.size());
	End();*/

#endif
}

/**
* @fn		MenuManager::Create
* @brief	���j���[�̍쐬
* @param	(int)	�ǂ̃��j���[���쐬���邩�̔ԍ�
* @param	(int)	�ǂ̈ʂ̃t���[���̌�쐬���邩
*/
void MenuManager::Create(int state,int delayCnt) {
	m_nCreateMenu = state;
	m_nDelayCnt = delayCnt;
}

#pragma region ---�^�C�g�����j���[�쐬
/**
* @fn		MenuManager::CreateTitleMenu
* @brief	�^�C�g�����j���[�̍쐬
*/
void MenuManager::CreateTitleMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();

	//�^�C�g�����I�΂�Ă���悤�ɂ���
	m_nCreateMenu = TITLE_STATE;

	//�K�v�ɂȂ��Ă���{�^����ǉ�����
	//���߂���
	Object* pButton = new Object(UI_BEGIN_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton->SetPosition(UI_BEGIN_POS_X, UI_BEGIN_POS_Y);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(BEGIN_TEX_NUM));
	drawButton->SetSize(UI_BEGIN_SIZE_X, UI_BEGIN_SIZE_Y);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton);

	//**********************************************************************************************
	//��������
	Object* pButton2 = new Object(UI_CONTINUE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton2 = pButton2->AddComponent<CTransform>();
	auto drawButton2 = pButton2->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton2->SetPosition(UI_CONTINUE_POS_X, UI_CONTINUE_POS_Y);
	drawButton2->SetTexture(TextureManager::GetInstance()->GetTexture(CONTINUE_TEX_NUM));
	drawButton2->SetSize(UI_CONTINUE_SIZE_X, UI_CONTINUE_SIZE_Y);
	//���j���[���X�g�ɒǉ�
	AddMenu(pButton2);
	//***************************************************************************************************

	//**********************************************************************************************
	//��
	Object* pButton3 = new Object(UI_END_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton3 = pButton3->AddComponent<CTransform>();
	auto drawButton3 = pButton3->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton3->SetPosition(UI_END_POS_X, UI_END_POS_Y);
	drawButton3->SetTexture(TextureManager::GetInstance()->GetTexture(END_TEX_NUM));
	drawButton3->SetSize(UI_END_SIZE_X, UI_END_SIZE_Y);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton3);
	//***************************************************************************************************

	//�ǉ����ꂽ�I�u�W�F�N�g�̏��������s��
	Start();
}
#pragma endregion

#pragma region ---�|�[�Y���j���[�쐬
/**
* @fn		MenuManager::CeratePauseMenu
* @brief	�|�[�Y���j���[�̍쐬�ƕ\��
*/
void MenuManager::CreatePauseMenu() {
	//�I�u�W�F�N�g�̃��X�g�̍폜
	DeleteMenu();
	//�I�v�V�������I�΂�Ă���悤�ɂ���
	m_nCreateMenu = PAUSE_STATE;

	//�K�v�ɂȂ��Ă���I�u�W�F�N�g��ǉ����Ă���
	//���w�i�����
	Object* Black = new Object("Black", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�ǉ�
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBlack->SetPosition(0.0f, 0.0f, 0.0);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	drawBlack->SetColor(0.0f, 0.0f, 0.0f);
	drawBlack->SetAlpha(0.5f);

	//���j���[���X�g�ɒǉ�
	AddMenu(Black);
	
	//�ĊJ
	Object* Resume = new Object(UI_RESUME_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Resume = Resume->AddComponent<CTransform>();
	auto D_Resume = Resume->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Resume->SetPosition(UI_RESUME_POS_X, UI_RESUME_POS_Y);
	D_Resume->SetTexture(TextureManager::GetInstance()->GetTexture(RESUME_TEX_NUM));
	D_Resume->SetSize(UI_RESUME_SIZE_X, UI_RESUME_SIZE_Y);
	//�ǉ�
	AddMenu(Resume);

	//�X�e�[�W�Z���N�g
	Object* Select = new Object(UI_STAGE_SELECT_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Select = Select->AddComponent<CTransform>();
	auto D_Select = Select->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Select->SetPosition(UI_STAGE_SELECT_POS_X, UI_STAGE_SELECT_POS_Y);
	D_Select->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_SELECT_TEX_NUM));
	D_Select->SetSize(UI_STAGE_SELECT_SIZE_X, UI_STAGE_SELECT_SIZE_Y);
	//�ǉ�
	AddMenu(Select);

	//�I�v�V����
	Object* OptionObj = new Object(UI_OPTION_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Option = OptionObj->AddComponent<CTransform>();
	auto D_Option = OptionObj->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Option->SetPosition(UI_OPTION_POS_X, UI_OPTION_POS_Y);
	D_Option->SetTexture(TextureManager::GetInstance()->GetTexture(OPTION_TEX_NUM));
	D_Option->SetSize(UI_OPTION_SIZE_X, UI_OPTION_SIZE_Y);
	//�ǉ�
	AddMenu(OptionObj);

	//�ǉ����ꂽ�I�u�W�F�N�g�̏�����������
	Start();
}
#pragma endregion

#pragma region ---�X�e�[�W�Z���N�g
/**
* @fn		MenuManager::CreateSelectMenu
* @brief	�X�e�[�W�Z���N�g���j���[�̍쐬
*/
void MenuManager::CreateSelectMenu() {

	//�X�e�[�W���ǂꂾ����������������������
	m_nStageMoveCnt = 0;
	m_fStageMove_1 = 0.0f;
	m_fStageMove_2 = 0.0f;
	m_fStageMove_3 = 0.0f;
	m_fStageMove_4 = 0.0f;
	m_fStageMove_5 = 0.0f;
	//�X�e�[�W�𓮂������ǂ������Ǘ�����ϐ��̏�����
	m_bStageMoveFlg = true;

	//�I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();
	//�X�e�[�W�Z���N�g���j���[���I�΂�Ă���悤�ɂ���
	m_nCreateMenu = SELECT_STATE;

	//�e�N�X�`���}�l�[�W���[
	TextureManager* pTexManager = TextureManager::GetInstance();
	//���f���}�l�[�W���[
	ModelManager* pModelManager = ModelManager::GetInstance();

	//�K�v�ɂȂ��Ă���I�u�W�F�N�g�𐶐�����
	//�v���C���[
	Object* Box = new Object("Stand", UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	//�ݒ艼
	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	TransBox->SetPosition(0.0f, 15.0f,-65.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X + 25, DXCHAN_SIZE_Y + 25);
	DrawBox->SetUVsize(XMFLOAT2(-1.0f, 1.0f));
	DrawBox->SetAnimSplit(3, 3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	AddMenu(Box);

	//�X�e�[�W��\���I�u�W�F�N�g�̔z�u
	//�X�e�[�W1
	//�I�u�W�F�N�g�̐���
	Object* Stage1 = new Object("Stage_1",UPDATE_MODEL,DRAW_MODEL);
	//�R���|�[�l���g�̒ǉ�
	auto T_Stage1 = Stage1->AddComponent<CTransform>();
	auto D_Stage1 = Stage1->AddComponent<CDraw3D>();
	//�I�u�W�F�N�g�̐ݒ�
	D_Stage1->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage1->SetPosition(0.0f - (g_nSelectStageNum * 150.0f), 55.0f, -10);
	T_Stage1->SetScale(50.0f,50.0f,50.0f);
	T_Stage1->SetRotate(0.0f,0.0f,0.0f);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Stage1);
	//�X�e�[�W�I�u�W�F�N�g���Ǘ����郊�X�g�ɂ��ǉ����Ă���
	m_StageObjList.push_back(Stage1);

	//�X�e�[�W2
	Object* Stage2 = new Object("Stage_2", UPDATE_MODEL, DRAW_MODEL);
	//�R���|�[�l���g�̒ǉ�
	auto T_Stage2 = Stage2->AddComponent<CTransform>();
	auto D_Stage2 = Stage2->AddComponent<CDraw3D>();
	//�I�u�W�F�N�g�̐ݒ�
	D_Stage2->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage2->SetPosition(150.0f - (g_nSelectStageNum * 150.0f), 55.0f, -10);
	T_Stage2->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage2->SetRotate(0.0f, 0.0f, 0.0f);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Stage2);
	//�X�e�[�W�I�u�W�F�N�g���Ǘ����郊�X�g�ɂ��ǉ����Ă���
	m_StageObjList.push_back(Stage2);

	//�X�e�[�W3
	Object* Stage3 = new Object("Stage_3", UPDATE_MODEL, DRAW_MODEL);
	//�R���|�[�l���g�̒ǉ�
	auto T_Stage3 = Stage3->AddComponent<CTransform>();
	auto D_Stage3 = Stage3->AddComponent<CDraw3D>();
	//�I�u�W�F�N�g�̐ݒ�
	D_Stage3->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage3->SetPosition(300.0f - (g_nSelectStageNum * 150.0f), 55.0f, -10);
	T_Stage3->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage3->SetRotate(0.0f, 0.0f, 0.0f);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Stage3);
	//�X�e�[�W�I�u�W�F�N�g���Ǘ����郊�X�g�ɂ��ǉ����Ă���
	m_StageObjList.push_back(Stage3);

	//�X�e�[�W4
	Object* Stage4 = new Object("Stage_4", UPDATE_MODEL, DRAW_MODEL);
	//�R���|�[�l���g�̒ǉ�
	auto T_Stage4 = Stage4->AddComponent<CTransform>();
	auto D_Stage4 = Stage4->AddComponent<CDraw3D>();
	//�I�u�W�F�N�g�̐ݒ�
	D_Stage4->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage4->SetPosition(450.0f - (g_nSelectStageNum * 150.0f), 55.0f, -10);
	T_Stage4->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage4->SetRotate(0.0f, 0.0f, 0.0f);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Stage4);
	//�X�e�[�W�I�u�W�F�N�g���Ǘ����郊�X�g�ɂ��ǉ����Ă���
	m_StageObjList.push_back(Stage4);

	//�X�e�[�W5
	Object* Stage5 = new Object("Stage_5", UPDATE_MODEL, DRAW_MODEL);
	//�R���|�[�l���g�̒ǉ�
	auto T_Stage5 = Stage5->AddComponent<CTransform>();
	auto D_Stage5 = Stage5->AddComponent<CDraw3D>();
	//�I�u�W�F�N�g�̐ݒ�
	D_Stage5->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
	T_Stage5->SetPosition(600.0f - (g_nSelectStageNum * 150.0f), 55.0f, -10);
	T_Stage5->SetScale(50.0f, 50.0f, 50.0f);
	T_Stage5->SetRotate(0.0f, 0.0f, 0.0f);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Stage5);
	//�X�e�[�W�I�u�W�F�N�g���Ǘ����郊�X�g�ɂ��ǉ����Ă���
	m_StageObjList.push_back(Stage5);

	//UI�֌W�̒ǉ�
	Object* Push_Button = new Object(UI_PUSH_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_Push = Push_Button->AddComponent<CTransform>();
	auto D_Push = Push_Button->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_Push->SetPosition(UI_PUSH_POS_X, UI_PUSH_POS_Y);
	D_Push->SetTexture(TextureManager::GetInstance()->GetTexture(PUSH_TEX_NUM));
	D_Push->SetSize(UI_PUSH_SIZE_X, UI_PUSH_SIZE_Y);

	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(Push_Button);

	//�㉺�Ɉړ�����ׂ̃��X�g�ɒǉ�����
	m_SelectSub.push_back(Push_Button);

	//UI�֌W�̒ǉ�
	Object* To_Title = new Object(UI_TITLE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto T_ToTitle = To_Title->AddComponent<CTransform>();
	auto D_ToTitle = To_Title->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	T_ToTitle->SetPosition(UI_TITLE_POS_X, UI_TITLE_POS_Y);
	D_ToTitle->SetTexture(TextureManager::GetInstance()->GetTexture(TO_TITLE_TEX_NUM));
	D_ToTitle->SetSize(UI_TITLE_SIZE_X, UI_TITLE_SIZE_Y);

	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	AddMenu(To_Title);

	//�㉺�Ɉړ�����ׂ̃��X�g�ɒǉ�����
	m_SelectSub.push_back(To_Title);

	//�ǉ����ꂽ�I�u�W�F�N�g�̏�����
	Start();

	//�X�e�[�W�I�u�W�F�N�g�̃��X�g�𓮂�����C�e���[�^�[�͐擪���w���Ă���
	m_itr_Stage = m_StageObjList.begin();
	//�㉺�Ɉړ����邽�߂̃C�e���[�^��擪��
	m_itr_SelectSub = m_SelectSub.begin();

	//�Ō�ɑI�΂�Ă���X�e�[�W���C�e���[�^�[�ŊǗ�����

	//�X�e�[�W�Z���N�g
	for (int i = 0;i < g_nSelectStageNum;i++) {
		m_itr_Stage++;
	}

	//while (m_bStageMoveFlg) {
	//	if (g_nSelectStageNum == 0) {
	//		break;
	//	}
	//
	//	//�O���[�o���ϐ��̔ԍ����擾���Ăǂ�����n�߂�����߂�
	//	for (auto&& obj : m_StageObjList) {
	//		obj->GetComponent<CTransform>()->Vel.x -= STAGEOBJ_VEL;
	//		if (obj->GetName() == "Stage_1") {
	//			m_fStageMove_1 += STAGEOBJ_VEL;
	//			if (m_fStageMove_1 >= MAX_OBJ_MOVE) {
	//				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
	//				m_nStageMoveCnt++;
	//				m_fStageMove_1 = 0;
	//			}
	//		}
	//		else if (obj->GetName() == "Stage_2") {
	//			m_fStageMove_2 += STAGEOBJ_VEL;
	//			if (m_fStageMove_2 >= MAX_OBJ_MOVE) {
	//				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
	//				m_nStageMoveCnt++;
	//				m_fStageMove_2 = 0;
	//			}
	//		}
	//		else if (obj->GetName() == "Stage_3") {
	//			m_fStageMove_3 += STAGEOBJ_VEL;
	//			if (m_fStageMove_3 >= MAX_OBJ_MOVE) {
	//				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
	//				m_nStageMoveCnt++;
	//				m_fStageMove_3 = 0;
	//			}
	//		}
	//		else if (obj->GetName() == "Stage_4") {
	//			m_fStageMove_4 += STAGEOBJ_VEL;
	//			if (m_fStageMove_4 >= MAX_OBJ_MOVE) {
	//				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
	//				m_nStageMoveCnt++;
	//				m_fStageMove_4 = 0;
	//			}
	//		}
	//		else if (obj->GetName() == "Stage_5") {
	//			m_fStageMove_5 += STAGEOBJ_VEL;
	//			if (m_fStageMove_5 >= MAX_OBJ_MOVE) {
	//				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
	//				m_nStageMoveCnt++;
	//				m_fStageMove_5 = 0;
	//			}
	//		}
	//
	//		if (m_nStageMoveCnt >= (5 * g_nSelectStageNum)) {
	//			m_nStageMoveCnt = 0;
	//			m_bStageMoveFlg = false;
	//			break;
	//		}
	//	}
	//
	//}

}
#pragma endregion

#pragma region ---�Q�[���I�[�o�[���j���[�̍쐬
/**
* @fn		MenuManager::CreateGameOver
* @brief	�Q�[���I�[�o�[���j���[�̍쐬
*/
void MenuManager::CreateGameOverMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();
	//�Q�[���I�[�o�[���I�΂�Ă���悤�ɂ���
	m_nCreateMenu = GAMEOVER_STATE;

	//�K�v�ɂȂ��Ă���{�^����ǉ�����
		//��
	Object* pButton = new Object("Button_Kari", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton->SetPosition(0.0f, 0.0f);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton->SetSize(100.0f, 50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton);

	//**********************************************************************************************
	//��
	Object* pButton2 = new Object("Button_Kari2", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton2 = pButton2->AddComponent<CTransform>();
	auto drawButton2 = pButton2->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton2->SetPosition(0.0f, -50.0f);
	drawButton2->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton2->SetSize(100.0f, 50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton2);
	//***************************************************************************************************

	//**********************************************************************************************
	//��
	Object* pButton3 = new Object("Button_Kari3", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton3 = pButton3->AddComponent<CTransform>();
	auto drawButton3 = pButton3->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton3->SetPosition(0.0f, -100.0f);
	drawButton3->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton3->SetSize(100.0f, 50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton3);
	//***************************************************************************************************

	//�ǉ����ꂽ�I�u�W�F�N�g�̏��������s��
	Start();
}
#pragma endregion

#pragma region ---�~�X�����Ƃ��ɕ\��
void MenuManager::CreateMissMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();
	//�~�X���j���[���I�΂�Ă���悤�ɂ���
	m_nCreateMenu = MISS_STATE;

	//�K�v�ɂȂ��Ă���I�u�W�F�N�g��ǉ����Ă���
	//�Â����邽�߂̃e�N�X�`��
	Object* Black = new Object("Black",UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�ǉ�
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBlack->SetPosition(0.0f,0.0f);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	drawBlack->SetAlpha(0.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(Black);

	//�~�X�̕���
	Object* MissMoji = new Object("MissMoji",UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transMiss = MissMoji->AddComponent<CTransform>();
	auto drawMiss = MissMoji->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transMiss->SetPosition(0.0f, 400.0f);
	drawMiss->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawMiss->SetSize(100.0f,50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(MissMoji);

	//�ǉ����ꂽ�I�u�W�F�N�g�̏�����
	Start();
}
#pragma endregion

#pragma region ---�^�C���A�b�v
/**
* @fn		MenuManager::CreateTimeUpMenu
* @brief	�^�C���A�b�v�p�̉��o
*/
void MenuManager::CreateTimeUpMenu() {
	//���j���[���X�g�̍폜
	DeleteMenu();
	//�X�e�[�^�X��ς���
	m_nCreateMenu = TIMEOUT_STATE;

	//�K�v�ɂȂ��Ă���I�u�W�F�N�g��ǉ����Ă���
	//�Â����邽�߂̃e�N�X�`��
	Object* Black = new Object("Black", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�ǉ�
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBlack->SetPosition(0.0f, 0.0f);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	drawBlack->SetAlpha(0.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(Black);

	//�����̒ǉ�
	for (int i = 0;i < 6;i++) {
		Object* pText = new Object("moji", UPDATE_UI, DRAW_UI);
		//�R���|�[�l���g�̒ǉ�
		auto trans = pText->AddComponent<CTransform>();
		auto draw = pText->AddComponent<CDraw2D>();
		//�I�u�W�F�N�g�̐ݒ�
		trans->SetPosition(-200.0f + i * 75.0f,400.0f);
		draw->SetTexture(TextureManager::GetInstance()->GetTexture(ARROW_NUM));
		draw->SetSize(25,25);
		//���j���[���X�g�ɒǉ�
		AddMenu(pText);
	}

	//�Ō�ɉ�������{�o���Ȃ��ƃ_������
	Object* pLine = new Object("line",UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto trans = pLine->AddComponent<CTransform>();
	auto draw = pLine->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	trans->SetPosition(-1000.0f,0.0f);
	draw->SetTexture(TextureManager::GetInstance()->GetTexture(ARROW_NUM));
	draw->SetSize(500.0f, 25.0f);
	//���j���[���X�g�ɒǉ�
	AddMenu(pLine);

	//�I�u�W�F�N�g�̏�����������
	Start();
}
#pragma endregion

#pragma region ---�I�v�V�������j���[
/**
* @fn		MenuManager::CreateOptionMenu
* @brief	�I�v�V�����p�̃��j���[���쐬����
*/
void MenuManager::CreateOptionMenu() {

	//���ɃI�v�V����������Ă�������Ȃ�
	if (m_bOption) {
		return;
	}

	//���j���[���X�g�̍폜
	DeleteMenu();

	//�I�v�V�����t���O�I��
	m_bOption = true;

	//�I�v�V�������I�΂�Ă���悤�ɂ���
	m_nCreateMenu = OPTION_STATE;

	//�K�v�ɂȂ��Ă���I�u�W�F�N�g��ǉ����Ă���
		//���w�i�����
	Object* Black = new Object("Black",UPDATE_OPTION ,DRAW_OPTION);
	//�R���|�[�l���g�ǉ�
	auto transBlack = Black->AddComponent<CTransform>();
	auto drawBlack = Black->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBlack->SetPosition(0.0f, 0.0f, 0.0);
	drawBlack->SetTexture(TextureManager::GetInstance()->GetTexture(FEAD_OUT_NUM));
	drawBlack->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	drawBlack->SetColor(0.0f, 0.0f, 0.0f);
	drawBlack->SetAlpha(0.5f);

	//���j���[���X�g�ɒǉ�
	AddMenu(Black);

	//BGM�̐ݒ�
	Object* objBGM = new Object(UI_BGM_NAME, UPDATE_OPTION,DRAW_OPTION);
	//�R���|�[�l���g�̒ǉ�
	auto transBGM = objBGM->AddComponent<CTransform>();
	auto drawBGM = objBGM->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBGM->SetPosition(UI_BGM_POS_X, UI_BGM_POS_Y);
	drawBGM->SetTexture(TextureManager::GetInstance()->GetTexture(BGM_TEX_NUM));
	drawBGM->SetSize(UI_BGM_SIZE_X, UI_BGM_SIZE_Y);

	//���j���[���X�g�ɒǉ�
	AddMenu(objBGM);

	//SE�̐ݒ�
	Object* objSE = new Object(UI_SE_NAME, UPDATE_OPTION,DRAW_OPTION);
	//�R���|�[�l���g�̒ǉ�
	auto transSE = objSE->AddComponent<CTransform>();
	auto drawSE = objSE->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transSE->SetPosition(UI_SE_POS_X, UI_SE_POS_Y);
	drawSE->SetTexture(TextureManager::GetInstance()->GetTexture(SE_TEX_NUM));
	drawSE->SetSize(UI_SE_SIZE_X, UI_SE_SIZE_Y);

	//���j���[���X�g�ɒǉ�
	AddMenu(objSE);

	//���邳�̐ݒ�
	Object* objBrightness = new Object(UI_BRIGHTNESS_NAME, UPDATE_OPTION,DRAW_OPTION);
	//�R���|�[�l���g�̒ǉ�
	auto transBright = objBrightness->AddComponent<CTransform>();
	auto drawBright = objBrightness->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	transBright->SetPosition(UI_BRIGHTNESS_POS_X, UI_BRIGHTNESS_POS_Y);
	drawBright->SetTexture(TextureManager::GetInstance()->GetTexture(BRIGHTNESS_TEX_NUM));
	drawBright->SetSize(UI_BRIGHTNESS_SIZE_X, UI_BRIGHTNESS_SIZE_Y);

	//���j���[���X�g�ɒǉ�
	AddMenu(objBrightness);

	//�ǉ����ꂽ�I�u�W�F�N�g�̏�����
	Start();
}
#pragma endregion

/**
* @fn		MenuManager::AddMenu
* @brief	���j���[���X�g�ɒǉ�
* @param	(Object*)	�ǉ�����I�u�W�F�N�g�̃|�C���^
*/
void MenuManager::AddMenu(Object* pMenuObject) {
	//�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList) {
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder()) {
			itr++;
			continue;
		}
		else {
			break;
		}
	}
	//m_MenuList.insert(itr,pMenuObject);
	m_MenuList.push_back(pMenuObject);

	pMenuObject->Start();
}

/**
* @fn		MenuManager::GetMenuList
* @brief	���j���[���X�g�̎擾
*/
std::list<Object*>& MenuManager::GetMenuList() {
	return m_MenuList;
}

/**
* @fn		MenuManager::DeleteMenu
* @brief	���j���[���X�g���J���ɂ���
*/
void MenuManager::DeleteMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList) {
		menuObject->Delete();
	}
	m_MenuList.clear();
}

/**
* @fn		MenuManager::GetCreateMenu
* @brief	����郁�j���[�̔ԍ���Ԃ�
* @return	(int)	�ǂ̔ԍ��̃��j���[������邩
*/
int MenuManager::GetCreateMenu() {
	return m_nCreateMenu;
}

/**
* @fn		MenuManager::SelectButton
* @brief	�I�΂ꂽ�{�^���̐F�ς���A�����ꂽ���̏���
*/
void MenuManager::SelectButton() {
	//�F��S�ăf�t�H���g�ɖ߂��Ă���
	for (auto&& menuObject : m_MenuList) {
		menuObject->GetComponent<CDraw2D>()->SetColor(1.0f,1.0f,1.0f);
		//���j���[�̍��F�͍��F�̂܂܂�
		if (menuObject->GetName() == "Black") {
			menuObject->GetComponent<CDraw2D>()->SetColor(0.0f,0.0f,0.0f);
		}
	}

	//�L�[���͂Ń{�^����I�ׂ�悤�ɂ���
	if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
		//�����
		//�C�e���[�^�[���X�^�[�g�n�_�������ꍇ�I���ɍs������
		if (m_itr_Menu == m_MenuList.begin()) {
			m_itr_Menu = (m_MenuList.end());
			m_itr_Menu--;
		}
		else {
			//�����łȂ��ꍇ��1������
			m_itr_Menu--;
			//���������ʔw�i�̍��������ꍇ���K�ɍs������
			if ((*m_itr_Menu)->GetName() == "Black") {
				m_itr_Menu = (m_MenuList.end());
				m_itr_Menu--;
			}
		}
	}
	else if(InputManager::Instance()->GetKeyTrigger(DIK_S)){
		//������
		//�C�e���[�^�[���I�[�ɂ����ꍇ�X�^�[�g�n�_�ɍs������
		std::list<Object*>::iterator itr = m_MenuList.end();
		itr--;
		if (m_itr_Menu == itr) {
			m_itr_Menu = m_MenuList.begin();
			//�w�i�̍��������ꍇ,�����1��ɐi�߂�
			if ((*m_itr_Menu)->GetName() == "Black") {
				m_itr_Menu++;
			}
		}
		else {
			m_itr_Menu++;
		}
	}

	//�I�΂ꂽ�{�^���̐F��ς���
	(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(1.0f,0.0f,0.0f);

	//����{�^���������ꂽ���̏���
	if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
		PushButton();
	}
}

/**
* @fn		MenuManager::PushButton
* @brief	����{�^���������ꂽ���̏���
* @detail	�V�[�����Ƃɏ�����ς���̂ŃX�C�b�`�����Ƃ�ł��Ȃ��Ȃ�\�z
*/
void MenuManager::PushButton() {
	//���ꂽ�V�[�����Ƃŏ�����ς���
	switch (m_nCreateMenu) {
	case TITLE_STATE:
		if ((*m_itr_Menu)->GetName() == UI_BEGIN_NAME) {
			//�V�[���J��
			//CSV������
			StageSelect::NewGame();

			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_CONTINUE_NAME) {
			//�X�e�[�W�Z���N�g�ֈړ�����
			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_END_NAME){
			//�Q�[���I��
			PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
		}
		break;
	case PAUSE_STATE:
		if ((*m_itr_Menu)->GetName() == UI_RESUME_NAME) {
			//�Q�[�����ĊJ����
			//�I���I�t���Ђ�����Ԃ�
			SceneGame::GetInstance()->m_bPauseMode = false;
			//���j���[��j������
			DeleteMenu();
			//�I�u�W�F�N�g��S�čĊJ����
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			//�J�������ĊJ����
		
		}
		else if ((*m_itr_Menu)->GetName() == UI_STAGE_SELECT_NAME) {
			//�V�[���J��
			//�X�e�[�W�Z���N�g�ֈړ�����
			SceneManager::Instance()->SetScene(SCENE_SELECT);
		}
		else if ((*m_itr_Menu)->GetName() == UI_OPTION_NAME) {
			//�I�v�V�������j���[�̍쐬
			CreateOptionMenu();
			
			//PostMessage(GetMainWnd(), WM_CLOSE, 0, 0);
		}
		break;
	case OPTION_STATE:
		//��ŏ���
		if ((*m_itr_Menu)->GetName() == UI_BGM_NAME) {
			DeleteOptionMenu();
			CreatePauseMenu();
		}
		break;
	case GAMECLEAR_STATE:break;
	case GAMEOVER_STATE:break;
	default:break;
	}
}

/**
* @fn
* @brief
* @detail
*/
void MenuManager::StageSelect() {
	//�C�e���[�^�[�őI�΂�Ă���z�ȊO�̑傫����ύX����
	for (auto&& obj : m_StageObjList) {
		if ((*m_itr_Stage) == obj) {
			//�f�t�H���g�̑傫���ɂ���
			obj->GetComponent<CTransform>()->SetScale(50.0f, 50.0f, 50.0f);
			continue;
		}
		obj->GetComponent<CTransform>()->SetScale(30.0f,30.0f,30.0f);
	}
	//�㉺���X�g�őI�΂�Ă�����̂͐F�ς���
	//���̂��߂ɂ܂��̓f�t�H���g�ɕς���
	for (auto&& obj : m_SelectSub) {
		obj->GetComponent<CDraw2D>()->SetColor(1.0f,1.0f,1.0f);
	}

	//����s�\�t���O���I�t�ɂȂ��Ă���ꍇ����ł���
	if (!m_bRestart) {
		//�㉺�̈ړ����ł���悤�ɂ���
		if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
			//������Ɉړ�
			if (m_itr_SelectSub == m_SelectSub.begin()) {
				m_itr_SelectSub = m_SelectSub.end();
				m_itr_SelectSub--;
			}
			else {
				m_itr_SelectSub--;
			}
		}
		else if (InputManager::Instance()->GetKeyTrigger(DIK_S)) {
			//�������Ɉړ�
			std::list<Object*>::iterator itr = m_SelectSub.end();
			itr--;
			if (m_itr_SelectSub == itr) {
				m_itr_SelectSub = m_SelectSub.begin();
			}
			else {
				m_itr_SelectSub++;
			}
		}

		//�I�΂�Ă�����̂̐F��ς���
		(*m_itr_SelectSub)->GetComponent<CDraw2D>()->SetColor(1.0f,0.0f,0.0f);

		//�X�e�[�W�Z���N�g���E�ɓ�����
		if (InputManager::Instance()->GetKeyTrigger(DIK_D)) {

			//�����Ŏ��̃X�e�[�W���I���ł��邶��Ȃ�������Ԃ�
			//�ŏI�X�e�[�W��������Ԃ��Ȃ�
			if (!StageSelect::GetSelectAble(StageSelect::m_nCurrentStage)) {
				return;
			}

			//���݂̃X�e�[�W�����炷
			StageSelect::SetCurrentStage(true);

			//����s�\�t���O�𗧂Ă�
			m_bRestart = true;
			//����Ă���I�u�W�F�N�g���C�e���[�^�[���s���Ȃ��悤�ɂ���
			std::list<Object*>::iterator itr = m_StageObjList.end();
			itr--;
			if (m_itr_Stage == itr) {
				m_itr_Stage = itr;
				//�[�����������ꍇ�����ɑ���ł���悤�ɂ���
				m_bRestart = false;
			}
			else {
				//�I�΂�Ă���I�u�W�F�N�g�̕ύX
				m_itr_Stage++;
				//�I�u�W�F�N�g�����ɃX���C�h���Ă���
				for (auto&& obj : m_StageObjList) {
					obj->GetComponent<CTransform>()->Vel.x -= STAGEOBJ_VEL;
				}
				//�v���C���[�̃e�N�X�`���𑖂��Ă�����̂ɕύX����
				Object* menuItr = nullptr;
				for (auto&& obj : m_MenuList) {
					if (obj->GetName() == "Stand") {
						menuItr = obj;
					}
				}
				Object* player = menuItr;
				player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
				player->GetComponent<CAnimMesh>()->SetAnimSplit(7,2);
				player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 10, DXCHAN_SIZE_Y + 10);
				player->GetComponent<CAnimMesh>()->SetVertex(true);
			}
		}
		if (InputManager::Instance()->GetKeyTrigger(DIK_A)) {

			//���ݑI�΂�Ă���X�e�[�W�ԍ������炷
			StageSelect::SetCurrentStage(false);

			//����s�\�t���O�𗧂Ă�
			m_bRestart = true;
			//��ԏ��߂̒l��艺�ɂ����ɂ悤�ɂ���
			if (m_itr_Stage == m_StageObjList.begin()) {
				m_itr_Stage = m_StageObjList.begin();
				//�[�����������ꍇ�����ɑ���ł���悤�ɂ���
				m_bRestart = false;
			}
			else {
				//�I�΂�Ă���I�u�W�F�N�g�̕ύX
				m_itr_Stage--;
				//�I�u�W�F�N�g�����ɃX���C�h���Ă���
				for (auto&& obj : m_StageObjList) {
					obj->GetComponent<CTransform>()->Vel.x += STAGEOBJ_VEL;
				}
				//�v���C���[�̃e�N�X�`���𑖂��Ă�����̂ɕύX����
				Object* menuItr = nullptr;
				for (auto&& obj : m_MenuList) {
					if (obj->GetName() == "Stand") {
						menuItr = obj;
					}
				}
				Object* player = menuItr;
				player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
				player->GetComponent<CAnimMesh>()->SetAnimSplit(7, 2);
				player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 10, DXCHAN_SIZE_Y + 10);
				player->GetComponent<CAnimMesh>()->SetVertex(false);
			}
		}
		//����{�^���ŃX�e�[�W�ɓ����悤�ɂ���
		//�v�b�V��A���Ԃ��Ȃ��Ă���Ƃ��̂݃X�e�[�W�ɓ���
		if (InputManager::Instance()->GetKeyTrigger(DIK_SPACE)) {
			if ((*m_itr_SelectSub)->GetName() == UI_PUSH_NAME) {
				StageIN();			
			}
			else if((*m_itr_SelectSub)->GetName() == UI_TITLE_NAME){
				SceneManager::Instance()->SetScene(SCENE_TITLE);
			}
		}

	}
	else {
		//������x�ړ������瑬�x��0�ɂ��čĂшړ��ł���悤�ɂ���
		m_fMoveObj += STAGEOBJ_VEL;
		if (m_fMoveObj >= MAX_OBJ_MOVE) {
			for (auto&& obj : m_StageObjList) {
				obj->GetComponent<CTransform>()->Vel.x = 0.0f;
			}
			m_bRestart = false;
			m_fMoveObj = 0.0f;
			//�v���C���[�̃e�N�X�`���𑖂��Ă�����̂ɕύX����
			Object* menuItr = nullptr;
			for (auto&& obj : m_MenuList) {
				if (obj->GetName() == "Stand") {
					menuItr = obj;
				}
			}
			Object* player = menuItr;
			player->GetComponent<CAnimMesh>()->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
			player->GetComponent<CAnimMesh>()->SetAnimSplit(3, 3);
			player->GetComponent<CAnimMesh>()->SetSize(DXCHAN_SIZE_X + 25, DXCHAN_SIZE_Y + 25);
			player->GetComponent<CAnimMesh>()->SetVertex(true);
		}
	}
}

/** 
* @fn		MenuManager::StageIN
* @brief	�I�΂ꂽ�X�e�[�W�ɂ���ēǂݍ��ރX�e�[�W��ς���
*/
void MenuManager::StageIN() {
	if ((*m_itr_Stage)->GetName() == "Stage_1") {
		SceneGame::GetInstance()->SetStage(STAGE_1);
		//�ǂ��̃X�e�[�W���I�΂�Ă��邩�ۑ�����
		g_nSelectStageNum = 0;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_2") {
		SceneGame::GetInstance()->SetStage(STAGE_2);
		//�ǂ��̃X�e�[�W���I�΂�Ă��邩�ۑ�����
		g_nSelectStageNum = 1;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_3") {
		int i = 1 + 1;
		//�ǂ��̃X�e�[�W���I�΂�Ă��邩�ۑ�����
		g_nSelectStageNum = 2;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_4") {
		int i = 1 + 1;
		//�ǂ��̃X�e�[�W���I�΂�Ă��邩�ۑ�����
		g_nSelectStageNum = 3;
	}
	else if ((*m_itr_Stage)->GetName() == "Stage_5") {
		int i = 1 + 1;
		//�ǂ��̃X�e�[�W���I�΂�Ă��邩�ۑ�����
		g_nSelectStageNum = 4;
	}

	//�V�[���J��
	SceneManager::Instance()->SetScene(SCENE_GAME);
}

/**
* @fn		MenuManager::MissMenu
* @brief	�~�X�����Ƃ��ɏo�����郁�j���[
*/
void MenuManager::MissMenu() {
	//��肽������
	//���𔼓�����Ԃɂ���
	//�ォ�當���𗎂Ƃ�
	//�X�e�[�W�̏��߂����蒼��
	for (auto obj:m_MenuList) {
		//���w�i�̍X�V
		if (obj->GetName() == "Black") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();

			m_fBlackAlpha += 0.1f;
			if (m_fBlackAlpha >= 0.5f) {
				m_fBlackAlpha = 0.5f;
			}

			draw->SetAlpha(m_fBlackAlpha);

		}
	
		//�����̍X�V
		if (obj->GetName() == "MissMoji") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();

			trans->Pos.y -= 20.0f;

			if (trans->Pos.y <= 0.0f) {
				trans->Pos.y = 0.0f;
			}
		}
	}

	//�V�[���J�ڃJ�E���g�����炷
	m_nChangeCnt--;

	if (m_nChangeCnt <= 0) {
		//�I�u�W�F�N�g���X�g�̉��
		for (auto&& menuObject : m_MenuList) {
			delete menuObject;
		}
		m_MenuList.clear();
		//�t�F�[�h�A�E�g���ď��߂����蒼��
		SceneManager::Instance()->SetScene(SCENE_GAME);
	}
}

/**
* @fn		MenuManager::DeleteOptionMenu
* @brief	�I�v�V�������폜����
*/
void MenuManager::DeleteOptionMenu() {
	m_bOption = false;

	//���j���[���X�g�̍폜
	for (auto&& obj:m_MenuList) {
		//�폜
		obj->Delete();
	}
}

/**
* @fn		MenuManager::TimeOutMenu
* @brief	�^�C���A�b�v�����Ƃ��̏���
*/
void MenuManager::TimeOutMenu() {
	//������1�������o���Ă���
	for (auto&& obj : m_MenuList) {
		//���w�i�̍X�V
		if (obj->GetName() == "Black") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();

			m_fBlackAlpha += 0.1f;
			if (m_fBlackAlpha >= 0.5f) {
				m_fBlackAlpha = 0.5f;
			}

			draw->SetAlpha(m_fBlackAlpha);

		}

		//�����̍X�V
		if (obj->GetName() == "moji") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();
			
			//�����Â����Ă���
			trans->Pos.y -= 50.0f;

			//������0�ɂȂ����玟�̕����ɍs��
			//�����łȂ��ꍇ���̕����ɂ͍s���Ȃ�
			if (trans->Pos.y <= 0.0f) {
				trans->Pos.y = 0.0f;
			}
			else {
				break;
			}
		}

		//�Ō�ɉ��̐����o��
		if (obj->GetName() == "line") {
			auto trans = obj->GetComponent<CTransform>();
			auto draw = obj->GetComponent<CDraw2D>();
			
			//�����Â��ɓ�����
			trans->Pos.x += 75.0f;

			if (trans->Pos.x >= 0) {
				trans->Pos.x = 0.0f;
			}
		}
	}

	//line���ł����Ă���V�[����ς���
	std::list<Object*>::iterator itr = m_MenuList.begin();

	//�C�e���[�^�[��line���폜����
	for (int i = 0;i < m_MenuList.size();i++,itr++) {
		if ((* itr)->GetName() == "line") {
			break;
		}
	}

	if ((*itr)->GetComponent<CTransform>()->Pos.x >= 0.0f) {
		int n = m_nChangeCnt - 50;
		m_nChangeCnt--;
		n--;

		if (n <= 0) {
			//�I�u�W�F�N�g���X�g�̉��
			for (auto&& menuObject : m_MenuList) {
				delete menuObject;
			}
			m_MenuList.clear();
			//�t�F�[�h�A�E�g���ď��߂����蒼��
			SceneManager::Instance()->SetScene(SCENE_GAME);
		}

	}

}

/**
* @fn		MenuManager::SetSelectStage
* @brief	�I�΂�Ă���X�e�[�W��ݒ�
*/
//void MenuManager: