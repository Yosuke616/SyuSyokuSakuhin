/**�C���N���[�h��**/
#include "MosaicRollComponent.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include "ObjInfo.h"
#include <process.h>

/**�萔��`**/
/** @brief �V�[���̏���������u�ŏI������ꍇ�҂��Ă��鎞��*/
#define MOSAIC_FRAM	(0.5f)

/**�ÓI�����o�ϐ�**/
/** @brief �X���b�h�n���h��*/
HANDLE MosaicRoll::m_handle;
/** @brief ���[�h��*/
bool MosaicRoll::m_bLoading = false;
/** @brief ���[�h���Ԃ̃J�E���g*/
float MosaicRoll::m_fLoadCount = 0;
/** @brief ���[�h��ʂ̃I�u�W�F�N�g���X�g*/
std::vector<Object*> MosaicRoll::m_LoadObject;
/** @brief ���U�C�N���o�������邽�߂̃J�E���^*/
float MosaicRoll::m_fMosaicCnt = 0;
/** @brief ��ԍŏ���������������*/
bool MosaicRoll::m_bFirstFlg = false;

/**
* @fn		MosaicRoll::MosaicRoll
* @brief	�R���X�g���N�^�̌��@��
*/
MosaicRoll::MosaicRoll() {

}

/**
* @fn		MosaicRoll::MosaicRoll
* @brief	�`�����̃f�X�g���N�^
*/
MosaicRoll::~MosaicRoll() {

}

/**
* @fn		MosaicRoll::Init
* @brief	���U�C�N���[���̏�����
*/
void MosaicRoll::Init() {
	//�e�N�X�`���Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	//�w�i
#pragma region ---�w�i
	//�I�u�W�F�N�g����
	Object* BG = new Object("Mosaic", MAX_UPDATE_ORDER, MAX_DRAW_ORDER);
	//�R���|�[�l���g�ǉ�
	auto draw_BG = BG->AddComponent<CAnimMesh>();
	auto trans_BG = BG->AddComponent<CTransform>();
	//�I�u�W�F�N�g�̐ݒ�
	trans_BG->SetPosition(ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x, ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.y);
	draw_BG->SetTexture(TextureManager::GetInstance()->GetTexture(MOSAIC_TEX_NUM));
	draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	draw_BG->SetAnimSplit(3,3);
	draw_BG->SetAnimFrame(1);
	draw_BG->SetLoop(true);
	//���X�g�ɒǉ�
	m_LoadObject.push_back(BG);
#pragma endregion
	//�S�ẴI�u�W�F�N�g�̏�����
	for (auto object:m_LoadObject) {
		object->Start();
	}
}

/**
* @fn		MosaicRoll::Uninit
* @brief	�I������
*/
void MosaicRoll::Uninit() {
	//�I�u�W�F�N�g�̉��
	for (auto object : m_LoadObject) {
		delete object;
	}
	//���X�g���J���ɂ���
	m_LoadObject.clear();
}

/**
* @fn		MosaicRoll::Begin
* @brief	���[�h��ʂ̊J�n
*/
void MosaicRoll::Begin() {
	//���[�h��ʂ̏�����
	MosaicRoll::Init();
	//���[�h��ʊJ�n�̂��m�点
	m_bLoading = true;
	//���[�h��ʂ̃X���b�h�𐶐�
	m_handle = (HANDLE)_beginthreadex(NULL,0,(_beginthreadex_proc_type)MosaicRoll::LoadScreen,NULL,0,NULL);
}

/**
* @fn		MosaicRoll::End
* @brief	���[�h��ʂ̏I��
*/
void MosaicRoll::End() {
	//���[�h�������I����Ă��w�肳�ꂽ�t���[���̓��[�h����
	while (m_fLoadCount < MOSAIC_FRAM * 60) {
		//�ҋ@
		Sleep(1);
	}

	//���[�h��ʏI���̂��m�点
	m_bLoading = false;
	//���[�h��ʂ̃X���b�h�I������܂ő҂�
	WaitForSingleObject(m_handle,INFINITE);
	//�n���h�������
	CloseHandle(m_handle);
	//���[�h��ʂ̃I�u�W�F�N�g���X�g
	MosaicRoll::Uninit();
}

