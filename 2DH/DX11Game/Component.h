/*
 Component.h
 全てのコンポーネントのベースクラス
 志水陽祐 真似っこ星人完璧
 2022/6/7
 */

 //===== インクルードガード ======
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

//===== インクルードガード ======
#include "mainApp.h"
#include "ObjectInfo.h"

#include "Object.h"

//===== 列挙体宣言 ======
enum COMPONENT_UPDATE_ORDER	//	コンポーネントの更新順序
{
	COM_UPDATE_1 = 0,		//	だいたいこれ指定
	COM_UPDATE_2,
	COM_UPDATE_TRANSRATE,	//	トランスフォームコンポーネント用
	COM_UPDATE_COLLIDER,	//	衝突処理
	COM_UPDATE_OFF_SCREEN,	//	画面外処理
	COM_UPDATE_DRAW,		//	描画コンポネント用
};

//===== クラス定義 =====
class Component
{
	//=== メンバ変数
private:
	bool m_bDelete = false;										//	削除フラグ
public:
	Object* Parent = nullptr;									//	コンポーネントの所有者
	COMPONENT_UPDATE_ORDER	m_eUpdateOrder = COM_UPDATE_1;		//	コンポーネントの更新順

	//=== メンバ関数
public:
	bool m_bDrawFlag = true;					//	描画フラグ
	bool m_bUpdateFlag = true;					//	更新フラグ
	void Delete() { m_bDelete = true; }			//	このオブジェクトを削除する
	bool GetDeleteFlag() { return m_bDelete; }	//	削除フラグ
	Component() {}								//	コンストラクタ
	virtual ~Component() {}						//	(仮想関数)デストラクタ
	virtual void Start() {}						//	(仮想関数)初期化
	virtual void Update() {}					//	(仮想関数)更新
	virtual void Draw() {}						//	(仮想関数)描画

/**
 * @fn		OnCollisionEnter
 * @brief	(仮想関数)オブジェクトとの衝突処理
 * @param	(Object*) このコンポーネントの所有者と衝突したオブジェクトのポインタ
 */
	virtual void OnCollisionEnter(Object*) {}
};
#endif // __COMPONENT_H__
