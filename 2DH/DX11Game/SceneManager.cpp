//===== インクルード =====
#include "SceneManager.h"

//#include "fade.h"
//
#include "TitleScene.h"

//===== 静的メンバ変数 =====
SceneManager* SceneManager::m_pInstance = nullptr;

// インスタンス生成
//一回だけ読み込む為にインスタンス化して生成する
SceneManager* SceneManager::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager();
	}

	return m_pInstance;
}

// インスタンスの削除
void SceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// コンストラクタ
//初めはリストにシーンが追加されていないのでヌルポインタを指さないといけない
SceneManager::SceneManager()
	: m_CurrentScene(nullptr)
{
}

// デストラクタ
SceneManager::~SceneManager()
{
	//何の為に２回削除するのか　これがわからない　聞きましょう

	//	シーンリストを削除
	/*for (auto scene : m_SceneList)
		delete scene;*/

	//	リストを空にする
	m_SceneList.clear();
}

// 初期化
void SceneManager::Init()
{
	//	全シーンをリストに追加
	m_SceneList.push_back(new SceneTitle());			//	タイトルシーンの追加
	//m_SceneList.push_back(SceneGame::GetInstance());	//	ゲームシーンの追加
	//m_SceneList.push_back(new SceneSelect());			//	セレクトシーンの追加
	//m_SceneList.push_back(new SceneEnding());			//	エンディングの追加

	//	最初はタイトル画面へ
	SetScene(SCENE_TITLE);

	//	フェード初期化
	//InitFade();
}

// 更新
void SceneManager::Update()
{
	//	シーン更新
	//選ばれているシーンだけを更新
	m_CurrentScene->Update();

	//	フェードイン/アウト更新
	//UpdateFade();
}

// 描画
void SceneManager::Draw()
{
	//	シーン描画
	m_CurrentScene->Draw();
	//	フェードイン/アウト描画
	//DrawFade();

}

// 終了
void SceneManager::Uninit()
{
	// フェード終了処理
	//UninitFade();

	//	インスタンス破棄
	SceneManager::Destroy();
}

// シーンの入手
EScene SceneManager::GetScene() const
{
	return m_eSceneState;
}

// シーンの切り替え
// 引数は変えたいシーン先の情報
void SceneManager::SetScene(EScene NextScene)
{
	//	例外処理
	if (NextScene >= SCENE_MAX)
	{
		MessageBox(GetMainWnd(), _T("シーン切り替え失敗"), NULL, MB_OK);
		return;
	}

	//	前シーンの終了処理
	//選ばれているシーンの終了処理をかけてから
	if (m_CurrentScene)
		m_CurrentScene->Uninit();

	//	現在シーンの開始
	m_CurrentScene = m_SceneList[NextScene];	//	シーン切り替え
	m_eSceneState = NextScene;					//	シーン状態の設定
	m_CurrentScene->Init();						//	初期化
}