/**
* @fn		MosaicRoll::LoadScreen
* @brief	���[�h��ʂ̍X�V�ƕ`��
* @return	(unsigned __stdcall)	�Ȃ��ɂ���
*/
unsigned __stdcall MosaicRoll::LoadScreen() {
	//�t���[������p
	DWORD dwExecLastTime = timeGetTime();	//	�Ō�Ɏ��s���������iexecutive�����s�j
	DWORD dwCurrentTime = 0;				//	���ݎ���

	// ���[�h���Ԃ̃J�E���^�̏�����
	m_fLoadCount = 0;

	//���[�h��ʏI���̂��m�点������܂Ń��[�v
	while (m_bLoading) {
		//�V�X�e���������~���P�ʂŎ擾
		dwCurrentTime = timeGetTime();

		//�t���[������(1/60�b���ƂɎ��s)
		if (dwCurrentTime - dwExecLastTime >= 1000 / 60) {
			dwExecLastTime = dwCurrentTime;	//	�Ō�Ɏ��s�������� = ���ݎ���

			//--- �X�V
			InputManager::Instance()->Update();		// ����
			CCamera::Get()->Update();				// �J����
			for (auto object : m_LoadObject) {		// �I�u�W�F�N�g
				object->Update();
			}

			//�X�N���[�����W���烏�[���h���W�ɕϊ�
			InputManager* Input = InputManager::Instance();
			CCamera* pCamera = CCamera::Get();
			XMFLOAT2 vPos = Input->GetMousePos();
			XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
			XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
			XMFLOAT3 vWorldPos;
			CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

			// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
			float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
			GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
			GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//--- �`��
			for (auto object : m_LoadObject)	// �I�u�W�F�N�g
				object->Draw();

			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
			GetSwapChain()->Present(0, 0);

			// ���[�h���Ԃ��J�E���g
			m_fLoadCount += 0.5f;

		}
	}

	//�X���b�h�I��
	_endthreadex(0);

	return 0;
}

/**
* @fn		MosaicRoll::GetLoadCount
* @brief	���[�h��ʂ̃t���[�����擾
* @return	(int)	�t���[��
*/
int MosaicRoll::GetLoadCount() {
	return m_fLoadCount;
}

/**
* @fn		MosaicRoll::Mosaic_Damage
* @brief	�_���[�W���󂯂��Ƃ��Ƀ��U�C�N�������悤�ɂ���
*/
void MosaicRoll::Mosaic_Damage() {
	//��ԍŏ�������������������
	if (!m_bFirstFlg) {
		//��������2����ʂ��Ȃ���
		m_bFirstFlg = true;
		//���U�C�N�̏�����
		MosaicRoll::Init();
		//���Ԃ̏�����
		m_fMosaicCnt = 0.0f;

		//�S�ẴI�u�W�F�N�g�̒�~
		for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
			obj->StopUpdate();
			obj->StopDraw();
		}

	}

	while (m_bFirstFlg) {
		//���U�C�N�̍X�V������
		for (auto obj : m_LoadObject) {
			obj->Update();
		}

		//�X�N���[�����W���烏�[���h���W�ɕϊ�
		InputManager* Input = InputManager::Instance();
		CCamera* pCamera = CCamera::Get();
		XMFLOAT2 vPos = Input->GetMousePos();
		XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
		XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
		XMFLOAT3 vWorldPos;
		CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
		GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
		GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		//���U�C�N�̕`��
		for (auto obj : m_LoadObject) {
			obj->Draw();
		}
		
		// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		GetSwapChain()->Present(0, 0);

		m_fMosaicCnt += 0.5f;

		if (m_fMosaicCnt > MOSAIC_FRAM * 60) {
			m_bFirstFlg = false;
			MosaicRoll::Uninit();

			//�I�u�W�F�N�g��S�čĊJ����
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			break;
		}
	}
}

/**
* @fn		MosaicRoll::Mosaic
* @brief	���ł����������̎��ԃ��U�C�N�𗬂���
* @param	(float)	���U�C�N���ǂ̈ʂ̃t���[������������			
*/
void MosaicRoll::Mosaic(float Num) {
	//��ԍŏ�������������������
	if (!m_bFirstFlg) {
		//��������2����ʂ��Ȃ���
		m_bFirstFlg = true;
		//���U�C�N�̏�����
		MosaicRoll::Init();
		//���Ԃ̏�����
		m_fMosaicCnt = 0.0f;

		//�S�ẴI�u�W�F�N�g�̒�~
		for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
			obj->StopUpdate();
			obj->StopDraw();
		}

	}

	while (m_bFirstFlg) {
		//���U�C�N�̍X�V������
		for (auto obj : m_LoadObject) {
			obj->Update();
		}

		//�X�N���[�����W���烏�[���h���W�ɕϊ�
		InputManager* Input = InputManager::Instance();
		CCamera* pCamera = CCamera::Get();
		XMFLOAT2 vPos = Input->GetMousePos();
		XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
		XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
		XMFLOAT3 vWorldPos;
		CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
		GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
		GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//���U�C�N�̕`��
		for (auto obj : m_LoadObject) {
			obj->Draw();
		}

		// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		GetSwapChain()->Present(0, 0);

		m_fMosaicCnt += 0.5f;

		if (m_fMosaicCnt > Num * 60) {
			m_bFirstFlg = false;
			MosaicRoll::Uninit();

			//�I�u�W�F�N�g��S�čĊJ����
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			break;
		}
	}
}