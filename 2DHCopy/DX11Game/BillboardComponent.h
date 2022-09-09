/**
* @file		BillboardComponent.h
* @brief	ビルボードさせるコンポーネント
* @author	志水陽祐
* @date		2022/9/8
*/

/**インクルード部**/
#ifndef _BILLBOARD_COMPONENT_H_
#define _BILLBOARD_COMPONENT_H_

/**インクルード部**/
#include "Component.h"
#include "DrawMeshComponent.h"
#include "Object.h"

/**前方宣言**/
class CTransform;

/**クラス定義**/
/**
* @brief	ビルボードさせる
* @detail	ゲーム上のUIや背景を表示させる
*/
class CBillboard : public CDrawMesh {
private:
	//メンバ変数

	//メンバ関数
	/** @brief ビルボードさせるためのメッシュを作る*/
	void CreateBillboardVertex();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CBillboard();										
	/** @brief デストラクタ*/
	~CBillboard();											
	/** @brief 初期化*/
	void Start() override;									
	/** @brief 更新処理*/
	void Update() override;									
	/** @brief 描画処理*/
	void Draw() override;									
	/** @brief 衝突処理*/
	void OnCollisionEnter(Object* pObject) override;		
};

#endif