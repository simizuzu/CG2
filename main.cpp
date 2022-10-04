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


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include "WinApi.h"
#include "DirectXCore.h"
#include "Input.h"

using namespace DirectX;

// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // �F (RGBA)
};

// �萔�o�b�t�@�p�f�[�^�\���́i3D�ϊ��s��j
struct ConstBufferDataTransform {
	XMMATRIX mat;
};

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
#pragma region WindowsAPI����������
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	WinApi* winApi = nullptr;
	winApi = winApi->GetInstance();
	winApi->CreateGameWindow();

#pragma endregion

#pragma region DirectX������
	HRESULT result;
	DirectXCore* directXCore = nullptr;
	directXCore = directXCore->GetInstance();
	directXCore->InitializeDirectXCore();

#pragma endregion

#pragma region �`�揉��������

#pragma region ���_�f�[�^������
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;	 // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;	 // uv���W
	};
	// ���_�f�[�^
	Vertex vertices[] = {
		//  x      y      z       u     v
		// �O
		{{-5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{-5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{ 5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{ 5.0f,  5.0f, -5.0f}, {}, {1.0f, 0.0f}}, // �E��
		// ��(�O�ʂ�Z���W�̕������t)
		{{-5.0f, -5.0f,  5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{-5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{ 5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{ 5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // �E��
		// ��
		{{-5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{-5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{-5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{-5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // �E��
		// �E�i���ʂ�X���W�������t�j
		{{ 5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{ 5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{ 5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{ 5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // �E��
		// ��
		{{ 5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{ 5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{-5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{-5.0f, -5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // �E��
		// ��i���ʂ�Y���W�̕������t�j
		{{ 5.0f,  5.0f, -5.0f}, {}, {0.0f, 1.0f}}, // ����
		{{ 5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}}, // ����
		{{-5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}}, // �E��
		{{-5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}}, // �E��
	};

	// �C���f�b�N�X�f�[�^
	unsigned short indices[] =
	{
		// �O
		0, 1, 2,    // �O�p�`1��
		2, 1, 3,    // �O�p�`2��
		// ��i�O�̖ʂ�4���Z�j
		6, 5, 4,    // �O�p�`3��
		7, 5 ,6,    // �O�p�`4��
		// ��
		10, 9, 8,   // �O�p�`5��
		9, 10, 11,  // �O�p�`6��
		// �E
		12, 13, 14, // �O�p�`7��
		15, 14, 13, // �O�p�`8��
		// ��
		16, 17, 18, // �O�p�`9��
		19, 18, 17, // �O�p�`10��
		// ��
		22, 21, 20, // �O�p�`11��
		21, 22, 23, // �O�p�`12��
	};
#pragma endregion

#pragma region ���_�o�b�t�@

	// �@���̌v�Z
	for (int i = 0; i < _countof(indices) / 3; i++) { // �O�p�`������ƂɌv�Z���Ă���
		// �O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ�������
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		// �O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		// p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// �O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// ���K���i������1�ɂ���j
		normal = XMVector3Normalize(normal);
		// ���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = directXCore->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region ���_�C���f�b�N�X
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = directXCore->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));



	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // �C���f�b�N�X���R�s�[
	}
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

#pragma region �萔�o�b�t�@�̐���

	//Sprite* constMapTransform = nullptr;

	// 0�Ԃ̒萔�o�b�t�@�𐶐�
	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffMaterial = nullptr;

	directXCore->Constant(sizeof(ConstBufferDataMaterial), constBuffMaterial);
	directXCore->Constant(sizeof(ConstBufferDataTransform), constBuffTransform0);

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);              // RGBA�Ŕ������̐�
	result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0); // �}�b�s���O
	assert(SUCCEEDED(result));

	// 1�Ԃ̒萔�o�b�t�@�𐶐�
	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	directXCore->Constant(sizeof(ConstBufferDataTransform), constBuffTransform1);

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1); // �}�b�s���O
	assert(SUCCEEDED(result));

	//// ���s���e�ϊ�
	//constMapTransform->mat = DirectX::XMMatrixOrthographicOffCenterLH(
	//	0.0f, winApi->GetWindowSize().window_width,
	//	winApi->GetWindowSize().window_height, 0.0f,
	//	0.0f, 1.0f
	//);

	// �r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 50, -50); // ���_���W
	XMFLOAT3 target(0, 0, 0); // �����_���W
	XMFLOAT3 up(0, 1, 0);	  // ������x�N�g��

	// 0�Ԃ̃��[���h�s��̍쐬

	// �X�P�[�����O�{��
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	// ��]�p
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	// ���W
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	/*constMapTransform->mat.r[0].m128_f32[0] = 2.0f / winApi->GetWindowSize().window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / winApi->GetWindowSize().window_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;*/

#pragma endregion

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/texture.png",   //�uResources�v�t�H���_�́utexture.png�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);


	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	result = directXCore->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	};

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xyz���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŕ`�����ق������₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = directXCore->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = metadata.mipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	directXCore->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

	blenddesc.BlendEnable = true;                   // �u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // �\�[�X�̒l��100% �g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // �f�X�g�̒l��  0% �g��

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;         //��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;     //�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
	rootParams[0].Descriptor.ShaderRegister = 0;                   // �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩����
	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;		    //�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              		//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;               //�S�ẴV�F�[�_���猩����
	// �萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
	rootParams[2].Descriptor.ShaderRegister = 1;                   // �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩����

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                              //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;           //�s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; //���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);        //���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = directXCore->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true; // �[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �`�����݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�n�t�H�[�}�b�g

	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	result = directXCore->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region Input����������
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input* input = new Input();
	input->Initialize();
#pragma endregion

	float angle = 0.0f; // �J�����̉�]�p

#pragma region �Q�[�����[�v
	//�Q�[�����[�v
	while (true) {
		if (winApi->WindowMessage()) {
			break;
		}

		input->Update();

		directXCore->DrawStart();

		//DirectX���t���[�������@��������
		// �r���[�ϊ�
		matView = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		// �������e�ϊ�
		XMMATRIX matProjection = DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f),
			(float)winApi->GetWindowSize().window_width / winApi->GetWindowSize().window_height, 0.1f, 1000.0f);

		// �萔�o�b�t�@�ɓ]��
		constMapTransform0->mat = matView * matProjection;

		// �v���C���[����]�����鏈��
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
			if (input->PushKey(DIK_D)) {
				angle += XMConvertToRadians(1.0f);
			}
			else if (input->PushKey(DIK_A)) {
				angle -= XMConvertToRadians(1.0f);
			}

			// angle���W�A������Y�����ɉ�]�B���a��-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);
		}

		// �G����]�����鏈��


		// �v���C���[���W���ړ����鏈��
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

		// 0�Ԃ̃��[���h�ϊ��s��
		XMMATRIX matWorld; // ���[���h�ϊ��s��
		XMMATRIX matScale; // �X�P�[�����O�s��
		XMMATRIX matRot;   // ��]�s��
		XMMATRIX matTrans; // ���s�ړ��s��

		// �e��ϊ��s����v�Z
		matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		matRot = DirectX::XMMatrixIdentity();
		matRot *= DirectX::XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z������45�x��]
		matRot *= DirectX::XMMatrixRotationX(XMConvertToRadians(rotation.x)); // Z������45�x��]
		matRot *= DirectX::XMMatrixRotationY(XMConvertToRadians(rotation.y)); // Z������45�x��]

		matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

		matWorld = DirectX::XMMatrixIdentity(); // �ό`�����Z�b�g
		matWorld *= matScale;					// ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matRot;						// ���[���h�s��ɉ�]�𔽉f
		matWorld *= matTrans;					// ���[���h�s��ɕ��s�ړ��𔽉f

		// �]��
		constMapTransform0->mat = matWorld * matView * matProjection;

		// 1�Ԃ̃��[���h�s��
		XMMATRIX matWorld1;
		matWorld1 = DirectX::XMMatrixIdentity();
		// �e��ϊ��s����v�Z
		XMMATRIX matScale1 = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX matRot1 = DirectX::XMMatrixRotationY(XM_PI / 4.0f);
		XMMATRIX matTrans1 = DirectX::XMMatrixTranslation(-20.0f, 0, 0);
		// ���[���h�s�������
		matWorld1 = matScale1 * matRot1 * matTrans1;
		// ���[���h�A�r���[�A�ˉe�s����������ăV�F�[�_�[�ɓ]��
		constMapTransform1->mat = matWorld1 * matView * matProjection;

		// 4.�`��R�}���h��������
		directXCore->GetCommandList()->IASetIndexBuffer(&ibView);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		directXCore->GetCommandList()->SetPipelineState(pipelineState);
		directXCore->GetCommandList()->SetGraphicsRootSignature(rootSignature);

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		directXCore->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		directXCore->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		// 0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		// SRV�q�[�v�̐ݒ�R�}���h
		directXCore->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
		// SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		directXCore->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// 0�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		// �`��R�}���h
		directXCore->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		// 1�Ԓ萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		directXCore->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		// �`��R�}���h
		directXCore->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		// 4.�`��R�}���h�����܂�

		//DirectX���t���[�������@�����܂�
		directXCore->DrawEnd();
	}

#pragma endregion
	return 0;
}