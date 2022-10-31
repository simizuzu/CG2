#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Initialize()
{
	device = DirectXCore::GetInstance()->GetDevice();
	cmdList = DirectXCore::GetInstance()->GetCommandList();

	float width = static_cast<float>(WinApi::GetInstance()->GetWindowSize().window_width);
	float height = static_cast<float>(WinApi::GetInstance()->GetWindowSize().window_height);

	CreateVertexIndexBuffer();
	CreateConstBuff();
}

void Sprite::Draw(TextureData& textureData, Math::Vector3 pos, Math::Vector3 scale, Math::Vector3 rot, Math::Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	float left = (0.0f - anchorpoint.x) * textureData.width;
	float right = (1.0f - anchorpoint.x) * textureData.height;
	float top = (0.0f - anchorpoint.x) * textureData.width;
	float bottom = (1.0f - anchorpoint.x) * textureData.height;

	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{left, top, 0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},		//左上インデックス0
		{{left, bottom, 0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},	//左下インデックス1
		{{right, top, 0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},		//右上インデックス2
		{{right, bottom, 0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},	//右下インデックス3
	};

	// インデックスデータ
	uint16_t indices[] =
	{
		1,0,3,	// 三角形1つ目
		2,3,0,	// 三角形2つ目
	};

	// 頂点バッファへのデータ転送
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	// インデックスバッファへのデータ転送
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	Math::Matrix4 mTrans, mRot, mScale, matWorld;
	// 平行移動行列
	mTrans.MakeTranslation(pos.x, pos.y, pos.z);
	// 回転行列
	mRot.MakeRotation(rot.x, rot.y, rot.z);
	// スケール行列
	mScale.MakeScaling(scale.x, scale.y, scale.z);
	// ワールド行列
	matWorld = mScale * mRot * mTrans;


}

void Sprite::CreateVertexIndexBuffer()
{
#pragma region 頂点バッファ
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(PosUvColor) * 4);

	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(PosUvColor);
#pragma endregion
#pragma region 頂点インデックス
	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);

	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファをマッピング
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	assert(SUCCEEDED(result));
#pragma endregion
}

void Sprite::CreateConstBuff()
{
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(Math::Matrix4) + 0xff) & ~0xff; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 定数バッファのマッピング
	result = constBuff->Map(0, nullptr, (void**)&constBuffMap); // マッピング
	assert(SUCCEEDED(result));
}
