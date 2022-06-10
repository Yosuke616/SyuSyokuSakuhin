//インクルード部
#include "SceneManager.h"

//シーンが増えたら追加する
#include "SceneTitle.h"

//追加されていったシーンの情報を追加していく

//静的メンバ変数
//初めは存在していない状態
SceneManager* SceneManager::m_pInstance = nullptr;

//インスタンス生成
SceneManager* SceneManager::Instance() {
	if (!m_pInstance) {
		//作られていない状態だったらここで作る
		m_pInstance = new SceneManager();
	}
	return m_pInstance;
}

//インスタンスの削除
void SceneManager::Destroy() {
	if (m_pInstance) {
		//インスタンス生成されていた場合削除する
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//コンストラクタ
SceneManager::SceneManager()
	:m_CurrentScene(nullptr) {

}

//デストラクタ
SceneManager::~SceneManager(){
	//シーンの中身を削除(リストを消していく)
	for (auto scene : m_pSceneList) {
		delete scene;
	}

	//リストを空にする
	m_pSceneList.clear();

}

//初期化
void SceneManager::Init() {
	//必要なシーンをリストに追加していく
	m_pSceneList.push_back(new SceneTitle());

	//初めのシーンを選択する
	SetScene(SCENE_TITLE);

}

//更新
void SceneManager::Update(){
	//シーン更新
	//選ばれているポインタに入っているシーンの更新
	m_CurrentScene->Update();
}

//描画
void SceneManager::Draw() {
	//　シーン描画
	m_CurrentScene->Draw();

}

//終了
void SceneManager::Uninit() {
	//インスタンスの破棄
	SceneManager::Destroy();
}

//シーンの入手
EScene SceneManager::GetScene() const {
	return m_eSceneState;
}

//シーンの切替
void SceneManager::SetScene(EScene NextScene) {
	//　例外処理
	if (NextScene >= MAX_SCENE) {
		//想定したシーンより大きい時にエラーを起こす
		MessageBox(GetMainWnd(), _T("シーン切り替え失敗"), NULL, MB_OK);
		return;
	}

	//現在のシーンを終了する
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//次のシーンの初期化をする
	m_CurrentScene = m_pSceneList[NextScene];		//現在のシーンの情報を更新する
	m_eSceneState = NextScene;						//送るための変数を更新する
	m_CurrentScene->Init();							//選ばれたシーンの初期化をする

}



