/*
	�I�u�W�F�N�g�̏����܂Ƃ߂邽�߂̃w�b�_�[
	�u���z�S
	2022/6/7
*/

//===== �C���N���[�h�K�[�h =====
#ifndef __OBJECT_INFO_H__
#define __OBJECT_INFO_H__

//===== �񋓑� ======

//	�I�u�W�F�N�g�̍X�V�̏���
enum UPDATE_ORDER
{
	UPDATE_BG,			//	�w�i
	UPDATE_PLAYER,		//	�v���C���[
	UPDATE_FIELD,		//	�n�`
	UPDATE_EFFECT,		//	�G�t�F�N�g
	UPDATE_MODEL,		//	���f��
	UPDATE_DEBUG,		//	debug
	UPDATE_UI,			//	UI
	UPDATE_OPTION,		//	�I�v�V�����I�u�W�F�N�g
	UPDATE_GOLETTER,

	MAX_UPDATE_ORDER
};

//	�I�u�W�F�N�g�̕`��̏���
enum DRAW_ORDER
{
	DRAW_BG,		//	�w�i
	DRAW_FIELD,		//	�n�`
	DRAW_MODEL,		//	�v���C���[
	DRAW_PLAYER,	//	�v���C���[
	DRAW_EFFECT,	//	�G�t�F�N�g

	DRAW_DEBUG,		//	debug
	DRAW_UI_FRAME,	//	UI�g
	DRAW_GOLETTER,
	DRAW_UI,		//	UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//	�I�v�V�����I�u�W�F�N�g
	DRAW_OPTION,	//	�I�v�V�����I�u�W�F�N�g

	MAX_DRAW_ORDER
};

	/***** �A�j���[�V���� ******/

// �v���C���[��FBX�̐�
enum PLAYER_FBX
{
	//FBX_ONE = 0,	// ���(�ҋ@�A���ɐ�����Ă���A�����Ă���A�Q�b�g������
	//FBX_TWO,		// ���(�S�[��������
};

// 1�ڂ�fbx
enum ANIM_ID
{
	/*PLAYER_IDLE = 0,
	PLAYER_CARRIED,
	PLAYER_FALL,
	PLAYER_GET,
	PLAYER_BIND,
	PLAYER_GAMEOVER,

	MAX_ANIMATION*/
};
#endif