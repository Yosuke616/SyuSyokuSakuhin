//インクルード部
#include "TextureManager.h"
#include "mainApp.h"
#include "Texture.h"
#include <crtdbg.h>

//静的メンバ変数
TextureManager* TextureManager::m_pInstance = nullptr;

/**
* @fn		TextureManager::TextureManager
* @brief	コンストラクタというのは分かっている
*/
TextureManager::TextureManager() {

}

/**
* @fn		TextureManager::~TextureManager
* @brief	もしかしたらデストラクタだ
*/
TextureManager::~TextureManager() {
	for (auto buff : m_TextureList) {
		SAFE_RELEASE(buff.second);
	}
	m_TextureList.clear();
}

/**
* @fn		TextureManager::GetInstance
* @brief	テクスチャマネージャーのインスタンス化
*/
TextureManager* TextureManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new TextureManager;
	}
	return m_pInstance;
}

/**
* @fn		TextureManager::Uninit
* @brief	テクスチャマネージャーの解放
*/
void TextureManager::Uninit() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		TextureManager::AddTexture
* @brief	テクスチャを追加していく
* @param	(char*)	ファイルの名前が入っているポインタ
* @param	(int)	テクスチャと紐づけるキー
* @return	成功したか失敗したかの真偽
*/
bool TextureManager::AddTexture(const char* pszFileName,int nTexID) {
	//テクスチャがあったら返す
	if (m_TextureList[nTexID] != nullptr) {
		return true;
	}

	//変数宣言
	/** @brief テクスチャの情報が入っているポインタ*/
	ID3D11ShaderResourceView* pTex;
	/** @brief 初期化用変数*/
	HRESULT hr;

	//テクスチャの読込
	hr = CreateTextureFromFile(GetDevice(),pszFileName,&pTex);

	/** @brief 謎である*/
	if (FAILED(hr)) {
		_ASSERT_EXPR(false,L"NO_TEXTURE");
		return false;
	}

	//mapに追加
	m_TextureList[nTexID] = pTex;

	return false;
}

/**
* @fn		TextureManager::GetTexture
* @brief	テクスチャの取得
* @param	(int)	テクスチャと紐づいているキー
* @return	(ID3D~*)あった時使用可能なテクスチャ番号、無かった時０
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(int nTexID) {
	//検索
	auto itr = m_TextureList.find(nTexID);

	//検索結果
	if (itr == m_TextureList.end()) {
		//無かった時
		return nullptr;
	}
	else {
		//あった時
		return itr->second;
	}

	return nullptr;
}

/**
* @fn		TextureManager::EraseTexture
* @brief	テクスチャの削除
* @param	(int)	テクスチャと紐づいているキー
* @return	(bool)	消すテクスチャが歩かないかの真偽
*/
bool TextureManager::EraseTexture(int nTexID) {
	//検索
	auto Tex = this->GetTexture(nTexID);

	//あった時
	if (Tex) {
		m_TextureList.erase(nTexID);
		return true;
	}

	//無かった時
	return false;
}

/**
* @fn		TextureManager::GetSize
* @brief	リストサイズ
* @return	読み込まれているテクスチャの数
*/
int TextureManager::GetSize() {
	//探す
	return (int)(m_TextureList.size());
}


