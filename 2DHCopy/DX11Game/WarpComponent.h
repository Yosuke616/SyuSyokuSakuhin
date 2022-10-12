/**
* @file		WarpComponent.h
* @brief	ワープするためのコンポネント
* @detail	各ステージごとにどうやってワープするか決める
* @author	志水陽祐
* @date		2022/10/11
*/

/**インクルードガード**/
#ifndef _WARP_COMPONENT_H_
#define _WARP_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**クラス定義**/
/**
* @brief	ワープを制御する為のコンポネント
* @detail	ワープに触れた状態で特定の行動をするとワープする
*/
class CWarp : public Component {
private:
	//メンバ変数
	/** @brief 座標とかの設定*/
	CTransform* m_pTransform;
	/** @brief モデルとかの設定*/
	CDraw3D* m_pDraw;
	/** @brief 当たり判定の情報*/
	CCollider* m_pCollider;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	/** @brief コンストラクタ*/
	CWarp();
	/** @brief デストラクタ*/
	~CWarp();
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