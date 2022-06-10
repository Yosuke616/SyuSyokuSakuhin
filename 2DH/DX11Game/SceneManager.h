#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

/*

	シーンを管理する
	作成者 : YOSUKE SHIMIZU

	更新
	志水　作成開始　2022/6/3
	

	シーン追加について
	ヘッダーのESceneにシーン名を登録し、Init関数内でシーンリストに追加する処理を加える

*/

//===== インクルード =====
#include "mainApp.h"
#include "Scene.h"

#include <vector>
#include <list>

//===== 列挙体定義 =====
enum EScene
{
	SCENE_TITLE,		//	タイトル画面
	SCENE_GAME,			//	ゲーム画面
	SCENE_SELECT,		//	セレクト画面
	SCENE_ENDING,

	SCENE_MAX
};


//===== クラス定義 =====
class SceneManager
{
	//=== メンバ変数
private:
	static SceneManager* m_pInstance;	//	インスタンス生成用
	vector<Scene*>	 m_SceneList;	//	シーンリスト
	Scene* m_CurrentScene;				//	現在のシーン
	EScene m_eSceneState;				//	シーンの状態

	//=== メンバ関数
protected:
	SceneManager();						//	コンストラクタ
public:
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
#endif //__SCENE_MANAGER_H__