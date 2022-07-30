//=============================================================================
//
// ���C������ [main.h]
// Author : SHIMIZU YOUSUKE
//
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "resource.h"

#include <math.h>
// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����

#define SinDeg(degree)	sinf(XMConvertToRadians(degree))	//	Sin�̒l  �����͊p�x
#define CosDeg(degree)	cosf(XMConvertToRadians(degree))	//	Cos�̒l	�����͊p�x

// 2022/01/21 ����
enum Direct
{
	DIR_NONE = 0,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_UP,
	DIR_DOWN,
};


enum EBlendState {
	BS_NONE = 0,							// ��������������
	BS_ALPHABLEND,							// ����������
	BS_ADDITIVE,							// ���Z����
	BS_SUBTRACTION,							// ���Z����

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// �J�����O���Ȃ�
	CULLMODE_CW,							// �O�ʃJ�����O
	CULLMODE_CCW,							// �w�ʃJ�����O

	MAX_CULLMODE
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT3 nor;		// �@���x�N�g��
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HWND GetMainWnd();							//	���C�� �E�B���h�E �n���h���擾
HINSTANCE GetInstance();					//	�C���X�^���X �n���h���擾
ID3D11Device* GetDevice();					//	�f�o�C�X�擾
ID3D11DeviceContext* GetDeviceContext();	//	�f�o�C�X �R���e�L�X�g�擾
void SetBlendState(int nBlendState);		//	�u�����h�X�e�[�g�̐ݒ�
void SetCullMode(int nCullMode);			//	�J�����O���[�h�̐ݒ�
void SetZBuffer(bool bEnable);				//	�[�x�o�b�t�@�L����������
void SetZWrite(bool bEnable);				//	�[�x�o�b�t�@�X�V�L����������


#define ���c

// �X�N���[�����W�����[���h���W�ɕϊ�
XMVECTOR* CalcScreenToWorld(
	XMVECTOR* pout,		//	�i�[��
	float Sx,			//	�X�N���[��X���W
	float Sy,			//	�X�N���[��Y���W
	float fZ,			//	�ˉe��Ԃł�Z�l�i0�`1�j
	int Screen_w,		//	�X�N���[���̕�
	int Screen_h,		//	�X�N���[���̍���
	XMMATRIX View,		//	�r���[�}�g���b�N�X
	XMMATRIX Prj		//	�ˉe�s��
);
// XY���ʂƃX�N���[�����W�̌�_�Z�o�֐�
XMVECTOR* CalcScreenToXY(
	XMFLOAT3* pout,		//	�i�[��
	float Sx,			//	�X�N���[��X���W
	float Sy,			//	�X�N���[��Y���W
	int Screen_w,		//	�X�N���[���̕�
	int Screen_h,		//	�X�N���[���̍���
	XMMATRIX View,		//	�r���[�}�g���b�N�X
	XMMATRIX Prj		//	�ˉe�s��
);


XMFLOAT2 CalcWorldSize();

// �����_���^�[�Q�b�g�r���[�̎擾
ID3D11RenderTargetView* GetRenderTargetView();
// Z�o�b�t�@
ID3D11DepthStencilView*	GetDepthStencilView();
// �X���b�v�`�F�[��
IDXGISwapChain*	GetSwapChain();
