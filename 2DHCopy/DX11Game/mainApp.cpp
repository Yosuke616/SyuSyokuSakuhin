//=============================================================================
//
// ���C������ [main.cpp]
// Author : SHIMIZU YOUSUKE
//
//=============================================================================
#include "mainApp.h"
#include "WinUser.h"
#include "resource.h"
#include "polygon.h"
#include "AssimpModel.h"
#include "mesh.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "SceneManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "FileManager.h"
#include "ResourceCSV.h"

#include "sceneGame.h"
#include "ObjInfo.h"

#include "Sound.h"

#include <process.h>


//-------- ���C�u�����̃����N
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")


//===== �}�N����` =====
#define CLASS_NAME		_T("AppClass")			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		_T("�A�E��i")			// �E�C���h�E�̃L���v�V������


//===== �v���g�^�C�v�錾 =====
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//	�v���V�[�W��
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);									//	WM_CREATE���b�Z�[�W�n���h��
HRESULT CreateBackBuffer();														//	�o�b�N�o�b�t�@����
void ReleaseBackBuffer();														//	�o�b�N�o�b�t�@���
HRESULT Init(HWND hWnd, BOOL bWindow);											//	������
void Uninit();																	//	�I��
void Update();																	//	�X�V
void Draw();																	//	�`��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(					//	Call from your application's message handler
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//===== �O���[�o���ϐ� =====
HWND						g_hWnd;							// ���C�� �E�B���h�E �n���h��
HINSTANCE					g_hInst;						// �C���X�^���X �n���h��

ID3D11Device*				g_pDevice;						// �f�o�C�X
ID3D11DeviceContext*		g_pDeviceContext;				// �f�o�C�X �R���e�L�X�g
IDXGISwapChain*				g_pSwapChain;					// �X���b�v�`�F�[��
ID3D11RenderTargetView*		g_pRenderTargetView;			// �t���[���o�b�t�@
UINT						g_uSyncInterval = 0;			// �������� (0:��, 1:�L)
ID3D11BlendState*			g_pBlendState[MAX_BLENDSTATE];	// �u�����h �X�e�[�g

ID3D11Texture2D*			g_pDepthStencilTexture;			// Z�o�b�t�@�p������
ID3D11DepthStencilView*		g_pDepthStencilView;			// Z�o�b�t�@
ID3D11RasterizerState*		g_pRs[MAX_CULLMODE];			// ���X�^���C�U �X�e�[�g
ID3D11DepthStencilState*	g_pDSS[2];						// Z/�X�e���V�� �X�e�[�g

int							g_nCountFPS;					// FPS�J�E���^

SceneManager*	g_pSceneManager = nullptr;
InputManager*	g_pInputManager = nullptr;


