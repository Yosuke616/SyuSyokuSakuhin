/**�C���N���[�h��**/
#include "SceneSelect.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include "imgui.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "Draw3dComponent.h"
#include "AnimMeshComponent.h"
#include "Sound.h"

#include "Load.h"

/**�\���̒�`**/
StageSelect::Row StageSelect::m_StageGrid;
int StageSelect::m_nCurrentStage = 0;
int StageSelect::m_nOldStage;
StageSelect::Column StageSelect::m_Ohuda;

/**�O���[�o���ϐ�**/
CAnimMesh *g_CurrentBG = nullptr;

/**
* @fn		StageSelect::StageSelect
* @brief	�R���X�g���N�^�͓�x����
*/
StageSelect::StageSelect() {
	StageSelect::Load();

	m_nOldStage = STAGE_A;
}

/**
* @fn		StageSelect::~StageSelect
* @brief	�����A�΂݂���f�X�g���N�^����яo����
*/
StageSelect::~StageSelect() {

}

/**
* @fn		StageSelect::Init()
* @brief	����������
*/
void StageSelect::Init() {
	//���[�h��ʂ̃X���b�h�����
	Load::Begin();

	//���[�h���܂�
	//�i�s�󋵂̓Ǎ�
	StageSelect::Load();

	//�Ǘ��N���X�擾
	m_pObjectManager = ObjectManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	TextureManager* pTexManager = TextureManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();

	//�J�����̏�����
	CCamera::Get()->Init();

	//�e�N�X�`���̓Ǎ�
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);
	pTexManager->AddTexture(PATH_TEX_PUSH, PUSH_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TO_TITLE, TO_TITLE_TEX_NUM);

	//���f���̓Ǎ�
	pModelManager->AddModel(PATH_WALK_ENEMY, WALK_ENEMY_MODEL_NUM);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	m_nCurrentStage = m_nOldStage;

	//�K�v�ȃI�u�W�F�N�g�̒ǉ�
	//���������ɖ����I�u�W�F�N�g�������ɐ�������	
#pragma region ---�w�i
	//�I�u�W�F�N�g�̐���
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CAnimMesh>();
	//�I�u�W�F�N�g�̐ݒ�
	objBG->GetComponent<CTransform>()->SetPosition(0.0f,55.0f,0.0f);
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(450, 260);
	Draw_BG->SetColor(0.0f,0.0f,1.0f);
	Draw_BG->SetAnimSplit(1,1);
	Draw_BG->SetLoop(true);
	//���X�g�̒ǉ�
	m_pObjectManager->AddObject(objBG);

#pragma endregion

	//�O���[�o���ϐ��ɒl������
	g_CurrentBG = Draw_BG;

	//���j���[�̍쐬
	m_pMenuManager->CreateSelectMenu();

	//BGM�Đ�
	CSound::Play(SELECT_SE);

	//���[�h�I��
	Load::End();
}

/**
* @fn		StageSelect::Uninit
* @brief	�I������
*/
void StageSelect::Uninit() {
	//BGM��~
	CSound::Stop(SELECT_SE);
	//�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();
	//���j���[�̏I��
	m_pMenuManager->Destroy();
}

/**
* @fn		StageSelect::Update
* @brief	�X�V����
*/
void StageSelect::Update() {

	//�X�e�[�W���Ƃɔw�i�̕��͋C��ς���
	TextureManager *pTex = TextureManager::GetInstance();

	switch (m_nCurrentStage) {
	case STAGE_A:
		//�I�΂�Ă���X�e�[�W�̍X�V
		m_nOldStage = STAGE_A;
		g_CurrentBG->SetColor(0.0f,0.0f,1.0f);
		break;
	case STAGE_B:
		//�I�΂�Ă���X�e�[�W�̍X�V
		m_nOldStage = STAGE_B;
		g_CurrentBG->SetColor(0.0f, 1.0f, 0.0f);
		break;
	case STAGE_C:
		//�I�΂�Ă���X�e�[�W�̍X�V
		m_nOldStage = STAGE_C;
		g_CurrentBG->SetColor(1.0f, 0.0f, 0.0f);
		break;
	case STAGE_D:
		//�I�΂�Ă���X�e�[�W�̍X�V
		m_nOldStage = STAGE_D;
		g_CurrentBG->SetColor(1.0f, 0.0f, 1.0f);
		break;
	case STAGE_E:
		//�I�΂�Ă���X�e�[�W�̍X�V
		m_nOldStage = STAGE_E;
		g_CurrentBG->SetColor(1.0f, 1.0f, 1.0f);
		break;
	default: break;
	}
	//�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();
	//���j���[�̍X�V
	m_pMenuManager->Update();
}

/** 
* @fn		StageSelect::Draw
* @brief	�`�揈��
*/
void StageSelect::Draw() {
	//�I�u�W�F�N�g���X�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	m_pMenuManager->Draw();
	SetZBuffer(false);
}

