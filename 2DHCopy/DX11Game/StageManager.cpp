/**�C���N���[�h��**/
#include "StageManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "sceneGame.h"
#include "Object.h"
#include "ObjInfo.h"
#include "imgui.h"
#include "Camera.h"
//�t�@�C�����Ǘ�����}�l�[�W���[�����
#include "FileManager.h"
//CSV��ǂݍ��ދ@�\��ǂݍ���
#include "ResourceCSV.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "SeeCollComponent.h"
#include "AnimMeshComponent.h"
#include "Draw3dComponent.h"
#include "Draw2DComponent.h"
#include "ModelManager.h"
#include "GravityComponent.h"
#include "OutOfRange.h"
#include "EnemyComponent.h"
#include "BillboardComponent.h"
#include "ItemComponent.h"

//�X�e�[�W�̃C���N���[�h
#include "SceneStage_1.h"
#include "SceneStage_1_Re.h"

/**�񋓑̐錾**/
/** @brief �ǂ̃I�u�W�F�N�g��z�u���邩�𔻕ʂ���*/
enum MAP_CHIP{
	N = -1,				//�����Ȃ�
	ENEMY_1 = 1,			//�G

	//��
	B_1 = 10,			//�u���b�N(�㕔) 
	GRASS_IN = 11,		//�u���b�N(����(�����蔻�薳��))
	GRASS_IN_COLL = 12,	//�u���b�N(����(�����蔻�肠��))

	GOAL = 50,			//�S�[��
	KOBAN,				//����(�X�R�A�A�b�v�A�C�e��)
	MAGA,				//����(�p���[�A�b�v�A�C�e��)
	OHUDA,				//���D(�L�[�A�C�e��)

	MISS_COLL = 99,					//�~�X�u���b�N
	WARP,							//�X�e�[�W���ƂɃ��[�v�̏�����ς���
	STAGE_1_MISS_COLL = 199,		//�X�e�[�W1�C�x���g�~�X

	STAGE_1_RE_CHANGE_COLL = 299,	//�I�u�W�F�N�g�̌����ڂ�ς��邽�߂̃C�x���g

	SOCRE_UP = 2000,				//�����R�C���I�Ȃ��

	ARROW = 9999,					//�r���{�[�h(��)

	MAX_MAP_CHIP
};

/**�ÓI�����o�ϐ�**/
StageManager* StageManager::m_pInstance = nullptr;

/**
* @fn		StageManager::GetInstance
* @brief	�C���X�^���X�̎擾
*/
StageManager* StageManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new StageManager();
	}

	return m_pInstance;
}

/**
* @fn		StageManager::Destroy
* @brief	�C���X�^���X�̔j��
*/
void StageManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**	
* @fn		StageManager::StageManager
* @brief	�R���X�g���N�^�����E�E�E�����Ō��߂�I�I�I
*/
StageManager::StageManager() 
	:m_nBlock_X(0),m_nBlock_Y(0),m_nMapSta(N)
	,m_pShowObject(nullptr){
	//��ʂɈڂ��Ă��郏�[���h���W�̗����Ɖ������v�Z����
	m_WorldSize = CalcWorldSize();

	//�X�e�[�W�`��̋N�_
	m_BasePos.x = -(m_WorldSize.x - MAPCHIP_WIDTH)  * 0.5f - MAPCHIP_WIDTH;
	m_BasePos.y =  (m_WorldSize.y - MAPCHIP_HEIGHT) * 0.5f;
}

/**
* @fn		StageManager::~StageManager
* @brief	���@�f�X�g���N�^�ɕ������E�E�E
*/
StageManager::~StageManager() {
	if (m_pShowObject) {
		delete m_pShowObject;
		m_pShowObject = nullptr;
	}
}