// ���C���֐�
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	//	���g�p�錾
	UNREFERENCED_PARAMETER(lpCmdLine);		//	���g�p�錾

	DWORD dwExecLastTime;					//	�Ō�Ɏ��s���������iexecutive�����s�j
	DWORD dwFPSLastTime;					//	0.5�b�̊Ԃ̃t���[���J�E���^
	DWORD dwCurrentTime;					//	���ݎ���
	DWORD dwFrameCount;						//	�t���[���J�E���^


	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRM)),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		nullptr
	};
	MSG msg;

	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBox(NULL, _T("COM�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
		return -1;
	}

	// �C���X�^���X �n���h���ۑ�
	g_hInst = hInstance;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�T�C�Y����E�B���h�E �T�C�Y�Z�o
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//	��ʃT�C�Y
	DWORD dwExStyle = 0;								//	�E�B���h�E�̊g���X�^�C��
	/*�E�B���h�E�X�^�C��
		�@WS_OVERLAPPED		�I�[�o�[���b�v�E�B���h�E�̓^�C�g���o�[�Ƙg
		�AWS_CAPTION			�^�C�g���o�[�����E�B���h�E���쐬
		�BWS_SYSMENU			�^�C�g���o�[��ɃE�B���h�E���j���[�����E�B���h�E���쐬
		�CWS_BORDER			�ׂ����E�������E�B���h�E���쐬
		�DWS_MINIMIZEBOX		�ŏ����{�^�������E�B���h�E���쐬
		�EWS_MAXIMIZEBOX		�ő剻�{�^�������E�B���h�E���쐬
	*/
	DWORD dwStyle = WS_VISIBLE | WS_POPUP;//WS_OVERLAPPED | WS_SYSMENU | WS_BORDER;
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);



	// �E�B���h�E�̍쐬
	g_hWnd = CreateWindowEx(
		dwExStyle,			//	�E�B���h�E�̊g���X�^�C��
		CLASS_NAME,			//	�֐��̌Ăяo���ɂ��쐬���ꂽ�E�B���h�E�N���X�̖��O��\��������ւ̃|�C���^
		WINDOW_NAME,		//	���̕����񂪃^�C�g���o�[�ɕ\�������
		dwStyle,			//	�E�B���h�E�X�^�C���i�E�B���h�E���j���[���Ƃ��������Ă���j
		CW_USEDEFAULT,		// �E�B���h�E�̍����W
		CW_USEDEFAULT,		// �E�B���h�E�̏���W
		GetSystemMetrics(SM_CXSCREEN),	// �E�B���h�E����
		GetSystemMetrics(SM_CYSCREEN),	// �E�B���h�E�c��
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// �t���[���J�E���g������
	timeBeginPeriod(1);									//	����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();		//	���Ԏ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// �E�B���h�E�̈ʒu
	MoveWindow(g_hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);

	//	�J�[�\�����\��
	//ShowCursor(FALSE);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(g_hWnd, true)))
	{
		return -1;
	}

	// ���b�Z�[�W���[�v
	for (;;)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//	�V�X�e���������~���b�P�ʂŎ擾
			dwCurrentTime = timeGetTime();

			//	1/2�b���ƂɎ��s
			if (dwCurrentTime - dwFPSLastTime >= 1000 / 2)	//	���ݎ����i�~���b�j - �t���[���J�E���^(�~���b) >= 1000�~���b / 2
			{
				//	�t���[�����[�g�@=�@���ݎ����i�b�j / 
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//	
				dwFrameCount = 0;
			}

			//	1/60�b���ƂɎ��s
			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)	//	���ݎ����i�~���b�j - �Ō�Ɏ��s���������i�~���b�j >= 1000�~���b / 60
			{
				dwExecLastTime = dwCurrentTime;	//	�Ō�Ɏ��s�������� = ���ݎ���

				// �X�V
				Update();

				// �`��
				Draw();
			}

			dwFrameCount++;						//	�t���[�������Z
		}
	}

	// �^�C�}�ݒ�����ɖ߂�
	timeEndPeriod(1);

	// �I������
	Uninit();

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, g_hInst);

	// COM�I������
	CoUninitialize();

	return (int)msg.wParam;
}

// ����������
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

#pragma region ���낢��ȏ�����
	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = bWindow;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&g_pSwapChain, &g_pDevice, nullptr, &g_pDeviceContext);
	if (FAILED(hr)) {
		return hr;
	}

	// �o�b�N�o�b�t�@����
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[2]);
	g_pDeviceContext->RSSetState(g_pRs[2]);

	// �u�����h �X�e�[�g����
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[0]);
	// �u�����h �X�e�[�g���� (�A���t�@ �u�����h�p)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// �u�����h �X�e�[�g���� (���Z�����p)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	// �[�x�X�e���V���X�e�[�g����
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);
#pragma endregion

	// �|���S���\��������
	hr = InitPolygon(g_pDevice);
	if (FAILED(hr)) return hr;

	// ���b�V��������
	hr = InitMesh();
	if (FAILED(hr)) return hr;

	// Assimp�p�V�F�[�_������
	if (!CAssimpModel::InitShader(g_pDevice)) return E_FAIL;

	// Imgui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// �J���[�e�[�}�ݒ�
	ImGui::StyleColorsDark();
	// �v���b�g�t�H�[��/�����_���̏�����
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);

	//	���͂̏�����
	g_pInputManager = InputManager::Instance();
	hr = g_pInputManager->Init();
	if (FAILED(hr))	MessageBox(g_hWnd, _T("dinput���������s"), NULL, MB_OK);

	//�T�E���h�f�[�^
	CSound::Init();
	CSound::SetVolume_BGM(0.5f);
	CSound::SetVolume_SE(1.0f);

	//	�V�[���}�l�[�W���[������
	g_pSceneManager = SceneManager::Instance();
	g_pSceneManager->Init();

	return hr;
}

