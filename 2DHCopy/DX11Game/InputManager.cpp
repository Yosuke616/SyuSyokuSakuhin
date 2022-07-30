//インクルード部
#include "InputManager.h"
#include "imgui.h"
#include <memory.h>

//ライブラリリンク
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//グローバル変数
/** @brief DirectInputオブジェクト*/
LPDIRECTINPUT8			lpDI			= nullptr;
/** @brief キーボードデバイス*/
LPDIRECTINPUTDEVICE8	lpKeyboard		= nullptr;
/** @brief マウスデバイス*/
LPDIRECTINPUTDEVICE8	lpMouseDevice	= nullptr;

//静的メンバ変数
InputManager*::InputManager::m_pInstance = nullptr;

/**
* @fn		InputManager::InputManager
* @brief	コンストラクタ　メンバ変数の初期化が多い
*/
InputManager::InputManager() {
	//メンバ変数の初期化
	SecureZeroMemory(m_CurrentKeyState,sizeof(m_CurrentKeyState));
	SecureZeroMemory(m_PrevKeyState,sizeof(m_PrevKeyState));
	m_MousePos = XMFLOAT2(0.0f,0.0f);
}

/**
* @fn		InputManager::~InputManager
* @brief	デストラクタに感謝
*/
InputManager::~InputManager() {

}

/**
* @fn		InputManager::Instance
* @brief	インスタンス生成する場所
* @return	(InputManager*)	生成されたインスタンス(または新しく作ったマネージャー)を返す
*/
InputManager* InputManager::Instance() {
	if (!m_pInstance) {
		m_pInstance = new InputManager();
	}
	return m_pInstance;
}

/**
* @fn		InputManager::Uninit
* @brief	終了処理
*/
void InputManager::Uninit() {
	//デバイスの制御停止
	lpKeyboard->Unacquire();
	lpMouseDevice->Unacquire();

	//解放
	lpMouseDevice->Release();
	lpKeyboard->Release();
	lpDI->Release();

	//インスタンス破棄
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		InputManager::Init
* @brief	初期化
* @return	(HRESULT)	初期化が失敗したかどうかを返す
*/
HRESULT InputManager::Init() {
	HRESULT hr = S_OK;

	// IDirectInput8の作成(DirectInput8の構築に成功した場合にそのポインタの入るのはLPDIRECTINPUT8の変数のポインタ)
	//つまり謎である
	hr = DirectInput8Create(GetInstance(),DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&lpDI,NULL);
	if (FAILED(hr)) {
		//作成に失敗
		return -1;
	}

	//キーボード初期化
	hr = InitKeyBoard();
	if (FAILED(hr)) {
		return -1;
	}

	//マウスの初期化
	hr = InitMouse();
	if (FAILED(hr)) {
		return -1;
	}

	return hr;

}

/**
* @fn		InputManager::InitKeyBoard()
* @brief	キーボードの初期化
* @return	初期化やつ
*/
HRESULT InputManager::InitKeyBoard()
{
	//	IDirectInputDevice8の取得
	HRESULT hr = lpDI->CreateDevice(
		GUID_SysKeyboard,	//第1引数は入力デバイスのGUID、ここではデフォルトのシステムキーボードということでGUID_SysKeyboardを定義
		&lpKeyboard,		//第2引数には取得したデバイスオブジェクトが入る変数のポインタ
		NULL);
	if (FAILED(hr))
	{
		lpDI->Release();
		return -1;
	}

	//	入力データ形式のセット
	hr = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	//	排他制御のセット
	hr = lpKeyboard->SetCooperativeLevel(GetMainWnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr)) {

		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	// 動作開始
	lpKeyboard->Acquire();

	return hr;
}

/**
* @fn		InputManager::InitMouse
* @brief	マウスの情報を初期化する
* @return	(return)	初期化成功かどうか 
*/
HRESULT InputManager::InitMouse() {
	//マウス用のデバイスオブジェクトを作成
	HRESULT hr = lpDI->CreateDevice(
		GUID_SysMouse,
		&lpMouseDevice,
		NULL);
	if (FAILED(hr)) {
		lpDI->Release();
		return -1;
	}

	//入力データ形式のセット
	hr = lpMouseDevice->SetDataFormat(&c_dfDIMouse);
	//データフォーマットに失敗
	if (FAILED(hr)) {
		return -1;
	}

	//	排他制御のセット
	/*
		DISCL_EXCLUSIVE		他のアプリケーションはその入力デバイスを取得できない
		DISCL_NONEXCLUSIVE	他のアプリケーションでもそのまま入力デバイスの取得ができる

		DISCL_FOREGROUND	ウィンドウがバックグラウンドに移動したらデバイスの取得ができない
		DISCL_BACKGROUND	ウィンドウが非アクティブ状態でもデバイスが取得できる

	*/
	hr = lpMouseDevice->SetCooperativeLevel(GetMainWnd(),
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hr)){
		return - 1;
	}

	//	デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	hr = lpMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	//デバイスの設定に失敗
	if (FAILED(hr)) {
		return false;
	}

	//動作開始
	lpMouseDevice->Acquire();

	return hr;

}

