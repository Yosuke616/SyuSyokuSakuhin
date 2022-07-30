//�C���N���[�h��
#include "InputManager.h"
#include "imgui.h"
#include <memory.h>

//���C�u���������N
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

//�O���[�o���ϐ�
/** @brief DirectInput�I�u�W�F�N�g*/
LPDIRECTINPUT8			lpDI			= nullptr;
/** @brief �L�[�{�[�h�f�o�C�X*/
LPDIRECTINPUTDEVICE8	lpKeyboard		= nullptr;
/** @brief �}�E�X�f�o�C�X*/
LPDIRECTINPUTDEVICE8	lpMouseDevice	= nullptr;

//�ÓI�����o�ϐ�
InputManager*::InputManager::m_pInstance = nullptr;

/**
* @fn		InputManager::InputManager
* @brief	�R���X�g���N�^�@�����o�ϐ��̏�����������
*/
InputManager::InputManager() {
	//�����o�ϐ��̏�����
	SecureZeroMemory(m_CurrentKeyState,sizeof(m_CurrentKeyState));
	SecureZeroMemory(m_PrevKeyState,sizeof(m_PrevKeyState));
	m_MousePos = XMFLOAT2(0.0f,0.0f);
}

/**
* @fn		InputManager::~InputManager
* @brief	�f�X�g���N�^�Ɋ���
*/
InputManager::~InputManager() {

}

/**
* @fn		InputManager::Instance
* @brief	�C���X�^���X��������ꏊ
* @return	(InputManager*)	�������ꂽ�C���X�^���X(�܂��͐V����������}�l�[�W���[)��Ԃ�
*/
InputManager* InputManager::Instance() {
	if (!m_pInstance) {
		m_pInstance = new InputManager();
	}
	return m_pInstance;
}

/**
* @fn		InputManager::Uninit
* @brief	�I������
*/
void InputManager::Uninit() {
	//�f�o�C�X�̐����~
	lpKeyboard->Unacquire();
	lpMouseDevice->Unacquire();

	//���
	lpMouseDevice->Release();
	lpKeyboard->Release();
	lpDI->Release();

	//�C���X�^���X�j��
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		InputManager::Init
* @brief	������
* @return	(HRESULT)	�����������s�������ǂ�����Ԃ�
*/
HRESULT InputManager::Init() {
	HRESULT hr = S_OK;

	// IDirectInput8�̍쐬(DirectInput8�̍\�z�ɐ��������ꍇ�ɂ��̃|�C���^�̓���̂�LPDIRECTINPUT8�̕ϐ��̃|�C���^)
	//�܂��ł���
	hr = DirectInput8Create(GetInstance(),DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&lpDI,NULL);
	if (FAILED(hr)) {
		//�쐬�Ɏ��s
		return -1;
	}

	//�L�[�{�[�h������
	hr = InitKeyBoard();
	if (FAILED(hr)) {
		return -1;
	}

	//�}�E�X�̏�����
	hr = InitMouse();
	if (FAILED(hr)) {
		return -1;
	}

	return hr;

}

/**
* @fn		InputManager::InitKeyBoard()
* @brief	�L�[�{�[�h�̏�����
* @return	���������
*/
HRESULT InputManager::InitKeyBoard()
{
	//	IDirectInputDevice8�̎擾
	HRESULT hr = lpDI->CreateDevice(
		GUID_SysKeyboard,	//��1�����͓��̓f�o�C�X��GUID�A�����ł̓f�t�H���g�̃V�X�e���L�[�{�[�h�Ƃ������Ƃ�GUID_SysKeyboard���`
		&lpKeyboard,		//��2�����ɂ͎擾�����f�o�C�X�I�u�W�F�N�g������ϐ��̃|�C���^
		NULL);
	if (FAILED(hr))
	{
		lpDI->Release();
		return -1;
	}

	//	���̓f�[�^�`���̃Z�b�g
	hr = lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	//	�r������̃Z�b�g
	hr = lpKeyboard->SetCooperativeLevel(GetMainWnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr)) {

		lpKeyboard->Release();
		lpDI->Release();
		return -1;
	}

	// ����J�n
	lpKeyboard->Acquire();

	return hr;
}