// �I������
void Uninit()
{
	//�t�@�C���}�l�[�W���[���
	FileManager::Destroy();

	//	���f���J��
	ModelManager::Uninit();

	//	�e�N�X�`���J��
	TextureManager::Uninit();

	//	�V�[���J������
	g_pSceneManager->Uninit();

	// Imgui �I��
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�T�E���h����
	CSound::Fin();

	//	���͏I������
	g_pInputManager->Uninit();

	// ���b�V���I������
	UninitMesh();

	// Assimp�p�V�F�[�_�I������
	CAssimpModel::UninitShader();

	// �|���S���\���I������
	UninitPolygon();

	// �[�x�X�e���V���X�e�[�g���
	for (int i = 0; i < _countof(g_pDSS); ++i) {
		SAFE_RELEASE(g_pDSS[i]);
	}

	// �u�����h �X�e�[�g���
	for (int i = 0; i < MAX_BLENDSTATE; ++i) {
		SAFE_RELEASE(g_pBlendState[i]);
	}

	// ���X�^���C�U �X�e�[�g���
	for (int i = 0; i < MAX_CULLMODE; ++i) {
		SAFE_RELEASE(g_pRs[i]);
	}

	// �o�b�N�o�b�t�@���
	ReleaseBackBuffer();

	// �X���b�v�`�F�[�����
	SAFE_RELEASE(g_pSwapChain);

	// �f�o�C�X �R���e�L�X�g�̊J��
	SAFE_RELEASE(g_pDeviceContext);

	// �f�o�C�X�̊J��
	SAFE_RELEASE(g_pDevice);
}

// �X�V����
void Update()
{
	//	���͍X�V
	g_pInputManager->Update();	// �K��Update�֐��̐擪�Ŏ��s.

#ifdef SCENE_CHANGE
	InputManager* input = InputManager::Instance();
	if (input->GetKeyTrigger(DIK_SPACE))
	{
		StartFadeOutSceneChange(SCENE_ENDING);
	}
	else if (input->GetKeyTrigger(DIK_F9))
	{
		StartFadeOutSceneChange(SCENE_TITLE);
	}
	else if (input->GetKeyTrigger(DIK_F1))
	{
		SceneGame::GetInstance()->SetSeason(SPRING_1);
	}
	else if (input->GetKeyTrigger(DIK_F2))
	{
		SceneGame::GetInstance()->SetSeason(SPRING_2);
	}
	else if (input->GetKeyTrigger(DIK_F3))
	{
		SceneGame::GetInstance()->SetSeason(SUMMER_1);
	}
	else if (input->GetKeyTrigger(DIK_F4))
	{
		SceneGame::GetInstance()->SetSeason(SUMMER_2);
	}
	else if (input->GetKeyTrigger(DIK_F5))
	{
		SceneGame::GetInstance()->SetSeason(FALL_1);
	}
	else if (input->GetKeyTrigger(DIK_F6))
	{
		SceneGame::GetInstance()->SetSeason(FALL_2);
	}
	else if (input->GetKeyTrigger(DIK_F7))
	{
		SceneGame::GetInstance()->SetSeason(WINTER_1);
	}
	else if (input->GetKeyTrigger(DIK_F8))
	{
		SceneGame::GetInstance()->SetSeason(WINTER_2);
	}
#endif // SCENE_CHANGE

	//	�V�[���X�V
	g_pSceneManager->Update();

	//�T�E���h�X�V
	CSound::Update();

}

// �`�揈��
void Draw()
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pDeviceContext->ClearDepthStencilView(g_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

#ifdef _DEBUG
	//*** ImGui New Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	using namespace ImGui;
	SetNextWindowSize(ImVec2(160, 160), ImGuiCond_FirstUseEver);				//	�E�B���h�E�T�C�Y
	SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver, ImVec2(0, 0));	//	���W(���オ(0,0))
	Begin("ImGuiTest");															//	
	Text("FPS : %d", (int)(GetIO().Framerate));									//	FPS
	XMFLOAT2 world = CalcWorldSize();
	Text("%.0f, %.0f", world.x, world.y);
	End();																		//	�I��
