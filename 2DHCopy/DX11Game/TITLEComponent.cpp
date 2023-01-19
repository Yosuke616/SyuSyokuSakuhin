/**インクルード部**/
#include "TITLEComponent.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "SceneTitle.h"
#include "imgui.h"

#include "ObjInfo.h"

/**
* @fn		CTITLE::CTITLE
* @brief	コンストラクタ。。。お前船降りろ
*/
CTITLE::CTITLE() 
	:m_pTransform(nullptr),m_pDraw2D(nullptr),m_pDrawMesh(nullptr),
	 m_StartObj(nullptr),m_EndObj(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CTITLE::~CTITLE
* @brief	デストラクタ、シンジに乗れ
*/
CTITLE::~CTITLE() {
	
}

/**
* @fn		CTITLE::START
* @brief	開始処理
*/
void CTITLE::Start() {
	//流れ星を落とすフラグ
	m_bFallStar = false;
	//スタート地点と終了地点
	m_StartPos = XMFLOAT2(0.0f,0.0f);
	m_EndPos = XMFLOAT2(0.0f,0.0f);

	if (Parent->GetName() == "TITLE_Start_Pos") {
		//開始地点と終了地点のオブジェクトの座標を取得する
		m_StartObj = Parent;

		for (auto&& Obj : ObjectManager::GetInstance()->GetUpdateList()) {
			if (Obj->GetName() == "TITLE_End_Pos") {
				m_EndObj = Obj;
			}
		}
	}

	//流れ星を落とすタイミングはランダムにする
	m_nStarCnt = rand()% 60 * 10;

	//一度だけ生成させるフラグ
	m_bStarCre = false;

	m_pTransform = Parent->GetComponent<CTransform>();
	//プレイヤーキャラクターのみこっちの処理
	if (Parent->GetName() == "Miko") {
		m_pDrawMesh = Parent->GetComponent<CAnimMesh>();
		//右に移動させる
		m_pTransform->Vel.x = 2.0f;
	}
	else if (Parent->GetName() == "TITLE_Start_Pos") {
		m_pTransform->Vel.x = rand() % 5;
	}
	else if (Parent->GetName() == "TITLE_End_Pos") {
		m_pTransform->Vel.x = rand() % 5;
	}else {
		m_pDraw2D = Parent->GetComponent<CDraw2D>();
	}

}

/**
* @fn		CTITLE::UPDATE
* @brief	更新処理
* @detail	巫女ちゃんは処理換えるよ
*/
void CTITLE::Update(){
	//処理換えるよ～ん
	if (Parent->GetName() == "Miko") {

		//画面端!に言ったら逆向きに走らせる
		if (m_pTransform->Pos.x >= 450.0f) {
			//テクスチャを反転させるて左向きに力を加える
			m_pDrawMesh->SetVertex(false);
			m_pTransform->Vel.x = -2.0f;
		}
		if (m_pTransform->Pos.x <= -450.0f) {
			//テクスチャを反転させて右向きに力を加える
			m_pDrawMesh->SetVertex(true);
			m_pTransform->Vel.x = 2.0f;
		}

	}	
	else if (Parent->GetName() == TITLE_PURPLE_NAME)  {
		//色をたまに紫に変更する
		if (rand()%10 == 5 || rand() % 10 == 4 || rand() % 10 == 3) {
			m_pDraw2D->SetColor(1.0f,0.0f,1.0f);
		}
		else {
			m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
		}
	}
	else if (Parent->GetName() == "TITLE_Start_Pos" ) {
		//左右に移動させて画面端で反転(200～-200)
		if (m_pTransform->Pos.x >= 200.0f) {
			m_pTransform->Vel.x = -(rand() % 5);
		}
		if (m_pTransform->Pos.x <= -200.0f) {
			m_pTransform->Vel.x = (rand() % 5);
		}
	}
	else if (Parent->GetName() == "TITLE_End_Pos") {
		//左右に移動させて画面端で反転(200～-200)
		if (m_pTransform->Pos.x >= 200.0f) {
			m_pTransform->Vel.x = -(rand() % 5);
		}
		if (m_pTransform->Pos.x <= -200.0f) {
			m_pTransform->Vel.x = (rand() % 5);
		}
	}

	//一度だけ処理させるため開始地点と紐付けておく
	if (Parent->GetName() == "TITLE_Start_Pos") {


		//適当な時間でスタート地点と終了時点のポジションを決める
		if (!m_bFallStar) {
			//タイマーを減らし続ける
			m_nStarCnt--;

			//タイマーが0を下回ったら
			if (m_nStarCnt <= 0) {
				m_bFallStar = true;
				//スタート地点と終了地点のポジションを設定する
				m_StartPos.x = m_StartObj->GetComponent<CTransform>()->Pos.x;
				m_StartPos.y = m_StartObj->GetComponent<CTransform>()->Pos.y;
				m_EndPos.x = m_EndObj->GetComponent<CTransform>()->Pos.x;
				m_EndPos.y = m_EndObj->GetComponent<CTransform>()->Pos.y;
			}
		}
		else {
			if (!m_bStarCre) {
				//オブジェクトを作る
				Object* Star = new Object("Star", UPDATE_DEBUG, DRAW_DEBUG);
				//コンポーネントの追加
				auto TransStar = Star->AddComponent<CTransform>();
				auto DrawStar = Star->AddComponent<CAnimMesh>();
				//オブジェクトの設定
				DrawStar->SetColor(0.0f, 1.0f, 1.0f);
				TransStar->SetPosition(m_StartPos.x, m_StartPos.y);
				DrawStar->SetSize(10.0f, 10.0f);
				DrawStar->SetLoop(true);
				//オブジェクトを追加
				ObjectManager::GetInstance()->AddObject(Star);
				//フラグをオフにして二度と作らない
				m_bStarCre = true;
				//このオブジェクトをポインタで指しておく
				m_StarObj = Star;
			}
			else {
				//作ったオブジェクトを動かす
				//流れ星みたいに落として消えたら情報なども全て消す
				//エルミート曲線
				m_StarObj->GetComponent<CTransform>()->SetPosition(rand() % 100, rand() % 100);
			}

		}
	}
}

/**
* @fn
* @brief
*/
void CTITLE::Draw() {

}