/**
* @fn		InputManager::InitMouse
* @brief	�}�E�X�̏�������������
* @return	(return)	�������������ǂ��� 
*/
HRESULT InputManager::InitMouse() {
	//�}�E�X�p�̃f�o�C�X�I�u�W�F�N�g���쐬
	HRESULT hr = lpDI->CreateDevice(
		GUID_SysMouse,
		&lpMouseDevice,
		NULL);
	if (FAILED(hr)) {
		lpDI->Release();
		return -1;
	}

	//���̓f�[�^�`���̃Z�b�g
	hr = lpMouseDevice->SetDataFormat(&c_dfDIMouse);
	//�f�[�^�t�H�[�}�b�g�Ɏ��s
	if (FAILED(hr)) {
		return -1;
	}

	//	�r������̃Z�b�g
	/*
		DISCL_EXCLUSIVE		���̃A�v���P�[�V�����͂��̓��̓f�o�C�X���擾�ł��Ȃ�
		DISCL_NONEXCLUSIVE	���̃A�v���P�[�V�����ł����̂܂ܓ��̓f�o�C�X�̎擾���ł���

		DISCL_FOREGROUND	�E�B���h�E���o�b�N�O���E���h�Ɉړ�������f�o�C�X�̎擾���ł��Ȃ�
		DISCL_BACKGROUND	�E�B���h�E����A�N�e�B�u��Ԃł��f�o�C�X���擾�ł���

	*/
	hr = lpMouseDevice->SetCooperativeLevel(GetMainWnd(),
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if(FAILED(hr)){
		return - 1;
	}

	//	�f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	hr = lpMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	//�f�o�C�X�̐ݒ�Ɏ��s
	if (FAILED(hr)) {
		return false;
	}

	//����J�n
	lpMouseDevice->Acquire();

	return hr;

}

/**
* @fn		InputManager::Update
* @brief	�X�V����
*/
void InputManager::Update() {
	//�L�[�{�[�h���͂̍X�V
	UpdateKeyBoard();

	//�}�E�X�̍X�V
	UpdateMouse();
}

/**
* @fn		InputManager::UpdateKeyBoard
* @brief	�L�[�{�[�h�̍X�V
*/
void InputManager::UpdateKeyBoard() {
	//	�L�[���͏��̑ޔ�
	memcpy_s(m_PrevKeyState, KEY_MAX, m_CurrentKeyState, KEY_MAX);

	//	�L�[���͏��̃N���A
	SecureZeroMemory(m_CurrentKeyState, sizeof(m_CurrentKeyState));

	//	�L�[���͏��̎擾
	HRESULT ret = lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	if (FAILED(ret))	// ���s�Ȃ�ĊJ�����Ă�����x�擾
	{
		//	����ĊJ
		lpKeyboard->Acquire();

		//	�L�[���͏��̎擾
		lpKeyboard->GetDeviceState(sizeof(m_CurrentKeyState), m_CurrentKeyState);
	}
}

/**
* @fn		InputManager::UpdateMouse
* @brief	�}�E�X�̍X�V
*/
void InputManager::UpdateMouse() {
	//	�}�E�X���̑ޔ�
	m_PrevMouseState = m_CurrentMouseState;

	//	�}�E�X���̎擾
	HRESULT	hr = lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		//	����ĊJ
		lpMouseDevice->Acquire();

		//	�}�E�X�����Ď擾
		hr = lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	}

	//	�}�E�X���W���擾����
	POINT p;
	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(GetMainWnd(), &p);

	m_MousePos = XMFLOAT2((float)(p.x - SCREEN_CENTER_X), -(float)(p.y - SCREEN_CENTER_Y));
}

