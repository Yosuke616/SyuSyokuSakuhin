//=============================================================================
//
// メイン処理 [main.cpp]
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


//-------- ライブラリのリンク
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")


//===== マクロ定義 =====
#define CLASS_NAME		_T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME		_T("就職作品")			// ウインドウのキャプション名


//===== プロトタイプ宣言 =====
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//	プロシージャ
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);									//	WM_CREATEメッセージハンドラ
HRESULT CreateBackBuffer();														//	バックバッファ生成
void ReleaseBackBuffer();														//	バックバッファ解放
HRESULT Init(HWND hWnd, BOOL bWindow);											//	初期化
void Uninit();																	//	終了
void Update();																	//	更新
void Draw();																	//	描画
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(					//	Call from your application's message handler
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//===== グローバル変数 =====
HWND						g_hWnd;							// メイン ウィンドウ ハンドル
HINSTANCE					g_hInst;						// インスタンス ハンドル

ID3D11Device*				g_pDevice;						// デバイス
ID3D11DeviceContext*		g_pDeviceContext;				// デバイス コンテキスト
IDXGISwapChain*				g_pSwapChain;					// スワップチェーン
ID3D11RenderTargetView*		g_pRenderTargetView;			// フレームバッファ
UINT						g_uSyncInterval = 0;			// 垂直同期 (0:無, 1:有)
ID3D11BlendState*			g_pBlendState[MAX_BLENDSTATE];	// ブレンド ステート

ID3D11Texture2D*			g_pDepthStencilTexture;			// Zバッファ用メモリ
ID3D11DepthStencilView*		g_pDepthStencilView;			// Zバッファ
ID3D11RasterizerState*		g_pRs[MAX_CULLMODE];			// ラスタライザ ステート
ID3D11DepthStencilState*	g_pDSS[2];						// Z/ステンシル ステート

int							g_nCountFPS;					// FPSカウンタ

SceneManager*	g_pSceneManager = nullptr;
InputManager*	g_pInputManager = nullptr;


// メイン関数
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	//	未使用宣言
	UNREFERENCED_PARAMETER(lpCmdLine);		//	未使用宣言

	DWORD dwExecLastTime;					//	最後に実行した時刻（executive＝実行）
	DWORD dwFPSLastTime;					//	0.5秒の間のフレームカウンタ
	DWORD dwCurrentTime;					//	現在時刻
	DWORD dwFrameCount;						//	フレームカウンタ


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

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBox(NULL, _T("COMの初期化に失敗しました。"), _T("error"), MB_OK);
		return -1;
	}

	// インスタンス ハンドル保存
	g_hInst = hInstance;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域サイズからウィンドウ サイズ算出
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//	画面サイズ
	DWORD dwExStyle = 0;								//	ウィンドウの拡張スタイル
	/*ウィンドウスタイル
		①WS_OVERLAPPED		オーバーラップウィンドウはタイトルバーと枠
		②WS_CAPTION			タイトルバーを持つウィンドウを作成
		③WS_SYSMENU			タイトルバー上にウィンドウメニューを持つウィンドウを作成
		④WS_BORDER			細い境界線を持つウィンドウを作成
		⑤WS_MINIMIZEBOX		最小化ボタンを持つウィンドウを作成
		⑥WS_MAXIMIZEBOX		最大化ボタンを持つウィンドウを作成
	*/
	DWORD dwStyle = WS_VISIBLE | WS_POPUP;//WS_OVERLAPPED | WS_SYSMENU | WS_BORDER;
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);



	// ウィンドウの作成
	g_hWnd = CreateWindowEx(
		dwExStyle,			//	ウィンドウの拡張スタイル
		CLASS_NAME,			//	関数の呼び出しにより作成されたウィンドウクラスの名前を表す文字列へのポインタ
		WINDOW_NAME,		//	この文字列がタイトルバーに表示される
		dwStyle,			//	ウィンドウスタイル（ウィンドウメニュー情報とかが入っている）
		CW_USEDEFAULT,		// ウィンドウの左座標
		CW_USEDEFAULT,		// ウィンドウの上座標
		GetSystemMetrics(SM_CXSCREEN),	// ウィンドウ横幅
		GetSystemMetrics(SM_CYSCREEN),	// ウィンドウ縦幅
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// フレームカウント初期化
	timeBeginPeriod(1);									//	分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();		//	時間取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// ウィンドウの位置
	MoveWindow(g_hWnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), true);

	//	カーソルを非表示
	//ShowCursor(FALSE);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if (FAILED(Init(g_hWnd, true)))
	{
		return -1;
	}

	// メッセージループ
	for (;;)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//	システム時刻をミリ秒単位で取得
			dwCurrentTime = timeGetTime();

			//	1/2秒ごとに実行
			if (dwCurrentTime - dwFPSLastTime >= 1000 / 2)	//	現在時刻（ミリ秒） - フレームカウンタ(ミリ秒) >= 1000ミリ秒 / 2
			{
				//	フレームレート　=　現在時刻（秒） / 
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//	
				dwFrameCount = 0;
			}

			//	1/60秒ごとに実行
			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)	//	現在時刻（ミリ秒） - 最後に実行した時刻（ミリ秒） >= 1000ミリ秒 / 60
			{
				dwExecLastTime = dwCurrentTime;	//	最後に実行した時刻 = 現在時刻

				// 更新
				Update();

				// 描画
				Draw();
			}

			dwFrameCount++;						//	フレーム数加算
		}
	}

	// タイマ設定を元に戻す
	timeEndPeriod(1);

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, g_hInst);

	// COM終了処理
	CoUninitialize();

	return (int)msg.wParam;
}

