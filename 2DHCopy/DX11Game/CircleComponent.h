/**
* @file		CircleComponent.h
* @brief	バリアを貼るためのコンポーネント
* @author	志水陽祐
* @date		2022/10/10
*/

/**インクルードガード**/
#ifndef _CIRCLE_COMPONENT_H_
#define _CIRCLE_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CAnimMesh;

/**クラス定義**/
/**
* @brief	サークルを制御するためのコンポーネント
*/
class CCircle : public Component{
private:
	//メンバ変数
	/** @brief 座標とかを制御するポインタ*/
	CTransform* m_pTransform;
	/** @brief 描画情報を制御するポインタ*/
	CAnimMesh* m_pDraw;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	/** @brief コンストラクタ*/
	CCircle();
	/** @brief デストラクタ*/
	~CCircle();
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;
	/** @brief 当たり判定の処理*/
	void OnCollisionEnter(Object* pObject)override;

	//メンバ関数

};

#endif