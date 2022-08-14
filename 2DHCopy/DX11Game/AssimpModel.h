// Assimp���C�u�����p���f���N���X��` [AssimpModel.h]
#pragma once
#define NOMINMAX
//#define D3DCOMPILER
#include <map>
#include <vector>
#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#ifdef D3DCOMPILER
#include <d3dcompiler.h>
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Camera.h"
#include "Light.h"


// �}�N��
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{if(p){(p)->Release();(p)=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{if(p){delete p;(p)=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{if(p){delete[]p;(p)=nullptr;}}
#endif

// �萔
enum EByOpacity {
	eNoAffect = 0,		// �S��
	eOpacityOnly,		// �s�����̂�
	eTransparentOnly,	// �����̂�
};

// �}�e���A��
struct TAssimpMaterial {
	DirectX::XMFLOAT4	Ka;		// �A���r�G���g+�e�N�X�`���L��
	DirectX::XMFLOAT4	Kd;		// �f�B�t���[�Y
	DirectX::XMFLOAT4	Ks;		// �X�y�L����+�X�y�L�������x
	DirectX::XMFLOAT4	Ke;		// �G�~�b�V�u
	ID3D11ShaderResourceView*	pTexture;		// �g�U�e�N�X�`��
	ID3D11ShaderResourceView*	pTexSpecular;	// ���ʔ��˃e�N�X�`��
	ID3D11ShaderResourceView*	pTexEmmisive;	// �����e�N�X�`��
	ID3D11ShaderResourceView*	pTexTransparent;// ���߃e�N�X�`��
	TAssimpMaterial()
	{
		Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// �A���r�G���g+�e�N�X�`���L��
		Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// �f�B�t���[�Y
		Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// �X�y�L����+�X�y�L�������x
		Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// �G�~�b�V�u
		pTexture = nullptr;
		pTexEmmisive = nullptr;
		pTexSpecular = nullptr;
		pTexTransparent = nullptr;
	}
	void Release()
	{
		SAFE_RELEASE(pTexTransparent);
		SAFE_RELEASE(pTexSpecular);
		SAFE_RELEASE(pTexEmmisive);
		SAFE_RELEASE(pTexture);
	}
};

// ���_���
struct TAssimpVertex {
	DirectX::XMFLOAT3	vPos;	// ���_�ʒu
	DirectX::XMFLOAT3	vNorm;	// ���_�@��
	DirectX::XMFLOAT2	vTex;	// UV���W
	UINT		uBoneIndex[4];	// �{�[���ԍ�
	float		fBoneWeight[4];	// �{�[���d��
	TAssimpVertex()
	{
		vPos = DirectX::XMFLOAT3(0, 0, 0);
		vNorm = DirectX::XMFLOAT3(0, 0, -1);
		vTex = DirectX::XMFLOAT2(0, 0);
		uBoneIndex[0] = uBoneIndex[1] = uBoneIndex[2] = uBoneIndex[3] = 0;
		fBoneWeight[0] = fBoneWeight[1] = fBoneWeight[2] = fBoneWeight[3] = 0.0f;
	}
};

// �A�j���]���N���X
class AnimEvaluator {
public:
	AnimEvaluator(const aiAnimation* pAnim);
	~AnimEvaluator();

	void Evaluate(double pTime);
	const std::vector<aiMatrix4x4>& GetTransformations() const { return mTransforms; }

protected:
	const aiAnimation* mAnim;
	double mLastTime;
	std::vector<std::tuple<unsigned int, unsigned int, unsigned int> > mLastPositions;
	std::vector<aiMatrix4x4> mTransforms;
};

// �V�[�� �m�[�h���(�ǉ��f�[�^�L�̃c���[�\��)
// (�ꕔ�̃R���p�C���ł�public�łȂ���΂Ȃ�Ȃ�)
struct SceneAnimNode {
	std::string mName;
	SceneAnimNode* mParent;
	std::vector<SceneAnimNode*> mChildren;

	// ���߂̃��[�J���ϊ��}�g���b�N�X
	aiMatrix4x4 mLocalTransform;

	// ���߂̃��[���h�ϊ��}�g���b�N�X
	aiMatrix4x4 mGlobalTransform;

	// ���݂̃`���l���z��C���f�b�N�X�B�A�j���[�V���������̏ꍇ��-1�B
	int mChannelIndex;

	// �f�t�H���g �R���X�g���N�^
	SceneAnimNode() : mName()
		, mParent(nullptr)
		, mChildren()
		, mLocalTransform()
		, mGlobalTransform()
		, mChannelIndex(-1)
	{
		// (��)
	}

	// ���O�𔺂��R���X�g���N�^
	SceneAnimNode(const std::string& pName) : mName(pName)
		, mParent(nullptr)
		, mChildren()
		, mLocalTransform()
		, mGlobalTransform()
		, mChannelIndex(-1)
	{
		// (��)
	}

	// �f�X�g���N�^
	~SceneAnimNode()
	{
		for (std::vector<SceneAnimNode*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
			delete *it;
		}
	}
};

// �A�j���[�V���� �m�[�h �}�g���b�N�X�����N���X
class SceneAnimator {
public:
	SceneAnimator(const aiScene* pScene, size_t pAnimIndex = 0);
	~SceneAnimator();

	void SetAnimIndex(size_t pAnimIndex);
	void Calculate(double pTime);
	const aiMatrix4x4& GetLocalTransform(const aiNode* node) const;
	const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;
	const std::vector<aiMatrix4x4>& GetBoneMatrices(const aiNode* pNode, size_t pMeshIndex = 0);

	size_t CurrentAnimIndex() const { return mCurrentAnimIndex; }

	aiAnimation* CurrentAnim() const {
		return static_cast<unsigned int>(mCurrentAnimIndex) < mScene->mNumAnimations ? mScene->mAnimations[mCurrentAnimIndex] : nullptr;
	}

protected:
	SceneAnimNode* CreateNodeTree(aiNode* pNode, SceneAnimNode* pParent);
	void UpdateTransforms(SceneAnimNode* pNode, const std::vector<aiMatrix4x4>& pTransforms);
	void CalculateGlobalTransform(SceneAnimNode* pInternalNode);

protected:
	const aiScene* mScene;
	int mCurrentAnimIndex;
	AnimEvaluator* mAnimEvaluator;
	SceneAnimNode* mRootNode;
	typedef std::map<const aiNode*, SceneAnimNode*> NodeMap;
	NodeMap mNodesByName;
	typedef std::map<const char*, const aiNode*> BoneMap;
	BoneMap mBoneNodesByName;
	std::vector<aiMatrix4x4> mTransforms;
};

// ���b�V�� �N���X
class CAssimpModel;
class CAssimpMesh
{
private:
	CAssimpModel* m_pModel;					//	���f���N���X
	std::vector<TAssimpVertex> m_aVertex;	//	���_���
	std::vector<UINT> m_aIndex;				//	���_�̃C���f�b�N�X
	TAssimpMaterial m_material;				//	�}�e���A�����
	ID3D11Buffer* m_pVertexBuffer;			//	���_�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer;			//	�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pConstantBuffer0;		//	???
	ID3D11Buffer* m_pConstantBuffer1;		//	???

	ID3D11Buffer* m_pConstantBufferBone;	//	???

public:

	CAssimpMesh(ID3D11Device* pDevice,		//	�R���X�g���N�^
		CAssimpModel* pModel,
		std::vector<TAssimpVertex> aVertex,
		std::vector<UINT> aIndex,
		TAssimpMaterial& material);

	virtual ~CAssimpMesh();					//	�f�X�g���N�^

	void Draw(ID3D11DeviceContext* pDC,		//	�`��
		DirectX::XMFLOAT4X4& mtxWorld,
		EByOpacity byOpacity = eNoAffect);

	void Release();							//	�J��

	void SetBoneMatrix(ID3D11DeviceContext* pDC, XMFLOAT4X4 mtxBone[]);	// �{�[�� �}�g���b�N�X�ݒ�
private:
	bool SetupMesh(ID3D11Device* pDevice);	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
};

// ���f�� �N���X
class CAssimpModel
{
private:
	std::string m_directory;			//	�t�@�C���ǂݍ��݂̃p�X
	std::string m_fname;				//	�t�@�C���ǂݍ��݂̃p�X
	std::string m_ext;					//	�t�@�C���ǂݍ��݂̃p�X
	std::vector<CAssimpMesh> m_aMesh;	//	3D���b�V�����X�g
	std::string m_textype;
	DirectX::XMFLOAT4X4 m_mtxTexture;	//	�e�N�X�`�� �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxWorld;		//	���[���h�} �g���b�N�X
	TAssimpMaterial* m_pMaterial;		//	�e�N�X�`���̃}�e���A�����
	DirectX::XMFLOAT3 m_vBBox;			//	���E���{�b�N�X
	DirectX::XMFLOAT3 m_vCenter;		//	���S���W
	TAssimpMaterial* m_pSaveMate;			//�@�}�e���A����ێ����Ă������߂̕ϐ�

	const aiScene* m_pScene;			//	�A�j���[�V�����V�[��
	SceneAnimator* m_pAnimator;			//	�A�j���[�V����
	double m_dCurrent;
	double m_dLastPlaying;

	static ID3D11InputLayout* m_pVertexLayout;
	static ID3D11VertexShader* m_pVertexShader;
	static ID3D11PixelShader* m_pPixelShader;
	static ID3D11SamplerState* m_pSampleLinear;

	//***�ǉ�
	D3D_PRIMITIVE_TOPOLOGY m_PrimitiveType;	//	�v���~�e�B�u

	bool m_bAnimation;
public:
	void SetAnimation(bool bAnimation);
	static double g_time;
	CAssimpModel();												//	�R���X�g���N�^
	virtual ~CAssimpModel();									//	�f�X�g���N�^
	static bool InitShader(ID3D11Device* pDevice);				//	�V�F�[�_�[������
	static void UninitShader();									//	�V�F�[�_�[�I��

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);			//	�v���~�e�B�u�̐ݒ�i�ǉ��j
	DirectX::XMFLOAT4X4& GetTextureMatrix();					//	�e�N�X�`���}�g���b�N�X�̎擾
	void SetTextureMatrix(DirectX::XMFLOAT4X4& mtxTexture);		//	�e�N�X�`���}�g���b�N�X�̐ݒ�
	//	���f���ǂݍ���
	bool Load(ID3D11Device* pDevice, ID3D11DeviceContext* pDC, std::string filename);
	//	�`��
	void Draw(ID3D11DeviceContext* pDC, DirectX::XMFLOAT4X4& mtxWorld, EByOpacity byOpacity = eNoAffect);
	//	�A�j���[�V�����p�̕`��
	void DrawNode(ID3D11DeviceContext* pDC, aiNode* piNode, const aiMatrix4x4& piMatrix, EByOpacity byOpacity);
	//�}�e���A���̕ۑ�
	void SetMate(TAssimpMaterial* mate);
	//�G�}�e���A���̎擾
	TAssimpMaterial* GetMate();

	//	���[���h�}�g���b�N�X�̎擾
	DirectX::XMFLOAT4X4& GetWorldMatrix()
	{
		return m_mtxWorld;
	}
	//	�}�e���A���̐ݒ�
	void SetMaterial(TAssimpMaterial* pMaterial)
	{
		m_pMaterial = pMaterial;
	}
	//	�}�e���A���̎擾
	TAssimpMaterial* GetMaterial()
	{
		return m_pMaterial;
	}
	//	�A�j���[�V�����̗L��
	SceneAnimator* HasAnimation()
	{
		return m_pAnimator;
	}

	void SetAnimIndex(int nAnimIndex);				//	�A�j���[�V�����C���f�b�N�X�̎擾
	UINT GetAnimCount();							//	�A�j���[�V���� �Z�b�g���擾
	double GetAnimDuration(int nAnimIndex = -1);	//???	/* -1:���݂̃A�j�� */
	void SetAnimTime(double dTime);					//	�A�j���[�V�����Đ��t���[���̐ݒ�
	void Release();									//	���b�V���A�}�e���A���̊J��

	//	���f�����ލŏ����̗����̂̃T�C�Y
	DirectX::XMFLOAT3& GetBBox()
	{
		return m_vBBox;
	}
	//	���f���̒��S���W�̎擾
	DirectX::XMFLOAT3& GetCenter()
	{
		return m_vCenter;
	}

	//���O���擾����
	constexpr const std::string& GetName() const {
		return m_fname;
	}

private:
	void ScaleAsset();										//	���E�{�b�N�X�v�Z
	void CalculateBounds(aiNode* piNode,					//	�m�[�h���ɒ��_���W�̍ő�ŏ��l���`�F�b�N
		aiVector3D* p_avOut,
		const aiMatrix4x4& piMatrix);
	void processNode(ID3D11Device* pDevice, aiNode* node);	//	�S���b�V����荞��
	CAssimpMesh processMesh(ID3D11Device* pDevice,			//	���b�V���W�J
		aiMesh* mesh);
	TAssimpMaterial loadMaterial(ID3D11Device* pDevice,		//	�}�e���A���Ǎ�
		aiMaterial* mat,
		aiMesh* mesh);
	std::string determineTextureType(aiMaterial* mat);		//	�e�N�X�`�� �^�C�v����
	int getTextureIndex(aiString* str);						//	�e�N�X�`�� �C���f�b�N�X�擾
	ID3D11ShaderResourceView* getTextureFromModel(			//	���f�� �f�[�^����e�N�X�`���Ǎ�
		ID3D11Device* pDevice, int textureindex);
	void LoadTexture(										//	�e�N�X�`���ǂݍ���
		ID3D11Device* pDevice,
		aiString& szPath,
		ID3D11ShaderResourceView** ppTexture);
	bool HasAlphaPixels(ID3D11ShaderResourceView* pTexture);//	 �e�N�X�`���̃A���t�@�L��
};