// 初期化処理
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

#pragma region いろいろな初期化
	// デバイス、スワップチェーンの作成
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

	// バックバッファ生成
	hr = CreateBackBuffer();
	if (FAILED(hr)) {
		return hr;
	}

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;	// カリング無し(両面描画)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[0]);
	rd.CullMode = D3D11_CULL_FRONT;	// 前面カリング(裏面描画)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[1]);
	rd.CullMode = D3D11_CULL_BACK;	// 背面カリング(表面描画)
	g_pDevice->CreateRasterizerState(&rd, &g_pRs[2]);
	g_pDeviceContext->RSSetState(g_pRs[2]);

	// ブレンド ステート生成
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
	// ブレンド ステート生成 (アルファ ブレンド用)
	//BlendDesc.AlphaToCoverageEnable = TRUE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[1]);
	// ブレンド ステート生成 (加算合成用)
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[2]);
	// ブレンド ステート生成 (減算合成用)
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc, &g_pBlendState[3]);
	SetBlendState(BS_ALPHABLEND);

	// 深度ステンシルステート生成
	CD3D11_DEFAULT def;
	CD3D11_DEPTH_STENCIL_DESC dsd(def);
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	dsd2.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);
#pragma endregion

	// ポリゴン表示初期化
	hr = InitPolygon(g_pDevice);
	if (FAILED(hr)) return hr;

	// メッシュ初期化
	hr = InitMesh();
	if (FAILED(hr)) return hr;

	// Assimp用シェーダ初期化
	if (!CAssimpModel::InitShader(g_pDevice)) return E_FAIL;

	// Imgui初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// カラーテーマ設定
	ImGui::StyleColorsDark();
	// プラットフォーム/レンダラの初期化
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(g_pDevice, g_pDeviceContext);

	//	入力の初期化
	g_pInputManager = InputManager::Instance();
	hr = g_pInputManager->Init();
	if (FAILED(hr))	MessageBox(g_hWnd, _T("dinput初期化失敗"), NULL, MB_OK);

	//サウンドデータ
	CSound::Init();
	CSound::SetVolume_BGM(0.5f);
	CSound::SetVolume_SE(1.0f);

	//	シーンマネージャー初期化
	g_pSceneManager = SceneManager::Instance();
	g_pSceneManager->Init();

	return hr;
}

// 終了処理
void Uninit()
{
	//ファイルマネージャー解放
	FileManager::Destroy();

	//	モデル開放
	ModelManager::Uninit();

	//	テクスチャ開放
	TextureManager::Uninit();

	//	シーン開放処理
	g_pSceneManager->Uninit();

	// Imgui 終了
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//サウンド処理
	CSound::Fin();

	//	入力終了処理
	g_pInputManager->Uninit();

	// メッシュ終了処理
	UninitMesh();

	// Assimp用シェーダ終了処理
	CAssimpModel::UninitShader();

	// ポリゴン表示終了処理
	UninitPolygon();

	// 深度ステンシルステート解放
	for (int i = 0; i < _countof(g_pDSS); ++i) {
		SAFE_RELEASE(g_pDSS[i]);
	}

	// ブレンド ステート解放
	for (int i = 0; i < MAX_BLENDSTATE; ++i) {
		SAFE_RELEASE(g_pBlendState[i]);
	}

	// ラスタライザ ステート解放
	for (int i = 0; i < MAX_CULLMODE; ++i) {
		SAFE_RELEASE(g_pRs[i]);
	}

	// バックバッファ解放
	ReleaseBackBuffer();

	// スワップチェーン解放
	SAFE_RELEASE(g_pSwapChain);

	// デバイス コンテキストの開放
	SAFE_RELEASE(g_pDeviceContext);

	// デバイスの開放
	SAFE_RELEASE(g_pDevice);
}

