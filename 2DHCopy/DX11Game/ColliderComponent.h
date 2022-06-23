/**
* @fn		ColliderComponent.h
* @brief	���̃R���|�[�l���g�����I�u�W�F�N�g�ǂ������������Ă��邩���f����
* @author	�u���z�S
* @date		2022/6/22
*
* @detail	�K�v�Ȑݒ荀��
*			�����蔻��̑傫��
*
*/
//�C���N���[�h�K�[�h
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//�C���N���[�h
#include "mainApp.h"
#include "Component.h"

#include <vector>

//�O���錾
class ObjectManager;
class Object;

//�N���X��`
class CCollider :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W*/

	/** @brief �����蔻��̑傫��*/	
	XMFLOAT3 m_CollisionSize;
	/** @brief ���f�����S�̃Y��*/
	XMFLOAT2 m_CenterOffset;
	/** @brief ���a*/
	float m_fRadisu;
	/** @brief �����蔻��̃��X�g*/
	static std::list<Object*> m_ColliderList;
	/** @brief �Փ˂����I�u�W�F�N�g����*/
	std::list<Object*> m_IntersectObjects;

	//�����o�֐�
	//�����蔻��Ɩ����Ȓ��Ԃ���
	/** @brief ��`�ǂ����̓����蔻��*/
	bool CheckCollisionRectToRect(XMFLOAT2 centerPos1,XMFLOAT2 centerPos2,
		XMFLOAT2 size1,XMFLOAT2 size2,XMFLOAT2 Velocity1,XMFLOAT2 Velocity2);
	/** @brief �~�Ɖ~�̓����蔻��*/
	bool CheckCollisionCircleToCircle(XMFLOAT2, XMFLOAT2, float, float);
	/** @brief ���Ƌ��̓����蔻��*/
	bool CollisionSphere(XMFLOAT3 Apos,XMFLOAT3 Bpos, float Ar, float Br);
	/** @brief �����̂ƒ����̂̓����蔻��*/
	bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	/** @brief xy���ʂ̓����蔻��*/
	bool CollisionRectToRectXY();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CCollider();
	/** @brief �f�X�g���N�^*/
	~CCollider() override;
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V*/
	void Update() override;

	/** @brief �����蔻�胊�X�g�ɒǉ�*/
	static void AddColliderList(Object* pObject);
	/** @brief �����蔻��������Ă���I�u�W�F�N�g�̃��X�g���擾*/
	static std::list<Object*>& GetColliderList();
	/** @brief �����蔻��������Ă���I�u�W�F�N�g�����̍X�V*/
	static void UpdateColliders();

	//�Q�b�^�[
	/** @brief �I�u�W�F�N�g�̒��S�̃Y��*/
	XMFLOAT2 GetOffSet();
	/** @brief �����蔻��̑傫���̎擾*/
	XMFLOAT3 GetColliderSize();
	/** @brief �����蔻��̒��S�̍��W�̎擾*/
	XMFLOAT2 GetCenterPos();

	//�Z�b�^�[
	/** @brief �����蔻��̑傫���̐ݒ�*/
	void SetCollisionSize(float fx,float fy,float fz = 0.0f);
	/** @brief �����蔻��̒��S�ƃI�u�W�F�N�g�̒��S�Ƃ̃Y��*/
	void SetOffset(float fx,float fy);
};

#endif