/**
* @fn		StageSelect::Load
* @brief	�X�e�[�W�N���A�󋵂�ǂݍ���
* @return	(bool)	�{���ɓǂݍ��߂��̂���
*/
bool StageSelect::Load() {
	//�N���[�o�[�擾���̓Ǎ�
	if (FileManager::GetInstance()->Load(OHUDA_DATA_CSV) ==false){
		//�G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//�N���A�󋵂̓Ǎ�
	if (FileManager::GetInstance()->Load(STAGE_CLEAR_CSV) == false) {
		//�G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//�N���A�󋵂���������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	m_StageGrid.resize(CSV->GetRowSize());				// �s�̐���ݒ�
	for (int y = 0; y < CSV->GetRowSize(); ++y) {
		m_StageGrid[y].resize(CSV->GetColumnSize(y));	// ��̐���ݒ�

		for (int x = 0; x < CSV->GetColumnSize(y); x++){
			// �v�f���i�[����
			m_StageGrid[y][x] = CSV->GetInt(x, y);
		}
	}

	//�N���[�o�[�̎擾�󋵂��ڂ�
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	m_Ohuda.clear();
	for (int x = 0;x < OhudaCSV->GetColumnSize(0);x++) {
		//�v�f���i�[����
		m_Ohuda.push_back(OhudaCSV->GetInt(x,0));
	}

	//�����ɕԂ�
	return true;
}

/**
* @fn		StageSelect::Save
* @brief	�X�e�[�W�̃N���A�󋵂�ۑ�����
* @return	(bool)	�X�e�[�W���N���A�����珑������
*/
bool StageSelect::Save() {
	// �N���A�󋵂̕ۑ�
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	if (CSV == nullptr) { 
		return false;
	}
	if (CSV->Save(STAGE_CLEAR_CSV) == false){
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	//�N���[�o�[�̎擾�󋵂̕ۑ�
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	if (OhudaCSV ==nullptr) {
		return false;
	}
	if (OhudaCSV->Save(OHUDA_DATA_CSV) == false) {
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

/**
* @fn		StageSelect::NewGame
* @brief	CSV�t�@�C��������������
* @return	(bool)	�������������������ǂ���
*/
bool StageSelect::NewGame() {
	m_nOldStage = STAGE_A;

	// ��������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);

	for (int nRow = 0; nRow < m_StageGrid.size(); nRow++){
		for (int nColumn = 0; nColumn < m_StageGrid[nRow].size(); nColumn++){
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	// �ŏ��̃X�e�[�W�I���͉\
	CSV->SetInt(0, 0, 1);

	//�N���[�o�[�̎擾��
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	for (int nColumn = 0;nColumn < m_Ohuda.size();nColumn++) {
		OhudaCSV->SetInt(nColumn,0,0);
	}

	StageSelect::Save();

	return true;
}

/**
* @fn		StageSelect::SaveClearinfo
* @brief	�N���A�󋵂̕ۑ�
* @param	(int)	���Ԗڂ̃X�e�[�W���N���A������	
* @return	(bool)	�������ۑ��ł������ǂ���
*/
bool StageSelect::SaveClearInfo(int nStage,bool bOhuda) {
	//���D��csv���擾����
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	
	//���D������������
	OhudaCSV->SetInt(nStage,0,bOhuda);

	// �N���A�󋵂�����������
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	// ���̃X�e�[�W�̃t���O��
	int x = nStage + 1;

	CSV->SetInt(x, 0, 1);	// (��, �s, �i�[���鐔��)

	// csv�ɕۑ�
	StageSelect::Save();

	return true;
}

/**
* @fn		StageSelect::GetSelectAble
* @brief	�X�e�[�W���I���\���ǂ����𒲂ׂ�
* @param	(int)	���Ԗڂ̃X�e�[�W���I���\�����ׂ�
* @return	(bool)	�I���\���ǂ����𒲂ׂ�
*/
bool StageSelect::GetSelectAble(int nStage) {

	int n = nStage + 1;

	if (m_StageGrid[0][n] == 0)
	{
		if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		
		}
		return false;
	}
	else {
		return true; 
	}


}

/**
* @fn		StageSelect::SetCurrentStage
* @brief	���ݑI�΂�Ă���X�e�[�W�ԍ��̐ݒ�
* @param	(bool)	true��������+ false�Ȃ�-	
*/
void StageSelect::SetCurrentStage(bool num) {
	if (num) {
		if (m_nCurrentStage >= STAGE_E) {
		
		}
		else {
			m_nCurrentStage++;		
		}
	}
	else {
		if (m_nCurrentStage <= STAGE_A) {
		
		}
		else {
			m_nCurrentStage--;
		}
	}
}

/**
* @fn		StageSelect::SaveOhudaInfo
* @brief	���D�̎擾�����擾����
* @param	(int)	�ǂ̃X�e�[�W���̏��𓾂�
* @return	(bool)	��������Ɠǂݍ��߂����ǂ���
*/
bool StageSelect::SaveOhudaInfo(int nStage) {
	m_Ohuda[nStage] = 1;

	return true;
}

/**
* @fn		StageSelect::GetOhuda
* @brief	���D�̏����擾����
* @return	(StageSelect::Column)	����ڂ��ǂ���
*/
StageSelect::Column StageSelect::GetOhuda() {
	return m_Ohuda;
}