/*
	作成者　志水陽祐　2022/6/3
	タイトルシーンを作る
*/

 //===== インクルードガード ======
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

//===== インクルード =====
#include "Scene.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ------------------------------
//#include "sceneGame.h"
#include <vector>
//---------------------------------------------------------------------



//===== クラス定義 =====
class SceneTitle : public Scene
{
public:
	SceneTitle();						//	コンストラクタ
	~SceneTitle() override;				//	デストラクタ
	void Init() override;				//	初期化
	void Uninit() override;				//	終了処理
	void Update() override;				//	更新
	void Draw() override;				//	描画
};
#endif // __SCENE_TITLE_H__


