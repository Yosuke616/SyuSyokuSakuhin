/*
	志水陽祐
	2022/6/8　制作開始
	どのシーンを更新して描画するかを管理するクラス

*/

//インクルードガード
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//インクルード部
#include "mainApp.h"
#include "Scene.h"

#include <vector>

//列挙隊宣言(シーン増加に伴い中身は増やしていく)
enum EScene {
	SCENE_TITLE = 0,

	MAX_SCENE
};

//クラス定義
class SceneManager {
private:
	//メンバ変数
	static SceneManager* m_pInstance;		//インスタンス生成用
	std::vector<Scene*> m_pSceneList;		//シーンリスト(タイトルとかゲーム画面とか)
	Scene* m_CurrentScene;					//現在のシーン
	EScene m_eSceneState;					//どのシーンが選ばれているかとかを送ったりセットしたりするため


	//メンバ関数

protected:
	SceneManager();						//	コンストラクタ

	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	static SceneManager* Instance();	//	インスタンス化
	static void Destroy();				//	インスタンスの削除

	~SceneManager();					//	デストラクタ
	void Init();						//	初期化
	void Update();						//	更新
	void Draw();						//	描画
	void Uninit();						//	終了

	void SetScene(EScene NextScene);	//	シーン切り替え
	EScene GetScene() const;			//	シーン情報の取得

};


#endif