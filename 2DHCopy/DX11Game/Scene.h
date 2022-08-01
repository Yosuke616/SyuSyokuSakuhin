#ifndef _SCENE_H_
#define _SCENE_H_

/*
	作成者:YOSUKE SHIMIZU 
	全てのシーンの元になる親クラス
	終わりの始まり

	更新
	志水　作成開始　2022/6/3
*/

//===== インクルード ======
#include "ObjectManager.h"
#include "MenuManager.h"

//===== クラス定義 ======
class Scene
{
protected:
	//=== メンバ変数 ===
	/** @brief オブジェクトの管理ポインタ*/
	ObjectManager* m_pObjectManager;
	/** @brief メニュー管理用のポインタ*/
	MenuManager* m_pMenuManager;

public:
	//=== メンバ関数 ===
	Scene() {}							//	コンストラクタ
	virtual ~Scene() {}					//	(仮想関数)デストラクタ
	virtual void Init() = 0;			//	(純粋仮想関数)初期化
	virtual void Uninit() = 0;			//	(純粋仮想関数)終了
	virtual void Update() = 0;			//	(純粋仮想関数)更新
	virtual void Draw() = 0;			//	(純粋仮想関数)描画
};

#endif