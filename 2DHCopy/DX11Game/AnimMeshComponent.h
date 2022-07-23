/**
* @file		AnimMeshComponent.h
* @brief	�e�N�X�`���A�j���[�V������`�悷��R���|�[�l���g
* @author	�u���z�S
* @date		2022/7/22
*/

/**�C���N���[�h�K�[�h**/
#ifndef _ANIM_MESH_H_
#define _ANIM_MESH_H_

/**�C���N���[�h��**/
#include "DrawMeshComponent.h"

/**�O���錾**/

/**�萔��`**/

/**�N���X��`**/
class CAnimMesh :public CDrawMesh {
private:
	/*�����o�ϐ�*/
	/** @brief �A�j���[�V�����̐ؑ֊Ԋu*/
	int m_nFrame;
	/** @brief �A�j���[�V�����J�E���^*/
	int m_nAnimCount;
	/** @brief �A�j���[�V�������[�v�t���O*/
	bool m_bLoop;

	/*�����o�֐�*/
	/** @brief ���_���̍쐬*/
	void CreateAnimVertex();

protected:
	/*�����o�ϐ�*/

	/*�����o�֐�*/

public:
	/*�����o�ϐ�*/

	/*�����o�֐�*/
	/** @brief �R���X�g���N�^*/
	CAnimMesh();
	/** @brief �f�X�g���N�^*/
	~CAnimMesh()override;
	/** @brief ����������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw()override;

	/**�Z�b�^�[**/
	/** @brief �A�j���[�V�����J�E���^�̐ݒ�*/
	void SetSwapFrame(int nCount);
	/** @brief �A�j���[�V���������[�v���邩�ǂ����̐ݒ�*/
	void SetLoop(bool loop);

	/** @brief �A�j���[�V�����̐ݒ�*/
	static void StartAnim(ID3D11ShaderResourceView* pTex,
		XMFLOAT3 pos, XMFLOAT2 size, int split_x, int split_y,
		bool loop,
		int swap_frame = 1,
		XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f)
		);

};
	
#endif