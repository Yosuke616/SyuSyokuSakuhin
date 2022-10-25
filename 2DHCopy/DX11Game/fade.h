/**
* @file		fade.h
* @brief	�t�F�[�h�C��/�A�E�g
* @author	�u���z�S
* @date		2022/10/22
*/

/**�C���N���[�h�K�[�h**/
#ifndef _FADE_H_
#define _FADE_H_

/**�C���N���[�h**/
#include "SceneManager.h"
#include "StageManager.h"

/**�񋓑̐錾**/
enum EFade {
	FADE_NONE = 0,			//�������Ă��Ȃ�
	FADE_IN,				//�t�F�[�h�C��������
	FADE_OUT,				//�t�F�[�h�A�E�g������
	FADE_OUT_SCENE_CHANGE,	//�t�F�[�h�A�E�g������(�V�[���J��)
	MAX_FADE
};

/**�v���g�^�C�v�錾**/
/** @brief ������*/
void InitFade();				
/** @brief �I������*/
void UninitFade();				
/** @brief �X�V����*/
void UpdateFade();					
/** @brief �`�揈��*/
void DrawFade();				
/** @brief �V�[���J�ڂ��܂߂��t�F�[�h�A�E�g*/
void StartFadeOutSceneChange(EScene eNext, int deley = 0);
/** @breif �t�F�[�h������������*/
void StartFadeOut(int deley = 0);

/** @brief ��Ԏ擾*/
EFade GetFade();					
/** @brief �t�F�[�h�A�E�g�F�ݒ�*/
void SetFadeColor(float fR, float fG, float fB); 
#endif