/**
* @fn		InputManager::GetKeyPress
* @brief	�L�[�{�[�h�������ꑱ���Ă��邩�ǂ���
* @param	(unsigned char)	�ǂ̃L�[��������Ă��邩
* @return	(bool)	������Ă��邩�ǂ���
*/
bool InputManager::GetKeyPress(unsigned char key) {
	return (m_CurrentKeyState[key] & 0x80);
}

/**
* @fn		InputManager::GetKeyRelease
* @brief	�L�[�{�[�h�g���K�[
* @param	(unsigned char)	�ǂ̃L�[�������ꂽ���̏��
* @return	(bool)	������Ă��邩�ǂ���
*/
bool InputManager::GetKeyTrigger(unsigned char key) {
	//	�O�t���[���ɉ�����Ă��Ȃ� && ���݉�����Ă���
	return (m_CurrentKeyState[key] & 0x80) &&
		!(m_PrevKeyState[key] & 0x80);
}

/**
* @fn		InputManager::GetKeyRelease
* @brief	�L�[�{�[�h�������[�X���ꂽ
* @param	(unsigned char)	�ǂ̃L�[�������[�X���ꂽ��
* @return	(bool)	���������ǂ���
*/
bool InputManager::GetKeyRelease(unsigned char key) {
	//	�O�t���[���ɉ�����Ă��� && ���݉�����Ă��Ȃ�
	return ((m_PrevKeyState[key] & 0x80) &&
		!(m_CurrentKeyState[key] & 0x80));
}

/**
* @fn		InputManager::GetMouseButton
* @brief	�}�E�X�̂ǂ̃{�^���������ꂽ��
* @param	(MOUSE_BUTTON)	�ǂ̃{�^���������ꂽ���̏��
* @return	(bool)	������Ă������ǂ����̐^�U��Ԃ�
*/
bool InputManager::GetMouseButton(MOUSE_BUTTON Btn) {
	return (m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}

/**
* @fn		InputManager::GetMouseTrigger
* @brief	�}�E�X����x���������ꂽ���̃g���K�[���
* @detail	�O�t���[���ɃN���b�N����Ă��Ȃ������݃N���b�N���ꂽ
* @param	(MOUSE_BUTTON)	�ǂ̃{�^���������ꂽ���̏��
* @return	(bool)	�{�^���������ꂽ���̐^�U 
*/
bool InputManager::GetMouseTrigger(MOUSE_BUTTON Btn) {
	return (!(m_PrevMouseState.rgbButtons[Btn] & 0x80) &&
			m_CurrentMouseState.rgbButtons[Btn] & 0x80);
}

/**
* @fn		InputManager::GetMouseRelease
* @brief	�}�E�X�̃{�^���������ꂽ���ǂ���
* @detail	�O�t���[���N���b�N����Ă��邩���݃N���b�N����Ă��Ȃ�
* @param	(MOUSE_BUTTON) �ǂ̃{�^���������ꂽ���̏��
* @return	(bool)	�����ꂽ���ǂ����̐^�U
*/
bool InputManager::GetMouseRelease(MOUSE_BUTTON Btn) {
	return (m_PrevMouseState.rgbButtons[Btn] & 0x80 &&
			!(m_CurrentMouseState.rgbButtons[Btn] & 0x80));
}

/**
* @fn		InputManager::GetMousePos const
* @brief	�}�E�X���W�̎擾
* @return	(XMFLOAT2)	��ʏ�̂ǂ̍��W�ɂ��邩
*/
XMFLOAT2 InputManager::GetMousePos() const {
	return m_MousePos;
}

/**
* @fn		InputManager::GetMouseVelocity const
* @brief	�}�E�X�̈ړ��ʂ̎擾
* @return	(XMFLOAT2)	�ǂ̒��x�ړ��������̏��(�c��)
*/
XMFLOAT2 InputManager::GetMouseVelocity() const{
	return XMFLOAT2((float)m_CurrentMouseState.lX, (float)-m_CurrentMouseState.lY);
}

/**
* @fn		InputManager::Draw
* @brief	�`��(�f�o�b�O�Ŏg�������}��)
*/
void InputManager::Draw() {

}