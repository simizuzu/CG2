#pragma once
//#DirectXのやつ
#include"ErrorException.h"

//自作.h
#include"DX12Math.h"
#include"WinApi.h"
#include"DirectXCore.h"
#include"SpriteInterface.h"

class Sprite2D : public SpriteInterface
{
private:


public:

	Sprite2D() = default;
	~Sprite2D() = default;

	//初期化
	virtual void Initialize(ModelShareVaria& modelShareVaria);

	///<summary>
	///読み込み
	///</summary>
	///<param name="filePath">ファイルパス</param>
	virtual void Load(const wchar_t* filePath);

	///<summary>
	///更新
	///</summary>
	///<param name="pos">座標</param>
	///<param name="rot">回転</param>
	///<param name="scale">拡大率</param>
	/// ///<param name="color"> カラー</param>
	virtual void Update(const DX12Math::Vector3& pos = { 0.0f, 0.0f, 0.0f }, const DX12Math::Vector3& rot = { 0.0f, 0.0f, 0.0f }, const DX12Math::Vector3& scale = { 1.0f,1.0f, 1.0f }, const DX12Math::Vector4& color = { 1.0f,1.0f, 1.0f, 1.0f });

	///<summary>
	///描画
	///</summary>
	virtual void Draw(Camera*& camera);

private:
	//シェーダ－リソースビュー生成
	virtual void CreateShaderResourceView();
	//定数バッファ生成(2D座標変換行列)
	virtual void CreatConstBuff();
	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer();
	//テクスチャバッファ生成
	virtual void CreatTextureBuffer();

	//コピーコンストラクタ・代入演算子削除
	Sprite2D& operator=(const Sprite2D&) = delete;
	Sprite2D(const Sprite2D&) = delete;
};