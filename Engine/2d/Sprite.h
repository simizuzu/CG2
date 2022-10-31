#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <string>
#include <memory>
#include <wrl.h>
#include <d3dcompiler.h>

#include "DirectXCore.h"
#include "WinApi.h"
#include "DX12Math.h"
#include "TextureManager.h"

//座標・UV
struct PosUvColor
{
	Math::Vector3 pos;//座標
	Math::Vector2 uv;//uv座標
	Math::Vector4 color;//カラー
};

//2D変換行列(定数バッファ)
struct SpriteConstBufferDataTransform
{
	Math::Matrix4 mat;//2D変換行列
	Math::Matrix4 matWorld;//ワールド座標
};

class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	HRESULT result;
	char PADDING[4]{};
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;

	// 頂点バッファ
	ComPtr<ID3D12Device> verBuff;
	// 頂点マップ
	PosUvColor* vertMap;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// インデックスバッファの生成
	ComPtr<ID3D12Device> indexBuff;
	// インデックスバッファをマッピング
	uint16_t* indexMap;
	// インデックスバッファビューの生成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// プロジェクション行列
	Math::Matrix4 matProjection;

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	Math::Matrix4* constBuffMap = nullptr;

	bool initializeFlag = true;
	char PADDING[7]{};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(TextureData& textureData, Math::Vector3 pos, Math::Vector3 scale, Math::Vector3 rot, Math::Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 頂点バッファ・インデックス生成
	/// </summary>
	void CreateVertexIndexBuffer();

	void CreateConstBuff();

private:
	//定数バッファのマッピング用ポインタ
	//SpriteConstBufferDataTransform* constMapTransform = nullptr;
};

