//�C���N���[�h��
#include "Draw3dComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "ObjInfo.h"
#include "sceneGame.h"
#include "Animation.h"

/**
* @fn		CDraw3D::CDraw3D
* @brief	�܂��R���X�g���N�^��y�����
*/
CDraw3D::CDraw3D() 
	:m_pTransform(nullptr),m_pMaterial(nullptr),m_pAnim(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	//mat.Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// �A���r�G���g+�e�N�X�`���L��
	//mat.Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// �f�B�t���[�Y
	//mat.Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// �X�y�L����+�X�y�L�������x
	//mat.Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// �G�~�b�V�u
	//mat.pTexture = nullptr;
	//mat.pTexEmmisive = nullptr;
	//mat.pTexSpecular = nullptr;
	//mat.pTexTransparent = nullptr;
}

/**
* @fn		CDraw3D::~CDraw3D
* @brief	�f�f�f�X�g���N�^�剤
*/
CDraw3D::~CDraw3D() {
	if (m_pMaterial) {
		delete m_pMaterial;
		m_pMaterial = m_pMaterial;
	}

	if (m_pAnim) {
		delete m_pAnim;
		m_pAnim = nullptr;
	}
}

/**
* @fn		CDraw3D::Start()
* @brief	���J�n
*/
void CDraw3D::Start() {
	//�I�u�W�F�N�g�̍��W���擾
	m_pTransform = Parent->GetComponent<CTransform>();

	/*if (m_model)
	{
		auto material = m_model->GetMate();
		m_Material = *material;
	}*/
}

/**
* @fn		CDraw3D::Update
* @brief	�X�V����
*/
void CDraw3D::Update() {

	if (m_pAnim) {
		m_pAnim->Update();
	}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	//���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	//�g�k�𔽓]
	mtxScale = XMMatrixScaling(
		m_pTransform->Scale.x,
		m_pTransform->Scale.y,
		m_pTransform->Scale.z
	);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	//X��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		0.0f,
		0.0f
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//Y��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.y),
		0.0f
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//Z��]�𔽉f
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.z)
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//�ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(
		m_pTransform->Pos.x,
		m_pTransform->Pos.y,
		m_pTransform->Pos.z
	);
	mtxWorld = XMMatrixMultiply(mtxWorld,mtxTranslate);

	//���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&m_mtxWorld,mtxWorld);
}

/**
* @fn		CDraw3D::Draw
* @brief	�`�揈��
*/
void CDraw3D::Draw() {
	//�f�o�C�X�R���e�N�X�g�̎擾
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//�s����������`��
	//���C�g�I�t
	CLight::Get()->SetDisable();
	//�A���t�@�u�����h�L��
	SetBlendState(BS_NONE);
	//Z�o�b�t�@�L��
	SetZBuffer(true);
	m_model->Draw(pDC,m_mtxWorld,eOpacityOnly);

	//������������`��
	//���u�����h�L��
	SetBlendState(BS_ALPHABLEND);
	//Z�o�b�t�@�X�V����
	SetZWrite(false);
	m_model->Draw(pDC,m_mtxWorld,eTransparentOnly);

	//���C�g�I��
	CLight::Get()->SetEnable();
	//Z�o�b�t�@�X�V����
	SetZWrite(true);
	SetBlendState(BS_NONE);
}

/**
* @fn		Draw3D::SetPrimitiveType
* @brief	���f���̃v���~�e�B�u�ݒ�
* @param	(D3D_PRIMITIVE_TOPOLOGY)	�v���~�e�B�u�̏��H
*/
void CDraw3D::SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type) {
	if (m_model != nullptr) {
		m_model->SetPrimitiveType(type);
	}
}

/**
* @fn		CDraw3D::GetWorldPos
* @brief    ���[���h�s��̎擾
* @return	(XMFLOAT4X4*)	�s��̓������|�C���^��Ԃ�
*/
XMFLOAT4X4* CDraw3D::GetWorldPos() {
	if (m_model != nullptr) {
		return &(m_model->GetWorldMatrix());
	}
	return nullptr;
}

