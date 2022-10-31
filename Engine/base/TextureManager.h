#pragma once
#include "DirectXCore.h"

struct TextureData
{
	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//GPU�f�X�N���v�^�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
	// �f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange;

	char PADDING[4]{};
	// ����
	size_t width = 0;
	// �c��
	size_t height = 0;
};

class TextureManager
{
public:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp;

	// �e�N�X�`����
	UINT texCount;

public:
	void Initialize();
	TextureData LoadTexture(const std::string& filePath);
	static TextureManager* GetInstance();
	
};