#endif // _DEBUG


	//	�V�[���`��
	g_pSceneManager->Draw();

	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
#ifdef _DEBUG
	ImGui::Render();	//*** ImGui Render
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pSwapChain->Present(g_uSyncInterval, 0);
}

// �v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//	imgui�v���V�[�W��
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:					//----- �E�B���h�E���������ꂽ
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:				//----- �E�B���h�E�j���w��������
		PostQuitMessage(0);				// �V�X�e���ɃX���b�h�̏I����v��
		break;
	case WM_KEYDOWN:				//----- �L�[�{�[�h�������ꂽ
		//switch (wParam) {
		//case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
		//	PostMessage(hWnd, WM_CLOSE, 0, 0);	// [x]�������ꂽ�悤�ɐU����
		//	return 0;
		//}
		//break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;			// [Alt]+[Enter]����BEEP��}�~
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WM_CREATE���b�Z�[�W�n���h��
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// �N���C�A���g�̈�T�C�Y��SCREEN_WIDTH�~SCREEN_HEIGHT�ɍĐݒ�.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) {
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	// IME������
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1��Ԃ���CreateWindow[Ex]�����s����.
}

// �o�b�N�o�b�t�@����
HRESULT CreateBackBuffer(void)
{
	// �����_�[�^�[�Q�b�g�r���[����
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	// Z�o�b�t�@�p�e�N�X�`������
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = SCREEN_WIDTH;
	td.Height = SCREEN_HEIGHT;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = g_pDevice->CreateTexture2D(&td, nullptr, &g_pDepthStencilTexture);
	if (FAILED(hr)) {
		return hr;
	}

	// Z�o�b�t�@�^�[�Q�b�g�r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture,
		&dsvd, &g_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

// �o�b�N�o�b�t�@���
void ReleaseBackBuffer()
{
	if (g_pDeviceContext) {
		g_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pDepthStencilTexture);
	SAFE_RELEASE(g_pRenderTargetView);
}

// ���C�� �E�B���h�E �n���h���擾
HWND GetMainWnd()
{
	return g_hWnd;
}

// �C���X�^���X �n���h���擾
HINSTANCE GetInstance()
{
	return g_hInst;
}

// �f�o�C�X�擾
ID3D11Device* GetDevice()
{
	return g_pDevice;
}

// �f�o�C�X �R���e�L�X�g�擾
ID3D11DeviceContext* GetDeviceContext()
{
	return g_pDeviceContext;
}

// �[�x�o�b�t�@�L����������
void SetZBuffer(bool bEnable)
{
	g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1], 0);
}

// �[�x�o�b�t�@�X�V�L����������
void SetZWrite(bool bEnable)
{
	g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0], 0);
}

// �u�����h �X�e�[�g�ݒ�
void SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState], blendFactor, 0xffffffff);
	}
}

// �J�����O�ݒ�
void SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		g_pDeviceContext->RSSetState(g_pRs[nCullMode]);
	}
}

// �X�N���[�����W�����[���h���W�ɕϊ�
XMVECTOR* CalcScreenToWorld(
	XMVECTOR* pout,
	float Sx,			//	�X�N���[��X���W
	float Sy,			//	�X�N���[��Y���W
	float fZ,			//	�ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,		//	�X�N���[���̕�
	int Screen_h,		//	�X�N���[���̍���
	XMMATRIX View,		//	�r���[�}�g���b�N�X
	XMMATRIX Prj		//	�ˉe�s��
)
{
	XMMATRIX InvView, InvPrj, InvViewport;	//	�r���[�A�ˉe�A�r���[�|�[�g��Inverse(matrix)
	XMVECTOR Vview, Vprj, Vviewport;		//	�r���[�A�ˉe�A�r���[�|�[�g��Inverse(vector)

	//--- �e�s��̋t�s����Z�o
	//	�r���[�s��
	InvView = XMMatrixInverse(&Vview, View);
	//	�ˉe�s��
	InvPrj = XMMatrixInverse(&Vprj, Prj);
	//	�r���[�|�[�g�s��
	XMFLOAT4X4 VP;
	VP._11 = Screen_w / 2.0f;	VP._12 = 0.0f;				VP._13 = 0.0f;	VP._14 = 0.0f;
	VP._21 = 0.0f;				VP._22 = Screen_h / 2.0f;	VP._23 = 0.0f;	VP._24 = 0.0f;
	VP._31 = 0.0f;				VP._32 = 0.0f;				VP._33 = 1.0f;	VP._34 = 0.0f;
	VP._41 = 0.0f;				VP._42 = 0.0f;				VP._43 = 0.0f;	VP._44 = 1.0f;
	XMMATRIX matrixVP = XMLoadFloat4x4(&VP);		//	XMFLOAT4X4 �� XMMATRIX
	InvViewport = XMMatrixInverse(&Vviewport, matrixVP);

	//	�t�s��v�Z
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR vec = DirectX::XMVectorSet(Sx, Sy, fZ, 0.0f);
	*pout = DirectX::XMVector3TransformCoord(vec, tmp);

	return pout;
}


