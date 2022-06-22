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
	//�����o�ϐ�
	bool m_bDelete = false;			//�폜���邩�̃t���O

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	Object* Parent = nullptr;									//�I�u�W�F�N�g�̏��L��
	COMPONENT_UPDATE_ORDER m_eUpdateOrder = COM_UPDATE_1;		//�I�u�W�F�N�g�̍X�V����

	//�����o�֐�
	bool m_bDrawFlag = true;									//�`��̃t���O
	bool m_bUpdateFlag = true;									//�X�V�̃t���O
	void Delete() { m_bDelete = true; }							//���̃R���|�[�l���g�H���폜����
	bool GetDeleteFlag() { return m_bDelete; }					//�폜�t���O
	Component() {}												//�R���X�g���N�^
	virtual ~Component() {}										//(���z�֐�)�f�X�g���N�^
	virtual void Start() {}										//(���z�֐�)������
	virtual void Update() {}									//(���z�֐�)�X�V
	virtual void Draw() {}										//(���z�֐�)�`��


	virtual void OnCollisionEnter(Object*) {}					//�ǂ̃I�u�W�F�N�g���Փ˂������𔻒肷�邩�̊֐�
																//���̃R���|�[�l���g�̏��L�҂��Փ˂����I�u�W�F�N�g�̃|�C���^��Ԃ�
};

#endif