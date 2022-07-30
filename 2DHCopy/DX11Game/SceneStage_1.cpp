/**インクルード部**/
#include "SceneStage_1.h"
#include "imgui.h"

#include "Object.h"
#include "ObjInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "Draw3dComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"

#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"

/**
* @fn		SceneStage_1::SceneStage_1
* @brief	コンストラクタは嘆いている
*/
SceneStage_1::SceneStage() {

}

/**	
* @fn		SceneStage_1::~SceneStage_1
* @brief	デストラクタ、お前の父は私だ
*/
SceneStage_1::~SceneStage_1{

}