// XY���ʂƃX�N���[�����W�̌�_�Z�o�֐�
XMVECTOR* CalcScreenToXY(
	XMFLOAT3* pout,
	float Sx,
	float Sy,
	int Screen_w,
	int Screen_h,
	XMMATRIX View,
	XMMATRIX Prj
)
{
	XMVECTOR nearpos, farpos;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);	//���W(Sx, Sy)�̃j�A�N���b�v���W�v�Z
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);	//���W(Sx, Sy)�̃t�@�[�N���b�v���W�v�Z

	//--- �t�@�[�ƃj�A�̃x�N�g����
	XMVECTOR vecRay = farpos - nearpos;
	XMVECTOR NormalizeVectorRay = vecRay;
	DirectX::XMVector3Normalize(NormalizeVectorRay);	//	���K��

	//	XY���ʂƌ����Ȃ�����nullptr��Ԃ�
	XMFLOAT3 NormalizeRay;
	DirectX::XMStoreFloat3(&NormalizeRay, NormalizeVectorRay);
	if (NormalizeRay.z <= 0)
	{
		return nullptr;
	}

	//--- z=0�Ƃ̌�_
	XMVECTOR XYnormal = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR LP0 = DirectX::XMVector3Dot((-nearpos), XYnormal);	//	���ςŋ��������߂�
	XMVECTOR Lray = DirectX::XMVector3Dot(vecRay, XYnormal);	//	���ςŋ��������߂�
	XMVECTOR result;			// �v�Z����
	result = nearpos + (LP0 / Lray) * vecRay;


	DirectX::XMStoreFloat3(pout, result);	//	XMVECTOR�@�� XMFLOAT3

	return nullptr;//&result;
}

/**
 * @fn			CalcWorldSize
 * @brief				�X�N���[���̉f���Ă��郏�[���h�̉����ƍ������v�Z����
 * @return	XMFLOAT2	�X�N���[���̉f���Ă��郏�[���h�̉����ƍ���
*/
XMFLOAT2 CalcWorldSize()
{
	XMFLOAT2 world;

	// XY���ʂƃX�N���[�����W�̌�_�Z�o
	CCamera* camera = CCamera::Get();
	XMMATRIX view = DirectX::XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&camera->GetProjMatrix());
	XMFLOAT3 left_up;
	XMFLOAT3 right_down;
	//	����
	CalcScreenToXY(&left_up, -SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);
	//	�E��
	CalcScreenToXY(&right_down, SCREEN_CENTER_X, -SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

	//	�X�N���[���ɉf���Ă��郏�[���h�̉����ƍ��������߂�
	world.x = right_down.x - left_up.x;	//	�E�[�̃��[���h���W - ���[�̃��[���h���W
	world.y = left_up.y - right_down.y;	//	��[�̃��[���h���W - �E�[�̃��[���h���W

	return world;
}

/* 2022/1/1
						���[�h��ʗp�̃Q�b�^�[
*/
// �����_���^�[�Q�b�g�r���[�̎擾
ID3D11RenderTargetView* GetRenderTargetView()
{
	return g_pRenderTargetView;
}

// Z�o�b�t�@
ID3D11DepthStencilView*	GetDepthStencilView()
{
	return g_pDepthStencilView;
}

// �X���b�v�`�F�[��
IDXGISwapChain*	GetSwapChain()
{
	return g_pSwapChain;
}