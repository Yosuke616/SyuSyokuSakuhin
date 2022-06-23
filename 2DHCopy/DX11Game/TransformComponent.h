/**
* @fn		TransformComponent.h
* @brief	���W�̂��ꂱ��̂��߂̃R���|�[�l���g
* @author	�u���z�S
* @date		2022/6/23
*/

//�C���N���[�h�K�[�h
#ifndef _TRANSFORM_COMPONENT_H_
#define _TRANSFORM_COMPONENT_H_

//�C���N���[�h��
#include "mainApp.h"
#include "Component.h"

//�N���X��`
class CTransform :public Component {
private:
	//�����o�ϐ�

	//�����o�֐�
protected:
	//�����o�ϐ�

	//�����o�֐�
public:
	//�����o�ϐ�
	/** @brief ���W*/
	XMFLOAT3 Pos;
	/** @brief ���x*/
	XMFLOAT3 Vel;
	/** @brief �����x*/
	XMFLOAT3 Acc;
	/** @brief �傫��*/
	XMFLOAT3 Scale;
	/** @brief ��]*/
	XMFLOAT3 Rotate;

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CTransform();
	/** @brief �f�X�g���N�^*/
	~CTransform();
	/** @brief �X�V����*/
	void Update() override;
	/** @brief ���W�̐ݒ�*/
	void SetPosition(float fx,float fy,float fz = 0.0f);
	/** @brief ���x�̐ݒ�*/
	void SetVelocity(float fx,float fy,float fz = 0.0f);
	/** @brief �傫���̐ݒ�*/
	void SetScale(float fx,float fy,float fz = 0.0f);
	/** @brief �X��(��])�̐ݒ�*/
	void SetRotate(float fx ,float fy,float fz = 0.0f);
};

#endif