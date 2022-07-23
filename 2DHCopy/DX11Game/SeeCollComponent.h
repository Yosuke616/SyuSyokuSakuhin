/**
* @file		SeeCollComponent.h
* @brief	当たり判定を見えるようにする
* @detail   親の座標と当たり判定の大きさを取得し可視化する 
* @author	志水陽祐
* @date		2022/7/21
*/

//インクルードガード
#ifndef _SEECOLL_H_
#define _SEECOLL_H_

//インクルード部
#include "mainApp.h"
#include "Component.h"

//前方宣言
class CTransform;
class CCollider;
class ObjectManager;
class Object;

//クラス定義
class CSeeColl :public Component {
private:
	/** @brief 座標*/
	CTransform* m_pTransform;
	/** @brief 当たり判定の情報*/
	CCollider* m_pColl;
	/** @brief object自体の情報を保存する*/
	Object* m_pObject;
protected:

public:
	/** @brief コンストラクタ*/
	CSeeColl();
	/** @brief デストラクタ*/
	~CSeeColl();
	/** @brief 初期化*/
	void Start() override;
	/** brief 更新処理*/
	void Update() override;
	/** brief 描画処理*/
	void Draw() override;
};

#endif