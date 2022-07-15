#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <random>
#include <DirectXTex.h>

//

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include "WinApi.h"
#include "DirectXCore.h"
#include "Input.h"

using namespace DirectX;

// 定数バッファ用データ構造体（マテリアル）
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色 (RGBA)
};

// 定数バッファ用データ構造体（3D変換行列）
struct ConstBufferDataTransform {
	XMMATRIX mat;
};

//Windowsアプリでのエントリーポイント(main関数)
#pragma region WindowsAPI初期化処理
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	WinApi* winApi = nullptr;
	winApi = winApi->GetInstance();
	winApi->CreateGameWindow();

#pragma endregion

#pragma region DirectX初期化
	HRESULT result;
	DirectXCore* directXCore = nullptr;
	directXCore = directXCore->GetInstance();
	directXCore->InitializeDirectXCore();

#pragma endregion





#pragma region 描画初期化処理

#pragma region 頂点データ初期化
	// 頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;	 // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;	 // uv座標
	};
	// 頂点データ
	Vertex vertices[] = {
		//  x      y      z       u     v
		// 前
		{{-5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{-5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{ 5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{ 5.0f,  5.0f, -5.0f}, {}, {1.0f, 0.0f}}, // 右上
		// 後(前面とZ座標の符号が逆)
		{{-5.0f, -5.0f,  5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{-5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{ 5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{ 5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // 右上
		// 左
		{{-5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{-5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{-5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{-5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // 右上
		// 右（左面とX座標符号が逆）
		{{ 5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{ 5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{ 5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{ 5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // 右上
		// 下
		{{ 5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{ 5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{-5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{-5.0f, -5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // 右上
		// 上（下面とY座標の符号が逆）
		{{ 5.0f,  5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // 左下
		{{ 5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // 左上
		{{-5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // 右下
		{{-5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // 右上
	};

	// インデックスデータ
	unsigned short indices[] =
	{
		// 前
		0, 1, 2,    // 三角形1つ目
		2, 1, 3,    // 三角形2つ目
		// 後（前の面に4加算）
		6, 5, 4,    // 三角形3つ目
		7, 5 ,6,    // 三角形4つ目
		// 左
		10, 9, 8,   // 三角形5つ目
		9, 10, 11,  // 三角形6つ目
		// 右
		12, 13, 14, // 三角形7つ目
		15, 14, 13, // 三角形8つ目
		// 下
		16, 17, 18, // 三角形9つ目
		19, 18, 17, // 三角形10つ目
		// 上
		22, 21, 20, // 三角形11つ目
		21, 22, 23, // 三角形12つ目
	};
#pragma endregion

#pragma region 頂点バッファ

	// 法線の計算
	for (int i = 0; i < _countof(indices) / 3; i++) { // 三角形を一つごとに計算していく
		// 三角形のインデックスを取り出して、一時的な変数を入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		// 三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		// p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// 正規化（長さを1にする）
		normal = XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
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
	result = directXCore->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region 頂点インデックス
	// インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

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
	result = directXCore->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));



	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // インデックスをコピー
	}
	// マッピング解除
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region 定数バッファの生成

	//Sprite* constMapTransform = nullptr;

	// 0番の定数バッファを生成
	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;

	directXCore->Constant(sizeof(ConstBufferDataMaterial), constBuffMaterial);
	directXCore->Constant(sizeof(ConstBufferDataTransform), constBuffTransform0);

	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));

	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);              // RGBAで半透明の赤
	result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0); // マッピング
	assert(SUCCEEDED(result));

	// 1番の定数バッファを生成
	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	directXCore->Constant(sizeof(ConstBufferDataTransform), constBuffTransform1);

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1); // マッピング
	assert(SUCCEEDED(result));

	//// 平行投影変換
	//constMapTransform->mat = DirectX::XMMatrixOrthographicOffCenterLH(
	//	0.0f, winApi->GetWindowSize().window_width,
	//	winApi->GetWindowSize().window_height, 0.0f,
	//	0.0f, 1.0f
	//);

	// ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 50, -50); // 視点座標
	XMFLOAT3 target(0, 0, 0); // 注視点座標
	XMFLOAT3 up(0, 1, 0);	  // 上方向ベクトル

	// 0番のワールド行列の作成

	// スケーリング倍率
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	// 回転角
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	// 座標
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / winApi->GetWindowSize().window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / winApi->GetWindowSize().window_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

#pragma endregion

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WICテクスチャのロード
	result = LoadFromWICFile(
		L"Resources/texture.png",   //「Resources」フォルダの「texture.png」
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);


	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result = directXCore->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
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

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト
	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	};

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で描いたほうが見やすい)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// 設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = directXCore->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = metadata.mipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	directXCore->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;                   // ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // ソースの値を100% 使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // デストの値を  0% 使う

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// その他の設定
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;         //一度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;     //テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// 定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // 種類
	rootParams[0].Descriptor.ShaderRegister = 0;                   // 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;                    // デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // 全てのシェーダから見える
	// テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;		    //デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              		//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;               //全てのシェーダから見える
	// 定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // 種類
	rootParams[2].Descriptor.ShaderRegister = 1;                   // 定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;                    // デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // 全てのシェーダから見える

	// テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;                                              //ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;           //ピクセルシェーダからのみ使用可能

	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; //ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);        //ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directXCore->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; // 深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 描き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度地フォーマット

	// パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = directXCore->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region Input初期化処理
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input* input = new Input();
	input->Initialize();
#pragma endregion

	float angle = 0.0f; // カメラの回転角

#pragma region ゲームループ
	//ゲームループ
	while (true) {
		if (winApi->WindowMessage()) {
			break;
		}

		input->Update();

		directXCore->DrawStart();

		//DirectX毎フレーム処理　ここから
		// ビュー変換
		matView = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		// 透視投影変換
		XMMATRIX matProjection = DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f),
			(float)winApi->GetWindowSize().window_width / winApi->GetWindowSize().window_height, 0.1f, 1000.0f);

		// 定数バッファに転送
		constMapTransform0->mat = matView * matProjection;

		// プレイヤーを回転させる処理
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
			if (input->PushKey(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			// angleラジアンだけY軸回りに回転。半径は-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);
		}

		// 敵を回転させる処理


		// プレイヤー座標を移動する処理
		if (input->PushKey(DIK_UP)) {
			position.z += 1.0f;
		}
		else if (input->PushKey(DIK_DOWN)) {
			position.z -= 1.0f;
		}
		if (input->PushKey(DIK_RIGHT)) {
			position.x += 1.0f;
		}
		else if (input->PushKey(DIK_LEFT)) {
			position.x -= 1.0f;
		}

		// 0番のワールド変換行列
		XMMATRIX matWorld; // ワールド変換行列
		XMMATRIX matScale; // スケーリング行列
		XMMATRIX matRot;   // 回転行列
		XMMATRIX matTrans; // 平行移動行列

		// 各種変換行列を計算
		matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		matRot = DirectX::XMMatrixIdentity();
		matRot *= DirectX::XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z軸回りに45度回転
		matRot *= DirectX::XMMatrixRotationX(XMConvertToRadians(rotation.x)); // Z軸回りに45度回転
		matRot *= DirectX::XMMatrixRotationY(XMConvertToRadians(rotation.y)); // Z軸回りに45度回転

		matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		matWorld = DirectX::XMMatrixIdentity(); // 変形をリセット
		matWorld *= matScale;					// ワールド行列にスケーリングを反映
		matWorld *= matRot;						// ワールド行列に回転を反映
		matWorld *= matTrans;					// ワールド行列に平行移動を反映

		// 転送
		constMapTransform0->mat = matWorld * matView * matProjection;

		// 1番のワールド行列
		XMMATRIX matWorld1;
		matWorld1 = DirectX::XMMatrixIdentity();
		// 各種変換行列を計算
		XMMATRIX matScale1 = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX matRot1 = DirectX::XMMatrixRotationY(XM_PI / 4.0f);
		XMMATRIX matTrans1 = DirectX::XMMatrixTranslation(-20.0f, 0, 0);
		// ワールド行列を合成
		matWorld1 = matScale1 * matRot1 * matTrans1;
		// ワールド、ビュー、射影行列を合成してシェーダーに転送
		constMapTransform1->mat = matWorld1 * matView * matProjection;

		// 4.描画コマンドここから
		directXCore->GetCommandList()->IASetIndexBuffer(&ibView);

		// パイプラインステートとルートシグネチャの設定コマンド
		directXCore->GetCommandList()->SetPipelineState(pipelineState);
		directXCore->GetCommandList()->SetGraphicsRootSignature(rootSignature);

		// プリミティブ形状の設定コマンド
		directXCore->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		// 頂点バッファビューの設定コマンド
		directXCore->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		// 0番定数バッファビュー(CBV)の設定コマンド
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		// SRVヒープの設定コマンド
		directXCore->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
		// SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
		directXCore->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// 0番定数バッファビュー(CBV)の設定コマンド
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		// 描画コマンド
		directXCore->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		// 1番定数バッファビュー(CBV)の設定コマンド
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		// 描画コマンド
		directXCore->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		// 4.描画コマンドここまで

		//DirectX毎フレーム処理　ここまで
		directXCore->DrawEnd();
	}

#pragma endregion
	return 0;
}