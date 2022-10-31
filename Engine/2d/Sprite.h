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

//���W�EUV
struct PosUvColor
{
	Math::Vector3 pos;//���W
	Math::Vector2 uv;//uv���W
	Math::Vector4 color;//�J���[
};

//2D�ϊ��s��(�萔�o�b�t�@)
struct SpriteConstBufferDataTransform
{
	Math::Matrix4 mat;//2D�ϊ��s��
	Math::Matrix4 matWorld;//���[���h���W
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

	// ���_�o�b�t�@
	ComPtr<ID3D12Device> verBuff;
	// ���_�}�b�v
	PosUvColor* vertMap;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Device> indexBuff;
	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap;
	// �C���f�b�N�X�o�b�t�@�r���[�̐���
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// �v���W�F�N�V�����s��
	Math::Matrix4 matProjection;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	Math::Matrix4* constBuffMap = nullptr;

	bool initializeFlag = true;
	char PADDING[7]{};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Sprite() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(TextureData& textureData, Math::Vector3 pos, Math::Vector3 scale, Math::Vector3 rot, Math::Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// ���_�o�b�t�@�E�C���f�b�N�X����
	/// </summary>
	void CreateVertexIndexBuffer();

	void CreateConstBuff();

private:
	//�萔�o�b�t�@�̃}�b�s���O�p�|�C���^
	//SpriteConstBufferDataTransform* constMapTransform = nullptr;
};

