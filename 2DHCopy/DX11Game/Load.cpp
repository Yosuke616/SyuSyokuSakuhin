/**インクルード部**/
#include "Load.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include "ObjInfo.h"
#include <process.h>

/**定数定義**/
/** @brief シーンの初期化が一瞬で終わった場合待ってもらう秒数*/
#define LOAD_FRAME	(3)

/**静的メンバ変数**/
/** @brief スレッドハンドル*/
HANDLE Load::m_handle;
/** @brief ロード中*/
bool Load::m_bLoading = false;
/** @brief ロード時間のカウント*/
int Load::m_nLoadCount = 0;
/** @brief ロード画面のオブジェクトリスト*/
std::vector<Object*> Load::m_LoadObject;

/**
* @fn		Load::Load
* @brief	コンストラクタにしてやんよ
*/
Load::Load() {

}

/**
* @fn		Load::~Load
* @brief	デストラクタは楽しいぞい
*/
Load::~Load() {

}

/**
* @fn		Load::Init
* @brief	ロード画面の初期化
*/
void Load::Init() {
	//カメラの初期化
	CCamera::Get()->Init();

	//テクスチャ読込
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);

	//オブジェクト初期化
#pragma region ---背景
	//オブジェクト生成
	Object* BG = new Object("BG",UPDATE_BG,DRAW_BG);
	//コンポーネントの追加
	auto draw_BG = BG->AddComponent<CDraw2D>();
	BG->AddComponent<CTransform>();
	//オブジェクトの設定
	draw_BG->SetColor(0.0f,0.0f,0.0f);
	draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//リストに追加
	m_LoadObject.push_back(BG);
#pragma endregion

#pragma region ---何かしら
	//オブジェクト生成
	Object* DXChan = new Object("Run",UPDATE_PLAYER,DRAW_PLAYER);
	//コンポーネントの追加
	auto trans_DX = DXChan->AddComponent<CTransform>();
	auto draw_DX = DXChan->AddComponent<CAnimMesh>();
	//オブジェクトの設定
	trans_DX->SetPosition(0.0f,50.0f);
	draw_DX->SetTexture(pTexManager->GetTexture(DXCHAN_RUN_TEX_NUM));
	draw_DX->SetSize(100.0f,100.0f);
	draw_DX->SetAnimSplit(7,2);
	draw_DX->SetAnimFrame(1);
	draw_DX->SetLoop(true);
	//リストに追加
	m_LoadObject.push_back(DXChan);
#pragma endregion

	//全てのオブジェクトの初期化
	for (auto object:m_LoadObject) {
		object->Start();
	}
}

/**
* @fn		Load::Uninit
* @brief	終了処理
*/
void Load::Uninit() {
	//オブジェクトの解放
	for (auto object : m_LoadObject) {
		delete object;
	}
	//リストをカラにする
	m_LoadObject.clear();
}

/**
* @fn		Load::Begin
* @brief	ロード画面の開始
*/
void Load::Begin() {
	//ロード画面の初期化
	Load::Init();

	//ロード画面開始のお知らせ
	m_bLoading = true;

	//ロード画面のスレッドを生成
	m_handle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Load::LoadScreen, NULL, 0, NULL);
}

/**
* @fn		Load::End
* @brief	ロード画面の終了
*/
void Load::End() {
	//ロードが早く終わっても指定されたフレームはロードする
	while (m_nLoadCount < LOAD_FRAME * 60) {
		//待機
		Sleep(1);
	}

	//ロード画面終了のお知らせ
	m_bLoading = false;

	//ロード画面のスレッド終了するまで待つ
	WaitForSingleObject(m_handle, INFINITE);

	//ハンドルを閉じる
	CloseHandle(m_handle);

	//ロード画面のオブジェクトリストの解放
	Load::Uninit();
}

/**
* @fn		Load::LoadScreen
* @brief	ロード画面の更新と描画
* @return	(unsigned __stdcall)	なぁにこれ
*/
unsigned __stdcall Load::LoadScreen() {
	//フレーム制御用
	DWORD dwExecLastTime = timeGetTime();	//	最後に実行した時刻（executive＝実行）
	DWORD dwCurrentTime = 0;				//	現在時刻

	// ロード時間のカウンタの初期化
	m_nLoadCount = 0;

	//ロード画面終了のお知らせが来るまでループ
	while (m_bLoading) {
		//システム時刻をミリ単位で取得
		dwCurrentTime = timeGetTime();

		//フレーム制御(1/60秒ごとに実行)
		if (dwCurrentTime - dwExecLastTime >= 1000 / 60) {
			dwExecLastTime = dwCurrentTime;	//	最後に実行した時刻 = 現在時刻

			//--- 更新
			InputManager::Instance()->Update();		// 入力
			CCamera::Get()->Update();				// カメラ
			for (auto object : m_LoadObject) {		// オブジェクト
				object->Update();
			}

			//スクリーン座標からワールド座標に変換
			InputManager* Input = InputManager::Instance();
			CCamera* pCamera = CCamera::Get();
			XMFLOAT2 vPos = Input->GetMousePos();
			XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
			XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
			XMFLOAT3 vWorldPos;
			CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);

			// バックバッファ＆Ｚバッファのクリア
			float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
			GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
			GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//--- 描画
			for (auto object : m_LoadObject)	// オブジェクト
				object->Draw();

			// バックバッファとフロントバッファの入れ替え
			GetSwapChain()->Present(0, 0);

			// ロード時間をカウント
			m_nLoadCount++;

		}
	}

	//スレッド終了
	_endthreadex(0);

	return 0;
}

/**
* @fn		Load::GetLoadCount
* @brief	ロード画面のフレーム数取得
* @return	(int)	フレーム数
*/
int Load::GetLoadCount() {
	return m_nLoadCount;
}
