/**
* @file		Draw3dCimponent.h
* @brief	3Dのオブジェクトを作るためのコンポーネント
* @author	志水陽祐
* @date		2022/7/7
*/

///インクルードガード
#ifndef _DRAW_3D_H_
#define _DRAW_3D_H_

//インクルード部
#include "Component.h"
#include "AssimpModel.h"
#include "mainApp.h"

//前方宣言
class CTransform;
class Animation;

//クラス定義
class CDraw3D :public Component {
private:
	//メンバ変数
	/** @brief モデル*/
	CAssimpModel* m_model;
	/** @brief ワールドマトリックス*/
	XMFLOAT4X4 m_mtxWorld;
	/** @brief オブジェクトの座標*/
	CTransform* m_pTransform;
	/** @brief マテリアル情報*/
	TAssimpMaterial* m_pMaterial;
	/** @brief アニメーションの情報*/
	Animation* m_pAnim;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CDraw3D();
	/** @brief デストラクタ*/
	~CDraw3D();
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新*/
	void Update() override;
	/** @brief 描画*/
	void Draw() override;

	/** @brief ワールド行列の取得*/
	XMFLOAT4X4* GetWorldPos();
	/** @brief モデルの大きさ*/
	XMFLOAT3 GetBox();
	/** @brief モデル情報の取得*/
	CAssimpModel* GetModel();

	/** @brief モデルの設定*/
	void SetModel(CAssimpModel* model);
	/** @brief プリミティブの設定*/
	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type);
	/** @brief モデルのテクスチャ設定*/
	void SetTexture(ID3D11ShaderResourceView* pTexture);

	/** @brief 謎でした*/
	//double GetAnimDuration();

	//アニメーション
	/** @brief アニメーションの追加*/
	void AddAnimation(int AnimID, double Start, double end, bool loop, double loopStartFrame = -1);
	/** @brief アニメーションの再生*/
	void Play(int AnimID);
	/** @brief アニメーションの再生フレーム設定*/
	void SetAnimTime(double dTime);
	/** @brief 常にアニメーションを流すか*/
	void AlwaysAnimation(bool always);
	/** @brief 再生速度*/
	void SetPlaySpeed(int AnimID,double speed);
	/** @brief 絶対的な奴*/
	void Absolute(int AnimID,bool Absolute);
	/** @brief アニメーション終了*/
	bool AnimEnd(int AnimID);
};

#endif