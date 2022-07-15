#include "Sprite3D.h"

//定数バッファの生成
void Sprite3D::CreateShaderResourceView()
{
	//SRVヒープの先頭ハンドルを取得
	cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//ハンドルのポインタずらし
	cpuHandle.ptr += static_cast<unsigned long long>(nextIndex) * incrementSize;
	gpuHandle.ptr += static_cast<unsigned long long>(nextIndex) * incrementSize;

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, cpuHandle);

	nextIndex++;
}

//定数バッファ生成
void Sprite3D::CreatConstBuff()
{
	//定数バッファ生成
	DirectXCore::GetInstance()->CreateConstBuff(constMapTransform, constBuffTransform);
}

//頂点バッファ・インデックス生成
void Sprite3D::CreatVertexIndexBuffer()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(PosUvColor) * 4);

	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点１つ分のデータサイズ
	vbView.StrideInBytes = sizeof(PosUvColor);

	// インデックスデータのサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);
	// 頂点バッファの設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf()));

	//インデックスバッファをマッピング
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	assert(SUCCEEDED(result));
}

//テクスチャバッファ生成
void Sprite3D::CreatTextureBuffer()
{
	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//幅
	textureResourceDesc.Height = (UINT)metadata.height;//高さ
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(texBuff.ReleaseAndGetAddressOf()));

	//テクスチャバッファにデータ転送
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // 全領域へコピー
			img->pixels,          // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

}

void Sprite3D::Initialize(ModelShareVaria& modelShareVaria)
{
	device = DirectXCore::GetInstance()->GetDevice();
	cmdList = DirectXCore::GetInstance()->GetCommandList();

	descriptorRange = modelShareVaria.descriptorRange;
	nextIndex = modelShareVaria.nextIndex;
	pipelineState = modelShareVaria.pipelineState;
	rootSignature = modelShareVaria.rootSignature;
	srvHeap = modelShareVaria.srvHeap;
}

//コンストラクタ
void Sprite3D::Load(const wchar_t* filePath)
{
	device = DirectXCore::GetInstance()->GetDevice();
	cmdList = DirectXCore::GetInstance()->GetCommandList();

	// WICテクスチャのロード
	result = LoadFromWICFile(
		filePath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));

	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	//テクスチャバッファ生成
	CreatTextureBuffer();

	//シェーダ－リソースビュー生成
	CreateShaderResourceView();

	//頂点バッファ・インデックス生成
	CreatVertexIndexBuffer();

	//定数バッファ生成(2D座標変換行列)
	CreatConstBuff();

	//画像の半分のサイズ
	float halfW = static_cast<float>(metadata.width) * 0.25f;
	float halfH = static_cast<float>(metadata.height) * 0.25f;

	// 頂点データ
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{-halfW, halfH, 0.0f},{},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//左上インデックス0
		{{-halfW, -halfH, 0.0f},{},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//左下インデックス1
		{{halfW, halfH, 0.0f},{},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},//右上インデックス2
		{{halfW, -halfH, 0.0f},{},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},//右下インデックス3
	};

	// インデックスデータ
	uint16_t indices[] =
	{
		1, 0, 3, // 三角形1つ目
		0, 3, 2, // 三角形2つ目
	};

	//頂点バッファへのデータ転送
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	//インデックスバッファへのデータ転送
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // インデックスをコピー
	}
	matWorld = DX12Math::MakeIdentity();
}

void Sprite3D::Update(const DX12Math::Vector3& pos, const DX12Math::Vector3& rot, const DX12Math::Vector3& scale, const DX12Math::Vector4& color)
{
	//カラー
	for (int i = 0; i < 4; i++)
	{
		vertMap[i].color = color;
	}

	DX12Math::Matrix4 mTrans, mRot, mScale;
	//平行移動行列
	mTrans.MakeTranslation(pos);
	//回転行列
	mRot.MakeRotation(rot);
	//スケール行列
	mScale.MakeScaling(scale);
	//ワールド行列
	matWorld = mScale * mRot * mTrans;
}

//描画
void Sprite3D::Draw(Camera*& camera)
{

	constMapTransform->mat = matWorld * camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファビューの設定コマンド
	cmdList->IASetIndexBuffer(&ibView);

	// 定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	cmdList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());

	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, gpuHandle);

	// 描画コマンド
	cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
