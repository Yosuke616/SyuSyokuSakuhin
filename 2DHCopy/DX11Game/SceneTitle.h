/*
	志水陽祐
	2022/6/8
	タイトルシーンの作成

*/

//インクルードガード
#ifndef _SCENE_TILTE_H_
#define _SCENE_TITLE_H_

//インクルード部
#include "Scene.h"

//クラス定義
class SceneTitle :public Scene {
private:
	//メンバ変数

	//メンバ関数
protected:
	//メンバ変数

	//メンバ関数
public:
	//メンバ変数

	//メンバ関数
	SceneTitle();						//	コンストラクタ
	~SceneTitle() override;				//	デストラクタ
	void Init() override;				//	初期化
	void Uninit() override;				//	終了処理
	void Update() override;				//	更新
	void Draw() override;				//	描画
};








#endif