#ifndef _OBJECT_INFO_H_
#define _OBJECT_INFO_H_

//�񋓑��錾

//�I�u�W�F�N�g�̍X�V����
enum UPDATE_ORDER {
	UPDATE_BG,			//�w�i
	UPDATE_PLAYER,		//�v���C���[
	UPDATE_FIELD,		//�n�`
	UPDATE_EFFECT,		//�G�t�F�N�g
	UPDATE_MODEL,		//���f��
	UPDATE_DEBUG,		//debug
	UPDATE_UI,			//UI
	UPDATE_OPTION,		//�I�v�V����
	UPDATE_GOLETTER,	//

	MAX_UPDATE_ORDER
};

//�I�u�W�F�N�g�̕`��̏���
enum DRAW_ORDER {
	DRAW_BG,			//�w�i
	DRAW_FIELD,			//�t�B�[���h
	DRAW_MODEL,			//���f��
	DRAW_PLAYER,		//�v���C���[
	DRAW_EFFECT,		//�G�t�F�N�g

	DRAW_DEBUG,			//debug
	DRAW_UI_FRAME,		//UI�g
	DRAW_GOLETTER,
	DRAW_UI,			//UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//�I�v�V�����I�u�W�F�N�g
	DRAW_OPTION,		//�I�v�V����

	MAX_DRAW_ORDER
};

//@@@@@ �f�B�G�b�N�X����� ----------------------------------------
#define PLAYER_NAME					("DXchan")
#define DXCHAN_SIZE_X				(100.0f)
#define DXCHAN_SIZE_Y				(100.0f)
#define DXCHAN_COLL_SIZE_X			(25.0f)
#define DXCHAN_COLL_SIZE_Y			(80.0f)
#define DXCHAN_COLL_SIZE_Z			(100.0f)

//@@@@@ �u���b�N�̊�{��� ----------------------------------------
#define BLOCK_NAME					("Block")
#define MAPCHIP_WIDTH				(40.0f)	
#define MAPCHIP_HEIGHT				(40.0f)	
#define BLOCK_SIZE_X				(10.0f)
#define BLOCK_SIZE_Y				(10.0f)
#define BLOCK_SIZE_Z				(10.0f)
#define BLOCK_COLL_SIZE_X			(38.5f)
#define BLOCK_COLL_SIZE_Y			(38.5f)
#define BLOCK_COLL_SIZE_Z			(45.0f)

//@@@@@ �G�̊�{��� ----------------------------------------------
//�G���Ƃɐݒ肷��l�͕ς���Ă���
#define ENEMY_NAME					("Enemy")
//�����G�@�����N���{�[
#define E_WALK_SIZE_X				(10.0f)
#define E_WALK_SIZE_Y				(10.0f)
#define E_WALK_SIZE_Z				(10.0f)
#define E_WALK_COLL_X				(43.5f)
#define E_WALK_COLL_Y				(43.5f)
#define E_WALK_COLL_Z				(50.0f)

#endif