// 更新処理
void Update()
{
	//	入力更新
	g_pInputManager->Update();	// 必ずUpdate関数の先頭で実行.

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

	//	シーン更新
	g_pSceneManager->Update();

	//サウンド更新
	CSound::Update();

}

// 描画処理
void Draw()
{
	// バックバッファ＆Ｚバッファのクリア
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
	SetNextWindowSize(ImVec2(160, 160), ImGuiCond_FirstUseEver);				//	ウィンドウサイズ
	SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver, ImVec2(0, 0));	//	座標(左上が(0,0))
	Begin("ImGuiTest");															//	
	Text("FPS : %d", (int)(GetIO().Framerate));									//	FPS
	XMFLOAT2 world = CalcWorldSize();
	Text("%.0f, %.0f", world.x, world.y);
	End();																		//	終了
#endif // _DEBUG


	//	シーン描画
	g_pSceneManager->Draw();

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);
	SetBlendState(BS_ALPHABLEND);
#ifdef _DEBUG
	ImGui::Render();	//*** ImGui Render
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

	// バックバッファとフロントバッファの入れ替え
	g_pSwapChain->Present(g_uSyncInterval, 0);
}

// プロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//	imguiプロシージャ
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:					//----- ウィンドウが生成された
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:				//----- ウィンドウ破棄指示がきた
		PostQuitMessage(0);				// システムにスレッドの終了を要求
		break;
	case WM_KEYDOWN:				//----- キーボードが押された
		//switch (wParam) {
		//case VK_ESCAPE:					// [ESC]キーが押された
		//	PostMessage(hWnd, WM_CLOSE, 0, 0);	// [x]が押されたように振舞う
		//	return 0;
		//}
		//break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;			// [Alt]+[Enter]時のBEEPを抑止
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// WM_CREATEメッセージハンドラ
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// クライアント領域サイズをSCREEN_WIDTH×SCREEN_HEIGHTに再設定.
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

	// IME無効化
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1を返すとCreateWindow[Ex]が失敗する.
}

// バックバッファ生成
HRESULT CreateBackBuffer(void)
{
	// レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	// Zバッファ用テクスチャ生成
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

	// Zバッファターゲットビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilTexture,
		&dsvd, &g_pDepthStencilView);
	if (FAILED(hr)) {
		return hr;
	}

	// 各ターゲットビューをレンダーターゲットに設定
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// ビューポート設定
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

// バックバッファ解放
void ReleaseBackBuffer()
{
	if (g_pDeviceContext) {
		g_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(g_pDepthStencilTexture);
	SAFE_RELEASE(g_pRenderTargetView);
}

// メイン ウィンドウ ハンドル取得
HWND GetMainWnd()
{
	return g_hWnd;
}

// インスタンス ハンドル取得
HINSTANCE GetInstance()
{
	return g_hInst;
}

// デバイス取得
ID3D11Device* GetDevice()
{
	return g_pDevice;
}

// デバイス コンテキスト取得
ID3D11DeviceContext* GetDeviceContext()
{
	return g_pDeviceContext;
}

// 深度バッファ有効無効制御
void SetZBuffer(bool bEnable)
{
	g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1], 0);
}

// 深度バッファ更新有効無効制御
void SetZWrite(bool bEnable)
{
	g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0], 0);
}

// ブレンド ステート設定
void SetBlendState(int nBlendState)
{
	if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState], blendFactor, 0xffffffff);
	}
}

// カリング設定
void SetCullMode(int nCullMode)
{
	if (nCullMode >= 0 && nCullMode < MAX_CULLMODE) {
		g_pDeviceContext->RSSetState(g_pRs[nCullMode]);
	}
}

