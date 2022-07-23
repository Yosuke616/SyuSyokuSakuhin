/**
* @file		DrawMeshComponent.h
* @breif	���b�V���`��p�R���|�[�l���g
* @author	�u���z�S
* @date		2022/7/21
*/

//�C���N���[�h�K�[�h
#ifndef _DRAW_MESH_H_
#define _DRAW_MESH_H_

//�C���N���[�h�K�[�h
#include "Component.h"
#include "mainApp.h"
#include "mesh.h"

//�O���錾
class CTransform;

//�N���X��`
class CDrawMesh :public Component {
private:
	//�����o�ϐ�
	/** @brief �e�N�X�`���̉��̃T�C�Y*/
	float m_fTexSizeX;
	/** @brief �e�N�X�`���̏c�̃T�C�Y*/
	float m_fTexSizeZ;

	//�����o�֐�
	/** @brief ���_���쐬*/
	HRESULT MakeVertex(
		float fSizeBlockX,float fSizeBlockZ,
		float fTexSizeX,float fTexSizeZ,
		XMFLOAT3 normal);

protected:
	//�����o�ϐ�
	/** @brief �I�t�Z�b�g*/
	XMFLOAT2 m_Offset;
	/** @brief 1�}�X�̉���*/
	float m_fWidth;
	/** @brief 1�}�X�̗���*/
	float m_fHeight;
	/** @brief �@���x�N�g��*/
	XMFLOAT3 m_fNormal;
	/** @brief ���b�V��*/
	MESH m_Mesh;
	/** @brief �}�e���A��*/
	MATERIAL m_Material;
	/** @brief ���W*/
	CTransform* m_pTransform;
	/** @brief �A�j���[�V����*/
	int m_nAnim;
	/** @brief ������X*/
	int m_nSplitX;
	/** @brief ������Y*/
	int m_nSplitY;


	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CDrawMesh();
	/** @brief �f�X�g���N�^*/
	~CDrawMesh() override;
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V*/
	void Update() override;
	/** @brief �`��*/
	void Draw() override;

	/**�Z�b�^�[**/
	/** @brief �e�N�X�`���̃Z�b�g*/
	void SetTexture(ID3D11ShaderResourceView* pTex);
	/** @brief �e�N�X�`�����W*/
	void SetUVsize(XMFLOAT2 uv);
	/** @brief �}�X�̑傫��*/
	void SetSize(float width,float height);
	/** @brief ���ˌ�*/
	void SetDiffuse(XMFLOAT4 Diffuse);
	/** @brief ����*/
	void SetAmbient(XMFLOAT4 Ambient);
	/** @brief ���ʔ��˂̐ݒ�*/
	void SetSpecular(XMFLOAT4 Specular); 
	/** @brief ���Ȕ����̐ݒ�*/
	void SetEmissive(XMFLOAT4 Emissive);
	/** @brief ����̐ݒ�*/
	void SetPower(float power);
	/** @brief �@���x�N�g���̐ݒ�*/
	void SetNormal(float fx,float fy,float fz);
	/** @brief �A�j���[�V�����̔ԍ�*/
	void SetAnimFrame(int Anim);
	/** @brief �������ݒ�*/
	void SetAnimSplit(int nx,int ny);
	/** @brief �I�t�Z�b�g*/
	void SetOffSet(XMFLOAT2 offset);
	/** @brief �F*/
	void SetColor(float r,float g,float b);

	/**�Q�b�^�[**/
	/** @brief ���b�V�����̎擾*/
	MESH* GetMesh();
};

#endif