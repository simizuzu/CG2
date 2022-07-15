#pragma once
#include"ErrorException.h"
#include"DX12Math.h"
#include"Camera.h"

class SpriteInterface
{
protected:
	HRESULT result;
	char PADDING[4];
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> cmdList = nullptr;
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	//スプライト数
	UINT nextIndex;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosUvColor* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//テクスチャバッファ
	Microsoft::WRL::ComPtr <ID3D12Resource> texBuff = nullptr;
	//GPUデスクプリタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	//CPUデスクプリタハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;
	//ワールド座標
	DX12Math::Matrix4 matWorld;
	//プロジェクション行列
	DX12Math::Matrix4 matProjection;
public:

	SpriteInterface() = default;

	virtual ~SpriteInterface() = default;

	//初期化
	virtual void Initialize(ModelShareVaria& modelShareVaria) = 0;


	///<summary>
	///読み込み
	///</summary>
	///<param name="filePath">ファイルパス</param>
	virtual void Load(const wchar_t* filePath) = 0;

	///<summary>
	///更新
	///</summary>
	///<param name="pos">座標</param>
	///<param name="rot">回転</param>
	///<param name="scale">拡大率</param>
	/// ///<param name="color"> カラー</param>
	virtual void Update(const DX12Math::Vector3& pos = { 0.0f, 0.0f, 0.0f }, const DX12Math::Vector3& rot = { 0.0f, 0.0f, 0.0f }, const DX12Math::Vector3& scale = { 1.0f,1.0f, 1.0f }, const DX12Math::Vector4& color = { 1.0f,1.0f, 1.0f, 1.0f }) = 0;

	///<summary>
	///描画
	///</summary>
	virtual void Draw(Camera*& camera) = 0;

protected:
	//シェーダ－リソースビュー生成
	virtual void CreateShaderResourceView() = 0;
	//定数バッファ生成(2D座標変換行列)
	virtual void CreatConstBuff() = 0;
	//頂点バッファ・インデックス生成
	virtual void CreatVertexIndexBuffer() = 0;
	//テクスチャバッファ生成
	virtual void CreatTextureBuffer() = 0;

	//コピーコンストラクタ・代入演算子削除
	SpriteInterface& operator=(const SpriteInterface&) = delete;
	SpriteInterface(const SpriteInterface&) = delete;
};