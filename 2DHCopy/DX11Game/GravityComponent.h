/**
* @file		GravityComponent.h
* @brief	重力を加えるコンポーネント
* @author	志水陽祐
* @date		2022/7/24
*/

/**インクルードガード**/
#ifndef _GRAVITY_H_
#define _GRAVITY_H_

/**インクルード**/
#include "mainApp.h"
#include "Component.h"

/**定数定義**/
/** @brief 重力の大きさ*/
#define GRAVITY (0.3f)
#define MAX_VELOCITY (7.5f)

/**前方宣言**/
class CTransform;

/**クラス定義**/
class CGravity :public Component {
private:
	//メンバ変数
	/** @brief 座標*/
	CTransform* m_pTransform;
	/** @brief ジャンプするときにこのコンポーネントを切る為の変数*/
	bool m_bUse;
	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CGravity();
	/** @brief デストラクタ*/
	~CGravity();
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新*/
	void Update()override;

	/**セッター**/
	/** @brief 更新を止めるか止めないか*/
	void SetUse(bool use);
};

#endif