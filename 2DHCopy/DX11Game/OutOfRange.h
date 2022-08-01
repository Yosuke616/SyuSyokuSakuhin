/**
* @file		OutOfRange.h
* @brief	カメラの範囲外なら描画をスキップする
* @author	志水陽祐
* @date		2022/7/31
*/

/**インクルードガード**/
#ifndef _OUTOFRANGE_H_
#define _OUTOFRANGE_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;

/**定数定義**/
//===== マクロ定義 =====
#define OUT_RANGE_X	(300.0f)
#define OUT_RANGE_Y	(220.0f)

/**クラス定義**/
class COutOfRange :public Component {
private:
	//メンバ変数
	/** @brief 座標とかの管理*/
	CTransform* m_pTransform;
	/** @brief 範囲外かどうかの判定*/
	XMFLOAT2 m_LimitRange;

	//メンバ関数
	/** @brief このコンポーネント以外の機能の停止*/
	void NoFunction();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	COutOfRange();
	/** @brief デストラクタ*/
	~COutOfRange()override;
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新*/
	void Update() override;
	/** @brief 描画*/
	void Draw();
	/** @brief 衝突判定の処理*/
	void OnCollisionEnter(Object* pObject)override;

	/** @brief 描画する範囲の設定*/
	void SetLimitRange(float x,float y);

};

#endif