/**
* @fn		StageManager::Update
* @brief	�X�V����
*/
void StageManager::Update() {
#ifdef _DEBUG
	//�ҏW���[�h
	if (SceneGame::GetInstance()->m_bEditMode) {
		//�}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����
		InputManager* Input = InputManager::Instance();
		XMFLOAT2 Mouse_Pos = Input->GetMousePos();
		XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
		XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
		XMFLOAT3 worldPos;		// �Z�o���ʂ��i�[����
		CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);

		//�����u������������������ǉ����Ă���
		if (Input->GetKeyTrigger(DIK_1)) m_nMapSta = B_1;				//�u���b�N1�ł���

		//�v�f�����o�����߂̌v�Z������
		m_Element_X = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));
		m_Element_Y = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));

		//�������W�����߂�
		float CreatePosX = m_BasePos.x + (m_Element_X)* MAPCHIP_WIDTH;		//�����N�_����v�f���~�u���b�N�̃T�C�Y�������ړ������_
		float CreatePosY = m_BasePos.y - (m_Element_Y)* MAPCHIP_HEIGHT;

		// �J�[�\���ŕ\��������̐���
		this->UpdateShowObject(CreatePosX, CreatePosY);

		//�{�^������������I�u�W�F�N�g��ǉ�����
		if (Input->GetMouseTrigger(MOUSE_L))
		{
			// ��������ꏊ�̃u���b�NID�����߂�
			int nMapChipID = (m_Element_Y * m_nBlock_X) + m_Element_X;	//y�̗v�f�T�C�Y �~ x�̗v�f�T�C�Y + x�̗v�f��

			//�I�u�W�F�N�g�����(�������Wx, �������Wy, �X�e�[�W�z��)
			CreateBlock_Deb(CreatePosX, CreatePosY, nMapChipID, m_Element_X, m_Element_Y, m_nMapSta);

			//2021/12/30 narita
			// �t�@�C����������
			std::string file;
			switch (SceneGame::GetInstance()->GetStage())
			{
				//�ǂݍ���CSV��I��(?)
			case STAGE_1:break;
			default: break;
			}
			ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
			CSV->SetInt(m_Element_X, m_Element_Y, m_nMapSta);
		}

		// �X�e�[�W��csv�ɕۑ�
		if (Input->GetKeyPress(DIK_LCONTROL) &&
			Input->GetKeyTrigger(DIK_S))
		{
			this->Save(SceneGame::GetInstance()->GetStage());
		}

		// �J�[�\���I�u�W�F�N�g�̍X�V
		if (m_pShowObject)	m_pShowObject->Update();
		//-----------------------------------------------------------------------------------------------------

		else

			// �ҏW���[�h�̏I���
			if (m_pShowObject)
			{
				delete m_pShowObject;
				m_pShowObject = nullptr;
			}
	}
#endif
}

/** 
* @fn		StageManager::Draw
* @brief	�`�悷���
* @detail	�u�������I�u�W�F�N�g�Ƃ��f�o�b�O�p�̕`��
*/
void StageManager::Draw() {
	//�J�[�\���I�u�W�F�N�g�̕`��
	if (m_pShowObject) {
		m_pShowObject->Draw();
	}

#ifdef _DEBUG
	using namespace ImGui;
	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Stage");

	//�}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����
	InputManager* Input = InputManager::Instance();
	XMFLOAT2 Mouse_Pos = Input->GetMousePos();
	XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
	XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
	XMFLOAT3 worldPos;		// �Z�o���ʂ��i�[����
	CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);
	//�v�f�����o�����߂̌v�Z������
	int Num_x = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));	// �v�f��
	int Num_y = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));	// �v�f��
	// ��������ꏊ�̃u���b�NID�����߂�
	int nMapChipID = (Num_y * m_nBlock_X) + Num_x;	//y�̗v�f�T�C�Y �~ x�̗v�f�T�C�Y + x�̗v�f��
	Text("ID   : %2d", nMapChipID);

	End();

#endif // _DEBUG

}

