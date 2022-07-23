/**
* @file		AnimMeshComponent.h
* @brief	テクスチャアニメーションを描画するコンポーネント
* @author	志水陽祐
* @date		2022/7/22
*/

/**インクルードガード**/
#ifndef _ANIM_MESH_H_
#define _ANIM_MESH_H_

/**インクルード部**/
#include "DrawMeshComponent.h"

/**前方宣言**/

/**定数定義**/

/**クラス定義**/
class CAnimMesh :public CDrawMesh {
private:
	/*メンバ変数*/
	/** @brief アニメーションの切替間隔*/
	int m_nFrame;
	/** @brief アニメーションカウンタ*/
	int m_nAnimCount;
	/** @brief アニメーションループフラグ*/
	bool m_bLoop;

	/*メンバ関数*/
	/** @brief 頂点情報の作成*/
	void CreateAnimVertex();

protected:
	/*メンバ変数*/

	/*メンバ関数*/

public:
	/*メンバ変数*/

	/*メンバ関数*/
	/** @brief コンストラクタ*/
	CAnimMesh();
	/** @brief デストラクタ*/
	~CAnimMesh()override;
	/** @brief 初期化処理*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw()override;

	/**セッター**/
	/** @brief アニメーションカウンタの設定*/
	void SetSwapFrame(int nCount);
	/** @brief アニメーションがループするかどうかの設定*/
	void SetLoop(bool loop);

	/** @brief アニメーションの設定*/
	static void StartAnim(ID3D11ShaderResourceView* pTex,
		XMFLOAT3 pos, XMFLOAT2 size, int split_x, int split_y,
		bool loop,
		int swap_frame = 1,
		XMFLOAT3 color = XMFLOAT3(1.0f, 1.0f, 1.0f)
		);

};
	
#endif