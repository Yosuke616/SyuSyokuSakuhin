/**
* @file		InputManager.h
* @brief	�R���g���[���[��L�[�{�[�h����ł���悤�ɂ���
* @author	�u���z�S
* @date		2022/6/30
*/

//�C���N���[�h�K�[�h
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

//�C���N���[�h��
#include "mainApp.h"
#define   DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//�萔��`
#define KEY_MAX (256)

/**�񋓑̐錾**/
enum MOUSE_BUTTON {
	MOUSE_L,	//���N���b�N
	MOUSE_R,	//�E�N���b�N
	MOSUE_C		//�z�C�[��
};

//�N���X��`
class InputManager {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X�p*/
	static InputManager* m_pInstance;
	/** @brief ���݂̃L�[�{�[�h����*/
	unsigned char m_CurrentKeyState[KEY_MAX];
	/** @brief �O�t���[���̃L�[�{�[�h����*/
	unsigned char m_PrevKeyState[KEY_MAX];
	/** @brief �}�E�X�̌��݂̓��͏��*/
	DIMOUSESTATE m_CurrentMouseState;
	/** @brief �}�E�X��1�t���[���O�̓��͏��*/
	DIMOUSESTATE m_PrevMouseState;
	/** @brief �}�E�X���W*/
	XMFLOAT2 m_MousePos;

	//�����o�֐�
	/** @brief �L�[�{�[�h������*/
	HRESULT InitKeyBoard();
	/** @brief �L�[�{�[�h�X�V*/
	void UpdateKeyBoard();
	/** @brief �}�E�X�̏�����*/
	HRESULT InitMouse();
	/** @brief �}�E�X�̍X�V*/
	void UpdateMouse();

protected:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	InputManager();

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X����*/
	static InputManager* Instance();
	/** @brief �f�X�g���N�^*/
	~InputManager();
	/** @brief ������*/
	HRESULT Init();
	/** @brief �I��*/
	void Uninit();
	/** @brief �X�V*/
	void Update();
	/** @brief �`��*/
	void Draw();

	/** @brief �L�[�{�[�h�̃v���X��Ԃ��擾*/
	bool GetKeyPress(unsigned char key);
	/** @brief �L�[�{�[�h�̃g���K�[��Ԃ��擾*/
	bool GetKeyTrigger(unsigned char key);
	/** @brief �L�[�{�[�h�̃����[�X��Ԃ��擾*/
	bool GetKeyRelease(unsigned char key);

	/** @brief �}�E�X�̃v���X��Ԃ��擾*/
	bool GetMouseButton(MOUSE_BUTTON Btn);
	/** @brief �}�E�X�̃g���K�[��Ԃ��擾*/
	bool GetMouseTrigger(MOUSE_BUTTON Btn);
	/** @brief �}�E�X�̃����[�X��Ԃ��擾*/
	bool GetMouseRelease(MOUSE_BUTTON Btn);

	/** @brief �}�E�X�̍��W�̎擾*/
	XMFLOAT2 GetMousePos() const;
	/** @brief �}�E�X�̈ړ��ʂ̎擾*/
	XMFLOAT2 GetMouseVelocity() const;
};

#endif