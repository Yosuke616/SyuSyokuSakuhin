/**�C���N���[�h��**/
#include "Load.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include "ObjInfo.h"
#include <process.h>

/**�萔��`**/
/** @brief �V�[���̏���������u�ŏI������ꍇ�҂��Ă��炤�b��*/
#define LOAD_FRAME	(3)

/**�ÓI�����o�ϐ�**/
/** @brief �X���b�h�n���h��*/
HANDLE Load::m_handle;
/** @brief ���[�h��*/
bool Load::m_bLoading = false;
/** @brief ���[�h���Ԃ̃J�E���g*/
int Load::m_nLoadCount = 0;
/** @brief ���[�h��ʂ̃I�u�W�F�N�g���X�g*/
std::vector<Object*> Load::m_LoadObject;

/**
* @fn		Load::Load
* @brief	�R���X�g���N�^�ɂ��Ă���
*/
Load::Load() {

}

/**
* @fn		Load::~Load
* @brief	�f�X�g���N�^�͊y��������
*/
Load::~Load() {

}

/**
* @fn		Load::Init
* @brief	���[�h��ʂ̏�����
*/
void Load::Init() {
	//�J�����̏�����
	CCamera::Get()->Init();

	//�e�N�X�`���Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);

	//�I�u�W�F�N�g������
#pragma region ---�w�i
	//�I�u�W�F�N�g����
	Object* BG = new Object("BG",UPDATE_BG,DRAW_BG);
	//�R���|�[�l���g�̒ǉ�
	auto draw_BG = BG->AddComponent<CDraw2D>();
	BG->AddComponent<CTransform>();
	//�I�u�W�F�N�g�̐ݒ�
	draw_BG->SetColor(0.0f,0.0f,0.0f);
	draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//���X�g�ɒǉ�
	m_LoadObject.push_back(BG);
#pragma endregion

#pragma region ---��������
	//�I�u�W�F�N�g����
	Object* DXChan = new Object("Run",UPDATE_PLAYER,DRAW_PLAYER);
	//�R���|�[�l���g�̒ǉ�
	auto trans_DX = DXChan->AddComponent<CTransform>();
	auto draw_DX = DXChan->AddComponent<CAnimMesh>();
	//�I�u�W�F�N�g�̐ݒ�
	trans_DX->SetPosition(0.0f,50.0f);
	draw_DX->SetTexture(pTexManager->GetTexture(DXCHAN_RUN_TEX_NUM));
	draw_DX->SetSize(100.0f,100.0f);
	draw_DX->SetAnimSplit(7,2);
	draw_DX->SetAnimFrame(1);
	draw_DX->SetLoop(true);
	//���X�g�ɒǉ�
	m_LoadObject.push_back(DXChan);
#pragma endregion

	//�S�ẴI�u�W�F�N�g�̏�����
	for (auto object:m_LoadObject) {
		object->Start();
	}
}

/**
* @fn		Load::Uninit
* @brief	�I������
*/
void Load::Uninit() {
	//�I�u�W�F�N�g�̉��
	for (auto object : m_LoadObject) {
		delete object;
	}
	//���X�g���J���ɂ���
	m_LoadObject.clear();
}

/**
* @fn		Load::Begin
* @brief	���[�h��ʂ̊J�n
*/
void Load::Begin() {
	//���[�h��ʂ̏�����
	Load::Init();

	//���[�h��ʊJ�n�̂��m�点
	m_bLoading = true;

	//���[�h��ʂ̃X���b�h�𐶐�
	m_handle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Load::LoadScreen, NULL, 0, NULL);
}

/**
* @fn		Load::End
* @brief	���[�h��ʂ̏I��
*/
void Load::End() {
	//���[�h�������I����Ă��w�肳�ꂽ�t���[���̓��[�h����
	while (m_nLoadCount < LOAD_FRAME * 60) {
		//�ҋ@
		Sleep(1);
	}

	//���[�h��ʏI���̂��m�点
	m_bLoading = false;

	//���[�h��ʂ̃X���b�h�I������܂ő҂�
	WaitForSingleObject(m_handle, INFINITE);

	//�n���h�������
	CloseHandle(m_handle);

	//���[�h��ʂ̃I�u�W�F�N�g���X�g�̉��
	Load::Uninit();
}

/**
* @fn		Load::LoadScreen
* @brief	���[�h��ʂ̍X�V�ƕ`��
* @return	(unsigned __stdcall)	�Ȃ��ɂ���
*/
unsigned __stdcall Load::LoadScreen() {
	//�t���[������p
	DWORD dwExecLastTime = timeGetTime();	//	�Ō�Ɏ��s���������iexecutive�����s�j
	DWORD dwCurrentTime = 0;				//	���ݎ���

	// ���[�h���Ԃ̃J�E���^�̏�����
	m_nLoadCount = 0;

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
			m_nLoadCount++;

		}
	}

	//�X���b�h�I��
	_endthreadex(0);

	return 0;
}

/**
* @fn		Load::GetLoadCount
* @brief	���[�h��ʂ̃t���[�����擾
* @return	(int)	�t���[����
*/
int Load::GetLoadCount() {
	return m_nLoadCount;
}
