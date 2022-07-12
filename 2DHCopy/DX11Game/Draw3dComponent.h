/**
* @file		Draw3dCimponent.h
* @brief	3D�̃I�u�W�F�N�g����邽�߂̃R���|�[�l���g
* @author	�u���z�S
* @date		2022/7/7
*/

///�C���N���[�h�K�[�h
#ifndef _DRAW_3D_H_
#define _DRAW_3D_H_

//�C���N���[�h��
#include "Component.h"
#include "AssimpModel.h"
#include "mainApp.h"

//�O���錾
class CTransform;
class Animation;

//�N���X��`
class CDraw3D :public Component {
private:
	//�����o�ϐ�
	/** @brief ���f��*/
	CAssimpModel* m_model;
	/** @brief ���[���h�}�g���b�N�X*/
	XMFLOAT4X4 m_mtxWorld;
	/** @brief �I�u�W�F�N�g�̍��W*/
	CTransform* m_pTransform;
	/** @brief �}�e���A�����*/
	TAssimpMaterial* m_pMaterial;
	/** @brief �A�j���[�V�����̏��*/
	Animation* m_pAnim;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CDraw3D();
	/** @brief �f�X�g���N�^*/
	~CDraw3D();
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V*/
	void Update() override;
	/** @brief �`��*/
	void Draw() override;

	/** @brief ���[���h�s��̎擾*/
	XMFLOAT4X4* GetWorldPos();
	/** @brief ���f���̑傫��*/
	XMFLOAT3 GetBox();
	/** @brief ���f�����̎擾*/
	CAssimpModel* GetModel();

	/** @brief ���f���̐ݒ�*/
	void SetModel(CAssimpModel* model);
	/** @brief �v���~�e�B�u�̐ݒ�*/
	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);
	/** @brief ���f���̃e�N�X�`���ݒ�*/
	void SetTexture(ID3D11ShaderResourceView* pTexture);

	/** @brief ��ł���*/
	//double GetAnimDuration();

	//�A�j���[�V����
	/** @brief �A�j���[�V�����̒ǉ�*/
	void AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame = -1);
	/** @brief �A�j���[�V�����̍Đ�*/
	void Play(int AnimID);
	/** @brief �A�j���[�V�����̍Đ��t���[���ݒ�*/
	void SetAnimTime(double dTime);
	/** @brief ��ɃA�j���[�V�����𗬂���*/
	void AlwaysAnimation(bool always);
	/** @brief �Đ����x*/
	void SetPlaySpeed(int AnimID,double speed);
	/** @brief ��ΓI�ȓz*/
	void Absolute(int AnimID,bool Absolute);
	/** @brief �A�j���[�V�����I��*/
	bool AnimEnd(int AnimID);
};

#endif