/**
* @fn		StageManager::CreateStage
* @brief	�X�e�[�W�̒n�`�̍쐬
* @param	(int)	�X�e�[�W�̔ԍ�(���Ԗڂ̃X�e�[�W��)
*/
void StageManager::CreateStage(int stage_state) {
	//�X�e�[�W�̓ǂݍ���(�ԍ��ɂ���ēǂݍ���csv��ύX)
	switch (stage_state) {
	case STAGE_1:this->Load(STAGE_1_CSV); break;
	case STAGE_1_RE:this->Load(STAGE_1_RE_CSV); break;
	default:break;
	}

	float fBaseX = m_BasePos.x;
	float fBaseY = m_BasePos.y;
	int nBlockID = 0;

	//�}�b�v�̔z�񂾂����[�v
	for (int y = 0;y < (int)m_StageGrid.size();y++) {
		for (int x = 0;x < (int)m_StageGrid[y].size();x++) {
			Object* pObject;
			//�u���b�N����
			pObject = CreateBlock(fBaseX,fBaseY,m_StageGrid[y][x],nBlockID);

			//���̃u���b�N�̍��W��
			fBaseX += MAPCHIP_WIDTH;
			nBlockID++;
		}

		//X���W�����Z�b�g����
		fBaseX = m_BasePos.x;
		//���̃u���b�N���W��(Y���W)
		fBaseY -= MAPCHIP_HEIGHT;
	}

	//�X�e�[�W�̗v�f����ۊ�
	//Y�̗v�f��
	m_nBlock_Y = (int)m_StageGrid.size();
	for (auto row:m_StageGrid) {
		//1�ԃT�C�Y���傫����𔽉f
		if (m_nBlock_X < (int)row.size()) {
			//X�̗v�f��
			m_nBlock_X = (int)row.size();
		}
	}

	//�X�e�[�W���T�C�Y���i�[
	//�v�f���~�}�b�v�`�b�v�̉���
	m_vStageSize.x = m_nBlock_X * MAPCHIP_WIDTH;
	m_vStageSize.y = m_nBlock_Y * MAPCHIP_HEIGHT;

	//�X�e�[�W�����I�������I�΂�Ă���X�e�[�W���ƂɁA
	//�ݒ肵�Ȃ��Ƃ����Ȃ����Ƃ�ݒ肷��
	switch (stage_state)
	{
	case STAGE_1:
		//���X�g�̒��g���������������Ƃ�����
		SceneStage_1::GetInstance()->DeleteList();
		SceneStage_1::GetInstance()->SetBaseInfo(ObjectManager::GetInstance()->GetUpdateList());
		break;
	case STAGE_1_RE:
		SceneStage_1_Re::GetInstance()->DeleteList();
		SceneStage_1_Re::GetInstance()->SetBaseInfo(ObjectManager::GetInstance()->GetUpdateList());
		break;
	default:break;
	}

}

/**
* @fn		StageManager::CreateBlock
* @brief	�u���b�N�̐���
* @param	(float)		X���W
* @param	(float)		Y���W
* @param	(int)		�I�u�W�F�N�g�i���o�[
* @param	(int)		�u���b�N�ƕR�t���Ă���L�[
* @return	(Object*)	���ꂽ�I�u�W�F�N�g�̃|�C���^
*/
Object* StageManager::CreateBlock(float fPosX,float fPosY,int nState,int nBlockID) {
	//�I�u�W�F�N�g�̔ԍ���N�������琶�����Ȃ�
	if (nState == N) {
		return nullptr;
	}

	//���f���}�l�[�W���[�ƃe�N�X�`���}�l�[�W���[
	ModelManager* pModelManager = ModelManager::GetInstance();
	TextureManager* pTextureManager = TextureManager::GetInstance();

#pragma region ---�΃u���b�N
	//�u���b�N
	if (nState == B_1) {
		Object* obj = new Object(BLOCK_NAME,UPDATE_FIELD,DRAW_FIELD);
		//�R���|�[�l���g�̒ǉ�
		auto trans    = obj->AddComponent<CTransform>();
		auto draw	  = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));
		trans->SetPosition(fPosX,fPosY+770,0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();
		
		return obj;
	}
