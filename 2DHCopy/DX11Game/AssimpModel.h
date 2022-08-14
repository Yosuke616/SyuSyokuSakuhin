// Assimpライブラリ用モデルクラス定義 [AssimpModel.h]
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


// マクロ
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{if(p){(p)->Release();(p)=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{if(p){delete p;(p)=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{if(p){delete[]p;(p)=nullptr;}}
#endif

// 定数
enum EByOpacity {
	eNoAffect = 0,		// 全て
	eOpacityOnly,		// 不透明のみ
	eTransparentOnly,	// 透明のみ
};

// マテリアル
struct TAssimpMaterial {
	DirectX::XMFLOAT4	Ka;		// アンビエント+テクスチャ有無
	DirectX::XMFLOAT4	Kd;		// ディフューズ
	DirectX::XMFLOAT4	Ks;		// スペキュラ+スペキュラ強度
	DirectX::XMFLOAT4	Ke;		// エミッシブ
	ID3D11ShaderResourceView*	pTexture;		// 拡散テクスチャ
	ID3D11ShaderResourceView*	pTexSpecular;	// 鏡面反射テクスチャ
	ID3D11ShaderResourceView*	pTexEmmisive;	// 発光テクスチャ
	ID3D11ShaderResourceView*	pTexTransparent;// 透過テクスチャ
	TAssimpMaterial()
	{
		Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// アンビエント+テクスチャ有無
		Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// ディフューズ
		Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// スペキュラ+スペキュラ強度
		Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// エミッシブ
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

// 頂点情報
struct TAssimpVertex {
	DirectX::XMFLOAT3	vPos;	// 頂点位置
	DirectX::XMFLOAT3	vNorm;	// 頂点法線
	DirectX::XMFLOAT2	vTex;	// UV座標
	UINT		uBoneIndex[4];	// ボーン番号
	float		fBoneWeight[4];	// ボーン重み
	TAssimpVertex()
	{
		vPos = DirectX::XMFLOAT3(0, 0, 0);
		vNorm = DirectX::XMFLOAT3(0, 0, -1);
		vTex = DirectX::XMFLOAT2(0, 0);
		uBoneIndex[0] = uBoneIndex[1] = uBoneIndex[2] = uBoneIndex[3] = 0;
		fBoneWeight[0] = fBoneWeight[1] = fBoneWeight[2] = fBoneWeight[3] = 0.0f;
	}
};

// アニメ評価クラス
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

// シーン ノード情報(追加データ有のツリー構造)
// (一部のコンパイラではpublicでなければならない)
struct SceneAnimNode {
	std::string mName;
	SceneAnimNode* mParent;
	std::vector<SceneAnimNode*> mChildren;

	// 直近のローカル変換マトリックス
	aiMatrix4x4 mLocalTransform;

	// 直近のワールド変換マトリックス
	aiMatrix4x4 mGlobalTransform;

	// 現在のチャネル配列インデックス。アニメーション無しの場合は-1。
	int mChannelIndex;

	// デフォルト コンストラクタ
	SceneAnimNode() : mName()
		, mParent(nullptr)
		, mChildren()
		, mLocalTransform()
		, mGlobalTransform()
		, mChannelIndex(-1)
	{
		// (空)
	}

	// 名前を伴うコンストラクタ
	SceneAnimNode(const std::string& pName) : mName(pName)
		, mParent(nullptr)
		, mChildren()
		, mLocalTransform()
		, mGlobalTransform()
		, mChannelIndex(-1)
	{
		// (空)
	}

	// デストラクタ
	~SceneAnimNode()
	{
		for (std::vector<SceneAnimNode*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it) {
			delete *it;
		}
	}
};

// アニメーション ノード マトリックス生成クラス
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

// メッシュ クラス
class CAssimpModel;
class CAssimpMesh
{
private:
	CAssimpModel* m_pModel;					//	モデルクラス
	std::vector<TAssimpVertex> m_aVertex;	//	頂点情報
	std::vector<UINT> m_aIndex;				//	頂点のインデックス
	TAssimpMaterial m_material;				//	マテリアル情報
	ID3D11Buffer* m_pVertexBuffer;			//	頂点バッファ
	ID3D11Buffer* m_pIndexBuffer;			//	インデックスバッファ
	ID3D11Buffer* m_pConstantBuffer0;		//	???
	ID3D11Buffer* m_pConstantBuffer1;		//	???

	ID3D11Buffer* m_pConstantBufferBone;	//	???

public:

	CAssimpMesh(ID3D11Device* pDevice,		//	コンストラクタ
		CAssimpModel* pModel,
		std::vector<TAssimpVertex> aVertex,
		std::vector<UINT> aIndex,
		TAssimpMaterial& material);

	virtual ~CAssimpMesh();					//	デストラクタ

	void Draw(ID3D11DeviceContext* pDC,		//	描画
		DirectX::XMFLOAT4X4& mtxWorld,
		EByOpacity byOpacity = eNoAffect);

	void Release();							//	開放

	void SetBoneMatrix(ID3D11DeviceContext* pDC, XMFLOAT4X4 mtxBone[]);	// ボーン マトリックス設定
private:
	bool SetupMesh(ID3D11Device* pDevice);	// 頂点バッファ/インデックス バッファ生成
};

// モデル クラス
class CAssimpModel
{
private:
	std::string m_directory;			//	ファイル読み込みのパス
	std::string m_fname;				//	ファイル読み込みのパス
	std::string m_ext;					//	ファイル読み込みのパス
	std::vector<CAssimpMesh> m_aMesh;	//	3Dメッシュリスト
	std::string m_textype;
	DirectX::XMFLOAT4X4 m_mtxTexture;	//	テクスチャ マトリックス
	DirectX::XMFLOAT4X4 m_mtxWorld;		//	ワールドマ トリックス
	TAssimpMaterial* m_pMaterial;		//	テクスチャのマテリアル情報
	DirectX::XMFLOAT3 m_vBBox;			//	境界線ボックス
	DirectX::XMFLOAT3 m_vCenter;		//	中心座標
	TAssimpMaterial* m_pSaveMate;			//　マテリアルを保持しておくための変数

	const aiScene* m_pScene;			//	アニメーションシーン
	SceneAnimator* m_pAnimator;			//	アニメーション
	double m_dCurrent;
	double m_dLastPlaying;

	static ID3D11InputLayout* m_pVertexLayout;
	static ID3D11VertexShader* m_pVertexShader;
	static ID3D11PixelShader* m_pPixelShader;
	static ID3D11SamplerState* m_pSampleLinear;

	//***追加
	D3D_PRIMITIVE_TOPOLOGY m_PrimitiveType;	//	プリミティブ

	bool m_bAnimation;
public:
	void SetAnimation(bool bAnimation);
	static double g_time;
	CAssimpModel();												//	コンストラクタ
	virtual ~CAssimpModel();									//	デストラクタ
	static bool InitShader(ID3D11Device* pDevice);				//	シェーダー初期化
	static void UninitShader();									//	シェーダー終了

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);			//	プリミティブの設定（追加）
	DirectX::XMFLOAT4X4& GetTextureMatrix();					//	テクスチャマトリックスの取得
	void SetTextureMatrix(DirectX::XMFLOAT4X4& mtxTexture);		//	テクスチャマトリックスの設定
	//	モデル読み込み
	bool Load(ID3D11Device* pDevice, ID3D11DeviceContext* pDC, std::string filename);
	//	描画
	void Draw(ID3D11DeviceContext* pDC, DirectX::XMFLOAT4X4& mtxWorld, EByOpacity byOpacity = eNoAffect);
	//	アニメーション用の描画
	void DrawNode(ID3D11DeviceContext* pDC, aiNode* piNode, const aiMatrix4x4& piMatrix, EByOpacity byOpacity);
	//マテリアルの保存
	void SetMate(TAssimpMaterial* mate);
	//敵マテリアルの取得
	TAssimpMaterial* GetMate();

	//	ワールドマトリックスの取得
	DirectX::XMFLOAT4X4& GetWorldMatrix()
	{
		return m_mtxWorld;
	}
	//	マテリアルの設定
	void SetMaterial(TAssimpMaterial* pMaterial)
	{
		m_pMaterial = pMaterial;
	}
	//	マテリアルの取得
	TAssimpMaterial* GetMaterial()
	{
		return m_pMaterial;
	}
	//	アニメーションの有無
	SceneAnimator* HasAnimation()
	{
		return m_pAnimator;
	}

	void SetAnimIndex(int nAnimIndex);				//	アニメーションインデックスの取得
	UINT GetAnimCount();							//	アニメーション セット数取得
	double GetAnimDuration(int nAnimIndex = -1);	//???	/* -1:現在のアニメ */
	void SetAnimTime(double dTime);					//	アニメーション再生フレームの設定
	void Release();									//	メッシュ、マテリアルの開放

	//	モデルを包む最小限の立方体のサイズ
	DirectX::XMFLOAT3& GetBBox()
	{
		return m_vBBox;
	}
	//	モデルの中心座標の取得
	DirectX::XMFLOAT3& GetCenter()
	{
		return m_vCenter;
	}

	//名前を取得する
	constexpr const std::string& GetName() const {
		return m_fname;
	}

private:
	void ScaleAsset();										//	境界ボックス計算
	void CalculateBounds(aiNode* piNode,					//	ノード毎に頂点座標の最大最小値をチェック
		aiVector3D* p_avOut,
		const aiMatrix4x4& piMatrix);
	void processNode(ID3D11Device* pDevice, aiNode* node);	//	全メッシュ取り込み
	CAssimpMesh processMesh(ID3D11Device* pDevice,			//	メッシュ展開
		aiMesh* mesh);
	TAssimpMaterial loadMaterial(ID3D11Device* pDevice,		//	マテリアル読込
		aiMaterial* mat,
		aiMesh* mesh);
	std::string determineTextureType(aiMaterial* mat);		//	テクスチャ タイプ決定
	int getTextureIndex(aiString* str);						//	テクスチャ インデックス取得
	ID3D11ShaderResourceView* getTextureFromModel(			//	モデル データ内包テクスチャ読込
		ID3D11Device* pDevice, int textureindex);
	void LoadTexture(										//	テクスチャ読み込み
		ID3D11Device* pDevice,
		aiString& szPath,
		ID3D11ShaderResourceView** ppTexture);
	bool HasAlphaPixels(ID3D11ShaderResourceView* pTexture);//	 テクスチャのアルファ有無
};
