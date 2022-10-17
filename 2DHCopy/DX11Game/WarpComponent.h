/**
* @file		WarpComponent.h
* @brief	���[�v���邽�߂̃R���|�l���g
* @detail	�e�X�e�[�W���Ƃɂǂ�����ă��[�v���邩���߂�
* @author	�u���z�S
* @date		2022/10/11
*/

/**�C���N���[�h�K�[�h**/
#ifndef _WARP_COMPONENT_H_
#define _WARP_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**�N���X��`**/
/**
* @brief	���[�v�𐧌䂷��ׂ̃R���|�l���g
* @detail	���[�v�ɐG�ꂽ��Ԃœ���̍s��������ƃ��[�v����
*/
class CWarp : public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ��̐ݒ�*/
	CTransform* m_pTransform;
	/** @brief �����蔻��̏��*/
	CCollider* m_pCollider;
	/** @brief ���[�v���邩�ǂ����̃t���O*/
	bool m_bWarpFlg;

	//�����o�֐�
	/** @brief ���̊֐���ŏ���*/
	void CreateBlak();
	/** @brief �J�����̒����_��؂��ăv���C���[�ƈꏏ�ɍ��W�𓮂���*/
	void ChangeCamera();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	/** @brief �R���X�g���N�^*/
	CWarp();
	/** @brief �f�X�g���N�^*/
	~CWarp();
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
	/** @brief �����蔻��̏���*/
	void OnCollisionEnter(Object* pObject)override;

	//�����o�֐�
	void SetWarp(bool bWarp);
};

#endif