#pragma endregion
#pragma region ---�y�u���b�N(�����蔻�薳��) 
	//�y�u���b�N
	else if (nState == GRASS_IN) {
		Object* obj = new Object(BLOCK_RARD_NAME, UPDATE_FIELD, DRAW_FIELD);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto Range = obj->AddComponent<COutOfRange>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetModel(pModelManager->GetModel(RARD_BLOCK_NUM));
		trans->SetPosition(fPosX, fPosY + 770, 0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		//collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---�y�u���b�N(�����蔻��L��)
	//�y�u���b�N
	else if (nState == GRASS_IN_COLL) {
		Object* obj = new Object(BLOCK_RARD_NAME, UPDATE_FIELD, DRAW_FIELD);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto Range = obj->AddComponent<COutOfRange>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetModel(pModelManager->GetModel(RARD_BLOCK_NUM));
		trans->SetPosition(fPosX, fPosY + 770, 0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---�����G
	//�����G
	else if(nState == ENEMY_1){
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(ENEMY_NAME,UPDATE_MODEL,DRAW_MODEL);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<CSeeColl>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CGravity>();
		//�G��p�̃R���|�[�l���g��ǉ�����\��
		obj->AddComponent<CEnemy>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
		trans->SetPosition(fPosX,fPosY+800,0.0f);//800
		trans->SetScale(E_WALK_SIZE_X, E_WALK_SIZE_Y, E_WALK_SIZE_Z);
		trans->SetRotate(0.0f,90.0f,0.0f);
		collider->SetCollisionSize(E_WALK_COLL_X, E_WALK_COLL_Y, E_WALK_COLL_Z);
		obj->GetComponent<CEnemy>()->SetEnemyType(ENEMY_WALK);
		//�I�u�W�F�N�g�}�l�[�W���ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---�S�[��
	else if (nState == GOAL) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(GOAL_NAME,UPDATE_MODEL,DRAW_MODEL);
		//�R���|�[�l���g�̒ǉ�
		auto trans	  = obj->AddComponent<CTransform>();
		auto draw	  = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
		trans->SetPosition(fPosX, fPosY + 765, 0.0f);
		trans->SetScale(E_WALK_SIZE_X, E_WALK_SIZE_Y, E_WALK_SIZE_Z);
		collider->SetCollisionSize(E_WALK_COLL_X, E_WALK_COLL_Y, E_WALK_COLL_Z);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);
		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---�~�X����
	else if (nState == MISS_COLL) {
		//�I�u�W�F�N�g�̐���(�����蔻��̂�)
		Object* obj = new Object("MISS_COLL",UPDATE_DEBUG,DRAW_DEBUG);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		//draw->SetLoop(true);
		trans->SetPosition(fPosX, fPosY + 940, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---���[�v�n�_�𐶐�
	else if (nState == WARP) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(WARP_NAME,UPDATE_UI,DRAW_UI);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�

		
	}
#pragma endregion
#pragma region ---�X�e�[�W1�̃~�X����
	else if (nState == STAGE_1_MISS_COLL) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(STAGE_1_MISS_EVENT,UPDATE_DEBUG,DRAW_DEBUG);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 770.0f, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);
		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---�X�e�[�W1(�����[�f�b�h)�̃C�x���g�����蔻��
	else if (nState == STAGE_1_RE_CHANGE_COLL) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(STAGE_RE_1_CHANGE_COLL,UPDATE_DEBUG,DRAW_DEBUG);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 770.0f, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);
		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---����
	else if (nState == KOBAN) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(ITEM_NAME,UPDATE_DEBUG,DRAW_DEBUG);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		type->SetItem(ITEM_KOBAN);
		draw->SetModel(pModelManager->GetModel(KOBAN_MODEL_NUM));
		trans->SetPosition(fPosX,fPosY+770.0f,0.0f);
		trans->SetScale(KOBAN_SIZE_X, KOBAN_SIZE_Y, KOBAN_SIZE_Z);
		collider->SetCollisionSize(KOBAN_COLL_SIZE_X, KOBAN_COLL_SIZE_Y, KOBAN_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
}
#pragma endregion
#pragma region ---����
	else if (nState == MAGA) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(ITEM_NAME,UPDATE_MODEL,DRAW_MODEL);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		type->SetItem(ITEM_MAGA);
		draw->SetModel(pModelManager->GetModel(MAGA_MODEL_NUM));
		trans->SetPosition(fPosX+20.0f,fPosY + 770.0f,0.0f);
		trans->SetScale(MAGA_SIZE_X,MAGA_SIZE_Y, MAGA_SIZE_Z);
		collider->SetCollisionSize(MAGA_COLL_SIZE_X,MAGA_COLL_SIZE_Y,MAGA_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);

		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion 
#pragma region ---���D
	else if (nState == OHUDA) {
		//�I�u�W�F�N�g�̐���
		Object* obj = new Object(ITEM_NAME,UPDATE_MODEL,DRAW_MODEL);
		//�R���|�[�l���g�̒ǉ�
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//�I�u�W�F�N�g�̐ݒ�
		type->SetItem(ITEM_OHUDA);
		draw->SetModel(pModelManager->GetModel(OHUDA_MODEL_NUM));
		trans->SetPosition(fPosX + 20.0f, fPosY + 770.0f, 0.0f);
		trans->SetScale(OHUDA_SIZE_X, OHUDA_SIZE_Y, OHUDA_SIZE_Z);
		collider->SetCollisionSize(OHUDA_COLL_SIZE_X, OHUDA_COLL_SIZE_Y, OHUDA_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(obj);
		//���[���h�}�g���b�N�X�̍X�V
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---��Ώ����Ė�ڂł���
	else if (nState == ARROW) {
		//�I�u�W�F����
		Object* Arr = new Object("Arr",UPDATE_UI,DRAW_UI);
		//�R���|�l���g
		auto trans = Arr->AddComponent<CTransform>();
		auto draw = Arr->AddComponent<CBillboard>();
		Arr->AddComponent<COutOfRange>();
		draw->SetTexture(TextureManager::GetInstance()->GetTexture(ARROW_NUM));
		draw->SetAnimSplit(1,1);
		draw->SetSize(E_WALK_SIZE_X, E_WALK_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 700, 0.0f);//800
		ObjectManager::GetInstance()->AddObject(Arr);

		draw->Update();

		return Arr;
		
	}
#pragma endregion
}

/**
* @fn		StageManager::CreateBlock_Deb
* @brief	�}�E�X����ł̃u���b�N�̐����ƍ폜
* @param	(float)	X���W
* @param	(float)	Y���W
* @param	(int)	�����u���b�N�̔ԍ�
* @param	(int)	�O���b�h�ɓo�^���邽�߂̔ԍ�X
* @param	(int)	�O���b�h�ɓo�^���邽�߂̔ԍ�Y
* @param	(int)	���o���u���b�N�̎��
*/
void StageManager::CreateBlock_Deb(float fPosX,float fPosY,int nNumber,int x,int y,int nCreate_Status) {
	//id����I�u�W�F�N�g�u���b�N�����擾
	Object *pBlock_Del = StageManager::GetBlock(nNumber);
	//�I�u�W�F�N�g�����݂��Ă�����폜����
	if (pBlock_Del) {
		//�I�u�W�F�N�g���폜
		pBlock_Del->Delete();
	}

	//�u���b�N�𐶐�����
	this->CreateBlock(fPosX,fPosY,nCreate_Status,nNumber);
	//�X�e�[�W�z��ɓo�^
	m_StageGrid[x][y] = nCreate_Status;
}

/**
* @fn		StageManager::GetBlock
* @brief	id����I�u�W�F�N�g�̏����擾����
* @param	(int)		�I�u�W�F�N�g�ƕR�Â��Ă���I�u�W�F�N�g
* @return	(Object*)	���݂��邩�ǂ���
*/
Object* StageManager::GetBlock(int nNum) {

	//�Ȃɂ����݂��Ȃ�������폜����
	//�������炠������Ԃ�
	//�܂肻�̃|�C���g�̍��W���K�v�ɂȂ��Ă���
	//���W�̓}�E�X���W�����[���h���W�ɂ�������
	return nullptr;
}

/**
* @fn		Stagemanager::UpdateShowObject
* @brief	�z�u����I�u�W�F�N�g�̕\���̍X�V
* @param	(float)	��������X���W
* @param	(float)	��������Y���W
*/
void StageManager::UpdateShowObject(float fPosX,float fPosY) {
	if (!m_pShowObject) {
		//�I�u�W�F�N�g�̐���
		m_pShowObject = new Object("debug",UPDATE_DEBUG,DRAW_DEBUG);
		//�R���|�[�l���g�̒ǉ�
		auto trans = m_pShowObject->AddComponent<CTransform>();
		auto draw  = m_pShowObject->AddComponent<CDraw3D>();
		m_pShowObject->Start();
	}

	//���W�̐ݒ�
	auto Transform = m_pShowObject->GetComponent<CTransform>();
	auto Draw = m_pShowObject->GetComponent<CDraw3D>();
	Transform->SetPosition(fPosX,fPosY);
	Transform->SetScale(1.0f,1.0f,1.0f);

	//���f���̐ݒ�
	if (m_nMapSta == B_1) {
		Transform->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		ModelManager* pModelManager = ModelManager::GetInstance();
		Draw->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));
	}
}

/**
* @fn		StageManager::Save
* @brief	�X�e�[�W���̏�������
* @param	(int)	���Ԗڂ̃X�e�[�W��
* @return	(bool)	���Ȃ��������߂���
*/
bool StageManager::Save(int Stage) {
	//�ۑ�����CSV�t�@�C�������߂�
	std::string file;
	switch (Stage) {
	case STAGE_1:file = STAGE_1_CSV; break;
	case STAGE_1_RE:file = STAGE_1_RE_CSV; break;
	default: break;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
	if (CSV == nullptr) {
		return false;
	}

	//�Z�[�u����
	if (CSV->Save(file) == false) {
		//�G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("stage_save"), NULL, MB_OK);
		return false;
	}
	else {
		switch (Stage) {
		case STAGE_1:MessageBox(GetMainWnd(), L"Stage_1.csv�ɕۑ����܂���", _T("����"), MB_OK); break;
		case STAGE_1_RE:MessageBox(GetMainWnd(), L"Stage_1_RE.csv�ɕۑ����܂���", _T("����"), MB_OK); break;
		default: break;
		}
	}

	return true;
}

/**
* @fn		StageManager::Load
* @brief	�X�e�[�W�̒n�`���̓ǂݎ��
* @param	(string)	�t�@�C����
* @return	(bool)	���Ȃ��ǂݍ��߂���
*/
bool StageManager::Load(std::string fileName) {

	//���[�h
	if (FileManager::GetInstance()->Load(fileName) == false) {
		//�G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("stage_load"), NULL, MB_OK);
		return false;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(fileName);

	//�Q�[���p�̔z��Ɋi�[����
	m_StageGrid.resize(CSV->GetRowSize());
	for (int y = 0;y <CSV->GetRowSize();++y) {
		m_StageGrid[y].resize(CSV->GetColumnSize(y));

		for (int x = 0;x < CSV->GetColumnSize(y);x++) {
			//�v�f���i�[����
			m_StageGrid[y][x] = CSV->GetInt(x,y);
		}

	}

	return true;
}
