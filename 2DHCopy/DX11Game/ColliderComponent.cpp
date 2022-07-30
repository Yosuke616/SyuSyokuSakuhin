//�C���N���[�h��
#include "ColliderComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "PlayerComponent.h"

//�ÓI�����o�ϐ�
/** @brief �����蔻��̃��X�g*/
std::list<Object*> CCollider::m_ColliderList;

/**
* @fn		CCollider::CCollider()
* @brief	�����̃R���X�g���N�^����ȏ�ł��ȉ��ł��Ȃ�
*/
CCollider::CCollider()
	:m_pTransform(nullptr){
	m_eUpdateOrder = COM_UPDATE_COLLIDER;		//�R���|�[�l���g���X�g�̍X�V����

	m_CenterOffset = XMFLOAT2(0.0f,0.0f);
	m_CollisionSize = XMFLOAT3(0.0f,0.0f,0.0f);
}

/**
* @fn		CCollider:~CCollider
* @brief	�f�X�g���N�^����������
*/
CCollider::~CCollider() {

}

/**
* @fn		CCollider:Start
* @brief	������
*/
void CCollider::Start() {
	//�R���|�[�l���g�̎擾
	//�I�u�W�F�N�g�̍��W�̎擾
	m_pTransform = Parent->GetComponent<CTransform>();
	
	//�����蔻��̃��X�g�ɓo�^
	AddColliderList(Parent);

}

/**
* @fn		Collder::Update
* @brief	�X�V����
*/
void CCollider::Update() {
	//�Փ˔��胊�X�g�ւ̒ǉ����[�v
	for (auto&& OtherObject:m_ColliderList) {
		//�ǂ��炩�̍폜�t���O��true�������ꍇ continue
		if (Parent->GetDeleteFlag() == true) {
			continue;
		}
		if (OtherObject->GetDeleteFlag() == true) {
			continue;
		}

		//�R���|�[�l���g���@�\��~�̎� continue
		auto OtherCollider = OtherObject->GetComponent<CCollider>();
		if (this->m_bUpdateFlag == false) {
			continue;
		}
		if (OtherCollider->m_bUpdateFlag == false) {
			continue;
		}

		//�����蔻������Ώۂ����̃I�u�W�F�N�g�������ꍇ continue
		if (OtherObject == Parent) {
			continue;
		}

		//�����蔻������Ώۂ̏����擾
		auto Other = OtherObject->GetComponent<CTransform>();

		//2��ڂ̏Փ˔���������
		bool bOnceHit = false;

		for (auto OnceCollision:this->m_IntersectObjects) {
			//�����������������Ƃ�����I�u�W�F�N�g�̃��X�g == ���݂̓����蔻��������
			if (OnceCollision == OtherObject) {
				bOnceHit = true;
				break;
			}
		}
		if (bOnceHit) {
			continue;
		}

		//x,y���ʂ̓����蔻��
		if (this->CollisionRectToRectXY(
			m_pTransform,Other,
			XMFLOAT2(m_CollisionSize.x,m_CollisionSize.y),
			XMFLOAT2(OtherCollider->m_CollisionSize.x,
				OtherCollider->m_CollisionSize.y),
			m_CenterOffset,
			OtherCollider->m_CenterOffset)) {
			//����ɓ����������Ƃ�m�点��
			OtherCollider->m_IntersectObjects.push_back(Parent);
		}
	}
}

/**
* @fn		CCollider::SetCollisionSize
* @brief	�����蔻��̑傫���̐ݒ�
* @param	(float)		�����蔻��̉���
* @param	(float)		�����蔻��̗���
* @param	(float)		�����蔻��̉��s
*/
void CCollider::SetCollisionSize(float fWidth,float fHeight,float fz) {
	m_CollisionSize.x = fWidth;
	m_CollisionSize.y = fHeight;
	m_CollisionSize.z = fz;
}

/**
* @fn		CCollider::SetOffset
* @brief	�I�u�W�F�N�g�Ɠ����蔻��̂���𒼂����߂̊֐�
* @param	(float)		x�����̂���𒼂�
* @param	(float)		y�����̂���𒼂�
*/
void CCollider::SetOffset(float fx,float fy) {
	m_CenterOffset.x = fx;
	m_CenterOffset.y = fy;
}

/**
* @fn		CCollider::GetOffset
* @brief	�ǂ̈ʃY���Ă��邩���擾����
* @return	(XMFLOAT2)	�ǂ̈ʒ��S���炸��Ă��邩�̏��
*/
XMFLOAT2 CCollider::GetOffSet() {
	return m_CenterOffset;
}

/**
* @fn		CCollider::GetColliderSize
* @brief	�����蔻��̑傫�����擾����
* @return	(XMFLOAT3)	�����蔻��̑傫���̏��
*/
XMFLOAT3 CCollider::GetColliderSize() {
	return m_CollisionSize;
}

