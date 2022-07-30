/**インクルード部**/
#include "GravityComponent.h"
#include "TransformComponent.h"

/**定数定義**/

/**
* @fn		CGravity::CGravity
* @brief	コ、コンストラクタ！！無事だったのか！！！
*/
CGravity::CGravity()
	:m_pTransform(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CGravity::~CGravity
* @brief	デストラクタ、サヨナラだ
*/
CGravity::~CGravity() {

}

/**
* @fn		CGravity::Start
* @brief	初期化処理
*/
void CGravity::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		CGravity::Update
* @brief	更新処理
*/
void CGravity::Update() {
	m_pTransform->Vel.y -= GRAVITY;
	if (m_pTransform->Vel.y < MAX_VELOCITY) {
		m_pTransform->Vel.y = -MAX_VELOCITY;
	}
}

/**
* @fn		CGravity::SetUse
* @brief	更新処理は・・・止まらねぇ！！(止まる)
* @param	(bool)	更新を止めたり、再開したりする関数
*/
void CGravity::SetUse(bool use) {
	Parent->GetComponent<CGravity>()->m_bUpdateFlag = use;
}
