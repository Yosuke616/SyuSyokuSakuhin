/**
* @file		TITLEComponent.h 
* @brief	�G�̓�����}������R���|�[�l���g
* @author	�u���z�S
* @date		2023/1/13
*/

/**�C���N���[�h�K�[�h**/
#ifndef _TITLE_COMPONENT_H_
#define _TITLE_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CDraw2D;
class CAnimMesh;

/**�N���X��`**/
class CTITLE :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�A�g�k�A��]*/
	CTransform* m_pTransform;
	/** @brief �`���� UI*/
	CDraw2D* m_pDraw2D;
	/** @brief �`���� ���b�V��*/
	CAnimMesh* m_pDrawMesh;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CTITLE();
	/** @brief �f�X�g���N�^*/
	~CTITLE()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw()override;

};

#endif