/**
* @fn		CCollider::GetCenterPos
* @brief	�����蔻������S���W���擾����
* @return	(XMFLOAT2)	�����蔻��̒��S�̍��W�̏��
*/
XMFLOAT2 CCollider::GetCenterPos() {
	return XMFLOAT2(m_pTransform->Pos.x + m_CenterOffset.x,m_pTransform->Pos.y + m_CenterOffset.y);
}

/**
* @fn		CCollider::CheckCollisionRectToRect
* @brief	��`���m�̓����蔻��
* @param	(XMFLOAT2)	�Е��̓����蔻��̒��S���W
* @param	(XMFLOAT2)	�����Е��̓����蔻��̒��S���W
* @param	(XMFLOAT2)	�Е��̓����蔻��̑傫��
* @param	(XMFLOAT2)	�����Е��̓����蔻��̑傫��
* @param	(XMFLOAT2)	�Е��̓����蔻��̑��x
* @param	(XMFLOAT2)	�����Е��̓����蔻��̑��x
* @return	(bool)		�������Ă������ǂ����̐^�U��Ԃ�
*/
bool CCollider::CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
										 XMFLOAT2 size1, XMFLOAT2 size2,
										 XMFLOAT2 Velocity1, XMFLOAT2 Velocity2) {
	XMFLOAT2 halfSize1 = XMFLOAT2(size1.x / 2.0f,size1.y / 2.0f);
	XMFLOAT2 halfSize2 = XMFLOAT2(size2.x / 2.0f,size2.y / 2.0f);

	if (centerPos2.x - halfSize2.x + Velocity2.x < centerPos1.x + halfSize1.x + Velocity1.x &&
		centerPos1.x - halfSize1.x + Velocity1.x < centerPos2.x + halfSize2.x + Velocity2.x) {

		if (centerPos2.y - halfSize2.y + Velocity2.y < centerPos1.y + halfSize1.y + Velocity1.y &&
			centerPos1.y - halfSize1.y + Velocity1.y < centerPos2.y + halfSize2.y + Velocity2.y){
			
			return true;
		}
	}
	return false;
}

/**
* @fn		CCollider::CheckCollisionCircleToCircle
* @brief	�~�Ɖ~�̓����蔻��
* @param	(XMFLOAT2)	�~�̒��S�̍��W
* @param	(XMFLOAT2)	�����Е��̉~�̒��S�̍��W
* @param	(float)		�~�̔��a
* @param	(float)		�����Е��̉~�̔��a
* @return	(bool)		�������������ۂ̐^�U�̔���
*/
bool CCollider::CheckCollisionCircleToCircle(XMFLOAT2 centerPos1,XMFLOAT2 centerPos2,
											 float radius1,float radius2) {
	if ((centerPos1.x - centerPos2.x) * (centerPos1.x - centerPos2.x)
		+ (centerPos1.y - centerPos2.y) * (centerPos1.y - centerPos2.y)
		<= (radius1 + radius2) * (radius1 * radius2)) {
	
		return true;
	}
	return false;
}

