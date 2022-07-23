/**
* @file		DrawMeshComponent.h
* @breif	メッシュ描画用コンポーネント
* @author	志水陽祐
* @date		2022/7/21
*/

//インクルードガード
#ifndef _DRAW_MESH_H_
#define _DRAW_MESH_H_

//インクルードガード
#include "Component.h"
#include "mainApp.h"
#include "mesh.h"

//前方宣言
class CTransform;

//クラス定義
class CDrawMesh :public Component {
private:
	//メンバ変数
	/** @brief テクスチャの横のサイズ*/
	float m_fTexSizeX;
	/** @brief テクスチャの縦のサイズ*/
	float m_fTexSizeZ;

	//メンバ関数
	/** @brief 頂点情報作成*/
	HRESULT MakeVertex(
		float fSizeBlockX,float fSizeBlockZ,
		float fTexSizeX,float fTexSizeZ,
		XMFLOAT3 normal);

protected:
	//メンバ変数
	/** @brief オフセット*/
	XMFLOAT2 m_Offset;
	/** @brief 1マスの横幅*/
	float m_fWidth;
	/** @brief 1マスの立幅*/
	float m_fHeight;
	/** @brief 法線ベクトル*/
	XMFLOAT3 m_fNormal;
	/** @brief メッシュ*/
	MESH m_Mesh;
	/** @brief マテリアル*/
	MATERIAL m_Material;
	/** @brief 座標*/
	CTransform* m_pTransform;
	/** @brief アニメーション*/
	int m_nAnim;
	/** @brief 分割数X*/
	int m_nSplitX;
	/** @brief 分割数Y*/
	int m_nSplitY;


	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CDrawMesh();
	/** @brief デストラクタ*/
	~CDrawMesh() override;
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新*/
	void Update() override;
	/** @brief 描画*/
	void Draw() override;

	/**セッター**/
	/** @brief テクスチャのセット*/
	void SetTexture(ID3D11ShaderResourceView* pTex);
	/** @brief テクスチャ座標*/
	void SetUVsize(XMFLOAT2 uv);
	/** @brief マスの大きさ*/
	void SetSize(float width,float height);
	/** @brief 反射光*/
	void SetDiffuse(XMFLOAT4 Diffuse);
	/** @brief 環境光*/
	void SetAmbient(XMFLOAT4 Ambient);
	/** @brief 鏡面反射の設定*/
	void SetSpecular(XMFLOAT4 Specular); 
	/** @brief 自己発光の設定*/
	void SetEmissive(XMFLOAT4 Emissive);
	/** @brief 光沢の設定*/
	void SetPower(float power);
	/** @brief 法線ベクトルの設定*/
	void SetNormal(float fx,float fy,float fz);
	/** @brief アニメーションの番号*/
	void SetAnimFrame(int Anim);
	/** @brief 分割数設定*/
	void SetAnimSplit(int nx,int ny);
	/** @brief オフセット*/
	void SetOffSet(XMFLOAT2 offset);
	/** @brief 色*/
	void SetColor(float r,float g,float b);

	/**ゲッター**/
	/** @brief メッシュ情報の取得*/
	MESH* GetMesh();
};

#endif