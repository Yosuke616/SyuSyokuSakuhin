/**
* @file		BillboardComponent.h
* @brief	�r���{�[�h������R���|�[�l���g
* @author	�u���z�S
* @date		2022/9/8
*/

/**�C���N���[�h��**/
#ifndef _BILLBOARD_COMPONENT_H_
#define _BILLBOARD_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"
#include "DrawMeshComponent.h"
#include "Object.h"

/**�O���錾**/
class CTransform;

/**�N���X��`**/
/**
* @brief	�r���{�[�h������
* @detail	�Q�[�����UI��w�i��\��������
*/
class CBillboard : public CDrawMesh {
private:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �r���{�[�h�����邽�߂̃��b�V�������*/
	void CreateBillboardVertex();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CBillboard();										
	/** @brief �f�X�g���N�^*/
	~CBillboard();											
	/** @brief ������*/
	void Start() override;									
	/** @brief �X�V����*/
	void Update() override;									
	/** @brief �`�揈��*/
	void Draw() override;									
	/** @brief �Փˏ���*/
	void OnCollisionEnter(Object* pObject) override;		
};

#endif