/**
* @fn		CCollider::CollisionSphere
* @brief	���Ƌ��̓����蔻��
* @param	(XMFLOAT3)	���̍��W
* @param	(XMFLOAT3)	�����Е��̋��̍��W
* @param	(float)		���̔��a
* @param	(float)		�����Е��̋��̔��a
* @return	(bool)		�����������ǂ����̐^�U�̔���
*/
bool CCollider::CollisionSphere(XMFLOAT3 Apos,XMFLOAT3 Bpos,float Ar,float Br) {
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

/**
* @fn		CCollider::CollisionAABB
* @brief	AABB���m�̓����蔻��
* @param	(XMFLOAT3)	�{�b�N�X�̍��W
* @param	(XMFLOAT3)	�����Е��̃{�b�N�X�̍��W
* @param	(XMFLOAT3)	�{�b�N�X�̑傫��
* @param	(XMFLOAT3)	�����Е��̃{�b�N�X�̑傫��
* @return	(bool)		�������Ă��邩�ǂ����̐^�U�̔���	
*/
bool CCollider::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Bpos, XMFLOAT3 Asize, XMFLOAT3 Bsize) {
	return(Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		  (Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		  (Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		  (Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		  (Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		  (Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

/**
* @fn		CCollider::CollisionRectToRectXY
* @brief	xy���ʂƂ̓����蔻��
* @param	(CTransform*)	�I�u�W�F�N�g�̏�񂪂���|�C���^
* @param	(CTransform*)	�����Е��̃I�u�W�F�N�g�̏�񂪂���|�C���^
* @param	(XMFLOAT2)		�I�u�W�F�N�g�̑傫��
* @param	(XMFLOAT2)		�����Е��̃I�u�W�F�N�g�̑傫��
* @param	(XMFLOAT2)		�I�u�W�F�N�g�̒��S�̃Y��
* @param	(XMFLOAT2)		�����Е��̃I�u�W�F�N�g�̒��S�̃Y��
* @return	(bool)			�������Ă��邩�ǂ����̐^�U�̔���
*/
bool CCollider::CollisionRectToRectXY(CTransform* pA, CTransform* pB,
								      XMFLOAT2 Asize, XMFLOAT2 Bsize,
									  XMFLOAT2 Aoffset, XMFLOAT2 Boffset) {
	//���S���W
	XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + Aoffset.x,pA->Pos.y + Aoffset.y);
	XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + Boffset.x,pB->Pos.y + Boffset.y);
	XMFLOAT2 halfSizeA = XMFLOAT2(Asize.x / 2.0f,Asize.y / 2.0f);
	XMFLOAT2 halfSizeB = XMFLOAT2(Bsize.x / 2.0f,Bsize.y / 2.0f);

	if (centerPosB.x - halfSizeB.x < centerPosA.x + halfSizeA.x &&
		centerPosA.x - halfSizeA.x < centerPosB.x + halfSizeB.x) {
		if (centerPosB.y - halfSizeB.y < centerPosA.y + centerPosA.y &&
			centerPosA.y - halfSizeA.y < centerPosB.y + halfSizeB.y) {
			return true;
		}
	}

	return false;
}

/**
* @fn		CCollider::AddColliderList
* @brief	�����蔻��̃��X�g�ɒǉ�����
* @param	(Object*)	���X�g�ɒǉ�����I�u�W�F�N�g�̃|�C���^
*/
void CCollider::AddColliderList(Object* pObject) {
	//�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_ColliderList.begin();
	for (auto&& object:m_ColliderList) {
		if (pObject->GetUpdateOrder() > object->GetUpdateOrder()) {
			itr++;
			continue;
		}
		else {
			break;
		}
	}
	m_ColliderList.insert(itr,pObject);
}

/**
* @fn		CCollider::GetColliderList
* @brief	�����蔻��̃��X�g�̎擾
* @return	(list<Object*>)&		�����蔻��̓������I�u�W�F�N�g�^�̃��X�g�̃A�h���X��Ԃ�
*/
std::list<Object*>& CCollider::GetColliderList() {
	return m_ColliderList;
}

/**
* @fn		CCollider::UpdateColliders
* @brief	�����蔻�胊�X�g�ɓ����Ă���I�u�W�F�N�g�̍X�V
*/
void CCollider::UpdateColliders() {
	for (auto this_object:m_ColliderList) {
		//�I�u�W�F�N�g�̖��O���v���C���[�̖��O����Ȃ������ꍇ
		if (this_object->GetName() != PLAYER_NAME) {
			//�R���C�_�[�̋@�\�������Ă���
			auto this_collider = this_object->GetComponent<CCollider>();

			//�Փˎ��̏���
			//�Փ˂����I�u�W�F�N�g�F�Փ˂������肽��
			for (auto collided_object:this_collider->m_IntersectObjects) {
				//���̃I�u�W�F�N�g�̏Փˏ��������s
				this_object->OnCollisionEnter(collided_object);
				//�Փ˂������肽���̏Փˏ��������s
				collided_object->OnCollisionEnter(this_object);
			}
		}
		//�v���C���[�������ꍇ
		else {
			auto this_collider = this_object->GetComponent<CCollider>();

			//��ł���Ƃ������Ƃ͂Ȃ��Ƃ������Ƃł���
			if (this_collider->m_IntersectObjects.size() > 1) {
				this_collider->m_IntersectObjects.sort([=](Object* A, Object* B) {
					//���W�̎擾
					auto thisPos = this_collider->GetCenterPos();
					auto PosA = A->GetComponent<CCollider>()->GetCenterPos();
					auto PosB = B->GetComponent<CCollider>()->GetCenterPos();
					//�����̂̌v�Z
					XMFLOAT2 DiffA(thisPos.x - PosA.x, thisPos.y - PosA.y);
					XMFLOAT2 DiffB(thisPos.x - PosB.x, thisPos.y - PosB.y);

					//���������ɓ�ł�����
					XMVECTOR vDiffA = DirectX::XMLoadFloat2(&DiffA);
					XMVECTOR vDiffB = DirectX::XMLoadFloat2(&DiffB);
					XMVECTOR vLengthA;
					XMVECTOR vLengthB;
					vLengthA = DirectX::XMVector2LengthEst(vDiffA);
					vLengthB = DirectX::XMVector2LengthEst(vDiffB);

					//Diff�̍ė��p
					DirectX::XMStoreFloat2(&DiffA,vLengthA);
					DirectX::XMStoreFloat2(&DiffB,vLengthB);

					return DiffA.x < DiffB.x;
				});
			}

			//�Փ˂����Ƃ��̏���
			//�Փ˂����I�u�W�F�N�g�F�Փ˂������肽��
			for (auto collided_object:this_collider->m_IntersectObjects) {
				//���̃I�u�W�F�N�g�̏Փˏ����̎��s
				this_object->OnCollisionEnter(collided_object);
				//�Փ˂�������̏Փˏ������s
				collided_object->OnCollisionEnter(this_object);
			}
		}
		//�Փ˂������Ƃ�m�点��̂���߂�
		this_object->GetComponent<CCollider>()->m_IntersectObjects.clear();

	}
}
