/*
	�S�ẴR���|�[�l���g�̌��̃N���X
	�u���z�S
	2022/6/9�@�쐬
*/

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//�C���N���[�h��
#include "mainApp.h"
#include "ObjInfo.h"

#include "Object.h"

//�񋓑��錾
enum COMPONENT_UPDATE_ORDER {		//�R���|�[�l���g�̍X�V��
	COM_UPDATE_1 = 0,				//����ő��v����
	COM_UPDATE_2,					
	COM_UPDATE_TRANSRATE,			//�g�����X�t�H�[���R���|�[�l���g
	COM_UPDATE_COLLIDER,			//�Փˏ���
	COM_UPDATE_OFF_SCREEN,			//��ʊO����
	COM_UPDATE_DRAW,				//�`��p�R���|�[�l���g

	MAX_COMPONENT
};

//�N���X��`
class Component {
private:

protected:

public:

};

#endif