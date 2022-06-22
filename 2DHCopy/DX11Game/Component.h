/*
	全てのコンポーネントの元のクラス
	志水陽祐
	2022/6/9　作成
*/

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//インクルード部
#include "mainApp.h"
#include "ObjInfo.h"

#include "Object.h"

//列挙隊宣言
enum COMPONENT_UPDATE_ORDER {		//コンポーネントの更新順
	COM_UPDATE_1 = 0,				//これで大丈夫だよ
	COM_UPDATE_2,					
	COM_UPDATE_TRANSRATE,			//トランスフォームコンポーネント
	COM_UPDATE_COLLIDER,			//衝突処理
	COM_UPDATE_OFF_SCREEN,			//画面外処理
	COM_UPDATE_DRAW,				//描画用コンポーネント

	MAX_COMPONENT
};

//クラス定義
class Component {
private:
	//メンバ変数
	bool m_bDelete = false;			//削除するかのフラグ

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	Object* Parent = nullptr;									//オブジェクトの所有者
	COMPONENT_UPDATE_ORDER m_eUpdateOrder = COM_UPDATE_1;		//オブジェクトの更新順序

	//メンバ関数
	bool m_bDrawFlag = true;									//描画のフラグ
	bool m_bUpdateFlag = true;									//更新のフラグ
	void Delete() { m_bDelete = true; }							//このコンポーネント？を削除する
	bool GetDeleteFlag() { return m_bDelete; }					//削除フラグ
	Component() {}												//コンストラクタ
	virtual ~Component() {}										//(仮想関数)デストラクタ
	virtual void Start() {}										//(仮想関数)初期化
	virtual void Update() {}									//(仮想関数)更新
	virtual void Draw() {}										//(仮想関数)描画


	virtual void OnCollisionEnter(Object*) {}					//どのオブジェクトが衝突したかを判定するかの関数
																//このコンポーネントの所有者が衝突したオブジェクトのポインタを返す
};

#endif