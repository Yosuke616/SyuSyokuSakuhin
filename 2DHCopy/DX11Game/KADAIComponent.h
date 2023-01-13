/**
* @file		KADAIComponent.h
* @brief	GRを終わらせるためだけのコンポーネント
* @author	志水陽祐
* @date		2023/1/3
*/

/**インクルードガード**/
#ifndef _KADAI_COMPONENT_H_
#define _KADAI_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class  CTransform;
class  Object;
class  CCollider;
class  CAnimMesh;

/**クラス定義**/
class CKADAI : public Component {
private:
	//メンバ変数
	/** @brief 座標とかの情報*/
	CTransform* m_pTrans;
	/** @brief 描画情報*/
	CAnimMesh* m_pDraw;
	/** @brief 当たり判定情報*/
	CCollider* m_pColl;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	
	//メンバ関数
	/** @brief コンストラクタ*/
	CKADAI();
	/** @brief デストラクタ*/
	~CKADAI();
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;
	/** @brief 衝突判定*/
	void OnCollisionEnter(Object* pObject) override;

};

#endif