/**
* @fn		CDraw3D::SetTexture
* @brief	�e�N�X�`���̐ݒ�
* @param	(ID3D11ShaderResourceView*)	�e�N�X�`���̏����s��
*/
void CDraw3D::SetTexture(ID3D11ShaderResourceView* pTexture) {
	if (pTexture) {
		if (!m_pMaterial) {
			m_pMaterial = new TAssimpMaterial();
		}
		m_pMaterial->pTexture = pTexture;
		m_model->SetMaterial(m_pMaterial);
	}
}

/**
* @fn		CDraw3D::GetBox
* @brief	�傫���̎擾
* @return	(XMFLOAT3)	�c�����̒l��Ԃ�
*/
XMFLOAT3 CDraw3D::GetBox() {
	return m_model->GetBBox();
}

/**
* @fn		CDra3D::GetModel
* @brief	���f���̏����擾
* @return	(CAssimpModel*)	���f���̏�񂪓������|�C���^��Ԃ�
*/
CAssimpModel* CDraw3D::GetModel(){
	return m_model;
}

/**
* @fn		CDraw3D::SetModel
* @brief	���f���̏��̐ݒ�
* @param	(CAssimpModel*)	���f���̏���p�ӂ���
*/
void CDraw3D::SetModel(CAssimpModel* model) {
	//���f���̃Z�b�g
	m_model = model;

	//�A�j���[�V��������������ǉ�
	if (model->HasAnimation()) {
		//�A�j���[�V�����N���X�̍쐬
		if (!m_pAnim) {
			m_pAnim = new Animation;

		}
		m_pAnim->SetModel(this);
	}
}

/**
* @fn		CDraw3D::AddAnimation
* @brief	�A�j���[�V�����̒ǉ�
* @param	(int)		�A�j���[�V�����̔ԍ�
* @param	(double)	�J�n����
* @param	(double)	�I������
* @param	(double)	���[�v�̎n�܂�̎���
* @param	(bool)		���[�v���邩�ǂ���
*/
void CDraw3D::AddAnimation(int AnimID,double Start,double end, bool loop,double loopStartFrame) {
	if (m_pAnim) {
		m_pAnim->AddAnimation(AnimID,Start,end,loop,loopStartFrame);
	}
}

/**
* @fn		CDraw3D::Play
* @brief	�A�j���[�V�������Đ�����
* @param	(int)���f���̔ԍ�
*/
void CDraw3D::Play(int AnimID) {
	if (m_pAnim) {
		m_pAnim->Play(AnimID);
	}
}

/**
* @fn		CDraw3D::SetAnimTime
* @brief	�A�j���[�V�����̃t���[���ݒ�
* @double	(dounle)	�ǂ̈ʂ̃t���[���ɂ��邩�̐��l
*/
void CDraw3D::SetAnimTime(double dTime) {
	m_model->SetAnimTime(dTime);
}

/**
* @fn		CDraw3D::AlwaysAnimation
* @brief	�A�j���[�V�������Đ��������邩
* @param	(bool)	�ǂ�����񂾂����
*/	
void CDraw3D::AlwaysAnimation(bool always) {
	if (m_pAnim) {
		m_pAnim->AlwaysAnimation(always);
	}
}

/**
* @fn		CDraw3D::SetPlaySpeed
* @brief	�Đ����x�̐ݒ�
* @param	(int)	���f���̔ԍ�	
* @param	(double)�Đ����x
*/
void CDraw3D::SetPlaySpeed(int AnimID,double speed) {
	if (m_pAnim) {
		m_pAnim->SetPlaySpeed(AnimID,speed);
	}
}
/**
* @fn		CDraw3D::AnimEnd
* @brief	�A�j���[�V�������I��点�邩
* @param	(int)	���f���̔ԍ�
* @return	(bool)	�I��点�ꂽ���ǂ���
*/
bool CDraw3D::AnimEnd(int AnimID){
	if (m_pAnim) {
		return m_pAnim->AnimEnd(AnimID);
	}
	else {
		return false;
	}
}