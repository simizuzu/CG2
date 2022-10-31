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
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	PosUvColor vertices[] =
	{//		x		y		z		u	v
		{{left, top, 0.0f},{0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},		//����C���f�b�N�X0
		{{left, bottom, 0.0f},{0.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},	//�����C���f�b�N�X1
		{{right, top, 0.0f},{1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}},		//�E��C���f�b�N�X2
		{{right, bottom, 0.0f},{1.0f,1.0f},{1.0f,1.0f,1.0f,1.0f}},	//�E���C���f�b�N�X3
	};

	// �C���f�b�N�X�f�[�^
	uint16_t indices[] =
	{
		1,0,3,	// �O�p�`1��
		2,3,0,	// �O�p�`2��
	};

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	Math::Matrix4 mTrans, mRot, mScale, matWorld;
	// ���s�ړ��s��
	mTrans.MakeTranslation(pos.x, pos.y, pos.z);
	// ��]�s��
	mRot.MakeRotation(rot.x, rot.y, rot.z);
	// �X�P�[���s��
	mScale.MakeScaling(scale.x, scale.y, scale.z);
	// ���[���h�s��
	matWorld = mScale * mRot * mTrans;


}

void Sprite::CreateVertexIndexBuffer()
{
#pragma region ���_�o�b�t�@
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(PosUvColor) * 4);

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
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(PosUvColor);
#pragma endregion
#pragma region ���_�C���f�b�N�X
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);

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
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	assert(SUCCEEDED(result));
#pragma endregion
}

void Sprite::CreateConstBuff()
{
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(Math::Matrix4) + 0xff) & ~0xff; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constBuffMap); // �}�b�s���O
	assert(SUCCEEDED(result));
}
