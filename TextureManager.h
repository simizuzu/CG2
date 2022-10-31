#pragma once
#include "DirectXCore.h"

struct TextureData
{
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPUデスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;

	char PADDING[4]{};
	// 横幅
	size_t width = 0;
	// 縦幅
	size_t height = 0;
};

class TextureManager
{
public:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp;

	// テクスチャ数
	UINT texCount;

public:
	void Initialize();
	TextureData LoadTexture(const std::string& filePath);
	static TextureManager* GetInstance();
	
};