/**
* @fn		InputManager::Update
* @brief	更新処理
*/
void InputManager::Update() {
	//キーボード入力の更新
	UpdateKeyBoard();

	//マウスの更新
	UpdateMouse();
}

/**
* @fn		InputManager::UpdateKeyBoard
* @brief	キーボードの更新
*/
void InputManager::UpdateKeyBoard() {
	//	キー入力情報の退避
	memcpy_s(m_PrevKeyState, KEY_MAX, m_CurrentKeyState, KEY_MAX);

	//	キー入力情報のクリア
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));

	//	キー入力情報の取得
	HRESULT ret = lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	if (FAILED(ret))	// 失敗なら再開させてもう一度取得
	{
		//	動作再開
		lpKeyboard->Acquire();

		//	キー入力情報の取得
		lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	}
}

/**
* @fn		InputManager::UpdateMouse
* @brief	マウスの更新
*/
void InputManager::UpdateMouse() {
	//	マウス情報の退避
	m_PrevMouseState = m_CurrentMouseState;

	//	マウス情報の取得
	HRESULT	hr = lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		//	動作再開
		lpMouseDevice->Acquire();

		//	マウス情報を再取得
		hr = lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	//	マウス座標を取得する
	POINT p;
	GetCursorPos(&p);

	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(GetMainWnd(), &p);

	m_MousePos = XMFLOAT2((float)(p.x - SCREEN_CENTER_X), -(float)(p.y - SCREEN_CENTER_Y));
}

/**
* @fn		InputManager::GetKeyPress
* @brief	キーボードが押され続けているかどうか
* @param	(unsigned char)	どのキーが押されているか
* @return	(bool)	押されているかどうか
*/
bool InputManager::GetKeyPress(unsigned char key) {
	return (m_CurrentKeyState[key] & 0x80);
}

/**
* @fn		InputManager::GetKeyRelease
* @brief	キーボードトリガー
* @param	(unsigned char)	どのキーが押されたかの情報
* @return	(bool)	押されているかどうか
*/
bool InputManager::GetKeyTrigger(unsigned char key) {
	//	前フレームに押されていない && 現在押されている
	return (m_CurrentKeyState[key] & 0x80) &&
		!(m_PrevKeyState[key] & 0x80);
}

/**
* @fn		InputManager::GetKeyRelease
* @brief	キーボードがリリースされた
* @param	(unsigned char)	どのキーがリリースされたか
* @return	(bool)	離したかどうか
*/
bool InputManager::GetKeyRelease(unsigned char key) {
	//	前フレームに押されている && 現在押されていない
	return ((m_PrevKeyState[key] & 0x80) &&
		!(m_CurrentKeyState[key] & 0x80));
}

/**
* @fn		InputManager::GetMouseButton
* @brief	マウスのどのボタンが押されたか
* @param	(MOUSE_BUTTON)	どのボタンが押されたかの情報
* @return	(bool)	押されていたかどうかの真偽を返す
*/
bool InputManager::GetMouseButton(MOUSE_BUTTON Btn) {
	return (m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}

/**
* @fn		InputManager::GetMouseTrigger
* @brief	マウスが一度だけ押されたかのトリガー情報
* @detail	前フレームにクリックされていないかつ現在クリックされた
* @param	(MOUSE_BUTTON)	どのボタンが押されたかの情報
* @return	(bool)	ボタンが押されたかの真偽 
*/
bool InputManager::GetMouseTrigger(MOUSE_BUTTON Btn) {
	return (!(m_PrevMouseState.rgbButtons[Btn] & 0x80) &&
			m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}

/**
* @fn		InputManager::GetMouseRelease
* @brief	マウスのボタンが離されたかどうか
* @detail	前フレームクリックされているかつ現在クリックされていない
* @param	(MOUSE_BUTTON) どのボタンが離されたかの情報
* @return	(bool)	離されたかどうかの真偽
*/
bool InputManager::GetMouseRelease(MOUSE_BUTTON Btn) {
	return (m_PrevMouseState.rgbButtons[Btn] & 0x80 &&
			!(m_CurrentMouseState.rgbButtons[Btn] & 0x80));
}

/**
* @fn		InputManager::GetMousePos const
* @brief	マウス座標の取得
* @return	(XMFLOAT2)	画面上のどの座標にいるか
*/
XMFLOAT2 InputManager::GetMousePos() const {
	return m_MousePos;
}

/**
* @fn		InputManager::GetMouseVelocity const
* @brief	マウスの移動量の取得
* @return	(XMFLOAT2)	どの程度移動したかの情報(縦横)
*/
XMFLOAT2 InputManager::GetMouseVelocity() const{
	return XMFLOAT2((float)m_CurrentMouseState.lX, (float)-m_CurrentMouseState.lY);
}

/**
* @fn		InputManager::Draw
* @brief	描画(デバッグで使いたいマン)
*/
void InputManager::Draw() {

}