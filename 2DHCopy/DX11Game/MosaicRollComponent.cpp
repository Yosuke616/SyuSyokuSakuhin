/**インクルード部**/
#include "MosaicRollComponent.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "InputManager.h"
#include "Camera.h"
#include "ObjInfo.h"
#include <process.h>

/**定数定義**/
/** @brief シーンの初期化が一瞬で終わった場合待っている時間*/
#define MOSAIC_FRAM	(0.5f)

/**静的メンバ変数**/
/** @brief スレッドハンドル*/
HANDLE MosaicRoll::m_handle;
/** @brief ロード中*/
bool MosaicRoll::m_bLoading = false;
/** @brief ロード時間のカウント*/
float MosaicRoll::m_fLoadCount = 0;
/** @brief ロード画面のオブジェクトリスト*/
std::vector<Object*> MosaicRoll::m_LoadObject;
/** @brief モザイクを出し続けるためのカウンタ*/
float MosaicRoll::m_fMosaicCnt = 0;
/** @brief 一番最初だけ初期化する*/
bool MosaicRoll::m_bFirstFlg = false;

/**
* @fn		MosaicRoll::MosaicRoll
* @brief	コンストラクタの欠如と
*/
MosaicRoll::MosaicRoll() {

}

/**
* @fn		MosaicRoll::MosaicRoll
* @brief	形だけのデストラクタ
*/
MosaicRoll::~MosaicRoll() {

}

/**
* @fn		MosaicRoll::Init
* @brief	モザイクロールの初期化
*/
void MosaicRoll::Init() {
	//テクスチャ読込
	TextureManager* pTexManager = TextureManager::GetInstance();

	//背景
#pragma region ---背景
	//オブジェクト生成
	Object* BG = new Object("Mosaic", MAX_UPDATE_ORDER, MAX_DRAW_ORDER);
	//コンポーネント追加
	auto draw_BG = BG->AddComponent<CAnimMesh>();
	auto trans_BG = BG->AddComponent<CTransform>();
	//オブジェクトの設定
	trans_BG->SetPosition(ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x, ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.y);
	draw_BG->SetTexture(TextureManager::GetInstance()->GetTexture(MOSAIC_TEX_NUM));
	draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	draw_BG->SetAnimSplit(3,3);
	draw_BG->SetAnimFrame(1);
	draw_BG->SetLoop(true);
	//リストに追加
	m_LoadObject.push_back(BG);
#pragma endregion
	//全てのオブジェクトの初期化
	for (auto object:m_LoadObject) {
		object->Start();
	}
}

/**
* @fn		MosaicRoll::Uninit
* @brief	終了処理
*/
void MosaicRoll::Uninit() {
	//オブジェクトの解放
	for (auto object : m_LoadObject) {
		delete object;
	}
	//リストをカラにする
	m_LoadObject.clear();
}

/**
* @fn		MosaicRoll::Begin
* @brief	ロード画面の開始
*/
void MosaicRoll::Begin() {
	//ロード画面の初期化
	MosaicRoll::Init();
	//ロード画面開始のお知らせ
	m_bLoading = true;
	//ロード画面のスレッドを生成
	m_handle = (HANDLE)_beginthreadex(NULL,0,(_beginthreadex_proc_type)MosaicRoll::LoadScreen,NULL,0,NULL);
}

/**
* @fn		MosaicRoll::End
* @brief	ロード画面の終了
*/
void MosaicRoll::End() {
	//ロードが早く終わっても指定されたフレームはロードする
	while (m_fLoadCount < MOSAIC_FRAM * 60) {
		//待機
		Sleep(1);
	}

	//ロード画面終了のお知らせ
	m_bLoading = false;
	//ロード画面のスレッド終了するまで待つ
	WaitForSingleObject(m_handle,INFINITE);
	//ハンドルを閉じる
	CloseHandle(m_handle);
	//ロード画面のオブジェクトリスト
	MosaicRoll::Uninit();
}

/**
* @fn		MosaicRoll::LoadScreen
* @brief	ロード画面の更新と描画
* @return	(unsigned __stdcall)	なぁにこれ
*/
unsigned __stdcall MosaicRoll::LoadScreen() {
	//フレーム制御用
	DWORD dwExecLastTime = timeGetTime();	//	最後に実行した時刻（executive＝実行）
	DWORD dwCurrentTime = 0;				//	現在時刻

	// ロード時間のカウンタの初期化
	m_fLoadCount = 0;

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
			m_fLoadCount += 0.5f;

		}
	}

	//スレッド終了
	_endthreadex(0);

	return 0;
}

/**
* @fn		MosaicRoll::GetLoadCount
* @brief	ロード画面のフレーム数取得
* @return	(int)	フレーム
*/
int MosaicRoll::GetLoadCount() {
	return m_fLoadCount;
}

/**
* @fn		MosaicRoll::Mosaic_Damage
* @brief	ダメージを受けたときにモザイクが流れるようにする
*/
void MosaicRoll::Mosaic_Damage() {
	//一番最初だけ初期化をかける
	if (!m_bFirstFlg) {
		//初期化は2回も通さないぜ
		m_bFirstFlg = true;
		//モザイクの初期化
		MosaicRoll::Init();
		//時間の初期化
		m_fMosaicCnt = 0.0f;

		//全てのオブジェクトの停止
		for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
			obj->StopUpdate();
			obj->StopDraw();
		}

	}

	while (m_bFirstFlg) {
		//モザイクの更新をする
		for (auto obj : m_LoadObject) {
			obj->Update();
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
		
		//モザイクの描画
		for (auto obj : m_LoadObject) {
			obj->Draw();
		}
		
		// バックバッファとフロントバッファの入れ替え
		GetSwapChain()->Present(0, 0);

		m_fMosaicCnt += 0.5f;

		if (m_fMosaicCnt > MOSAIC_FRAM * 60) {
			m_bFirstFlg = false;
			MosaicRoll::Uninit();

			//オブジェクトを全て再開する
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			break;
		}
	}
}

/**
* @fn		MosaicRoll::Mosaic
* @brief	いつでもいい感じの時間モザイクを流せる
* @param	(float)	モザイクをどの位のフレーム流したいか			
*/
void MosaicRoll::Mosaic(float Num) {
	//一番最初だけ初期化をかける
	if (!m_bFirstFlg) {
		//初期化は2回も通さないぜ
		m_bFirstFlg = true;
		//モザイクの初期化
		MosaicRoll::Init();
		//時間の初期化
		m_fMosaicCnt = 0.0f;

		//全てのオブジェクトの停止
		for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
			obj->StopUpdate();
			obj->StopDraw();
		}

	}

	while (m_bFirstFlg) {
		//モザイクの更新をする
		for (auto obj : m_LoadObject) {
			obj->Update();
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

		//モザイクの描画
		for (auto obj : m_LoadObject) {
			obj->Draw();
		}

		// バックバッファとフロントバッファの入れ替え
		GetSwapChain()->Present(0, 0);

		m_fMosaicCnt += 0.5f;

		if (m_fMosaicCnt > Num * 60) {
			m_bFirstFlg = false;
			MosaicRoll::Uninit();

			//オブジェクトを全て再開する
			for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
				obj->Use();
			}
			break;
		}
	}
}