// スクリーン座標をワールド座標に変換
XMVECTOR* CalcScreenToWorld(
	XMVECTOR* pout,
	float Sx,			//	スクリーンX座標
	float Sy,			//	スクリーンY座標
	float fZ,			//	射影空間でのZ値（0～1）
	int Screen_w,		//	スクリーンの幅
	int Screen_h,		//	スクリーンの高さ
	XMMATRIX View,		//	ビューマトリックス
	XMMATRIX Prj		//	射影行列
)
{
	XMMATRIX InvView, InvPrj, InvViewport;	//	ビュー、射影、ビューポートのInverse(matrix)
	XMVECTOR Vview, Vprj, Vviewport;		//	ビュー、射影、ビューポートのInverse(vector)

	//--- 各行列の逆行列を算出
	//	ビュー行列
	InvView = XMMatrixInverse(&Vview, View);
	//	射影行列
	InvPrj = XMMatrixInverse(&Vprj, Prj);
	//	ビューポート行列
	XMFLOAT4X4 VP;
	VP._11 = Screen_w / 2.0f;	VP._12 = 0.0f;				VP._13 = 0.0f;	VP._14 = 0.0f;
	VP._21 = 0.0f;				VP._22 = Screen_h / 2.0f;	VP._23 = 0.0f;	VP._24 = 0.0f;
	VP._31 = 0.0f;				VP._32 = 0.0f;				VP._33 = 1.0f;	VP._34 = 0.0f;
	VP._41 = 0.0f;				VP._42 = 0.0f;				VP._43 = 0.0f;	VP._44 = 1.0f;
	XMMATRIX matrixVP = XMLoadFloat4x4(&VP);		//	XMFLOAT4X4 → XMMATRIX
	InvViewport = XMMatrixInverse(&Vviewport, matrixVP);

	//	逆行列計算
	XMMATRIX tmp = InvViewport * InvPrj * InvView;
	XMVECTOR vec = DirectX::XMVectorSet(Sx, Sy, fZ, 0.0f);
	*pout = DirectX::XMVector3TransformCoord(vec, tmp);

	return pout;
}


// XY平面とスクリーン座標の交点算出関数
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
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);	//座標(Sx, Sy)のニアクリップ座標計算
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);	//座標(Sx, Sy)のファークリップ座標計算

	//--- ファーとニアのベクトル差
	XMVECTOR vecRay = farpos - nearpos;
	XMVECTOR NormalizeVectorRay = vecRay;
	DirectX::XMVector3Normalize(NormalizeVectorRay);	//	正規化

	//	XY平面と交わらない時はnullptrを返す
	XMFLOAT3 NormalizeRay;
	DirectX::XMStoreFloat3(&NormalizeRay, NormalizeVectorRay);
	if (NormalizeRay.z <= 0)
	{
		return nullptr;
	}

	//--- z=0との交点
	XMVECTOR XYnormal = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR LP0 = DirectX::XMVector3Dot((-nearpos), XYnormal);	//	内積で距離を求める
	XMVECTOR Lray = DirectX::XMVector3Dot(vecRay, XYnormal);	//	内積で距離を求める
	XMVECTOR result;			// 計算結果
	result = nearpos + (LP0 / Lray) * vecRay;


	DirectX::XMStoreFloat3(pout, result);	//	XMVECTOR　→ XMFLOAT3

	return nullptr;//&result;
}

/**
 * @fn			CalcWorldSize
 * @brief				スクリーンの映っているワールドの横幅と高さを計算する
 * @return	XMFLOAT2	スクリーンの映っているワールドの横幅と高さ
*/
XMFLOAT2 CalcWorldSize()
{
	XMFLOAT2 world;

	// XY平面とスクリーン座標の交点算出
	CCamera* camera = CCamera::Get();
	XMMATRIX view = DirectX::XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&camera->GetProjMatrix());
	XMFLOAT3 left_up;
	XMFLOAT3 right_down;
	//	左上
	CalcScreenToXY(&left_up, -SCREEN_CENTER_X, SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);
	//	右下
	CalcScreenToXY(&right_down, SCREEN_CENTER_X, -SCREEN_CENTER_Y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

	//	スクリーンに映っているワールドの横幅と高さを求める
	world.x = right_down.x - left_up.x;	//	右端のワールド座標 - 左端のワールド座標
	world.y = left_up.y - right_down.y;	//	上端のワールド座標 - 右端のワールド座標

	return world;
}

/* 2022/1/1
						ロード画面用のゲッター
*/
// レンダラターゲットビューの取得
ID3D11RenderTargetView* GetRenderTargetView()
{
	return g_pRenderTargetView;
}

// Zバッファ
ID3D11DepthStencilView*	GetDepthStencilView()
{
	return g_pDepthStencilView;
}

// スワップチェーン
IDXGISwapChain*	GetSwapChain()
{
	return g_pSwapChain;
}