/**
* @file		TITLEComponent.h 
* @brief	敵の動きを抑制するコンポーネント
* @author	志水陽祐
* @date		2023/1/13
*/

/**インクルードガード**/
#ifndef _TITLE_COMPONENT_H_
#define _TITLE_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CDraw2D;
class CAnimMesh;

/**クラス定義**/
class CTITLE :public Component {
private:
	//メンバ変数
	/** @brief 座標、拡縮、回転*/
	CTransform* m_pTransform;
	/** @brief 描画情報 UI*/
	CDraw2D* m_pDraw2D;
	/** @brief 描画情報 メッシュ*/
	CAnimMesh* m_pDrawMesh;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CTITLE();
	/** @brief デストラクタ*/
	~CTITLE()override;
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw()override;

};

#endif