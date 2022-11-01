#pragma once
#include "DirectXCore.h"

class SpriteCommon
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCore* directXcore);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	DirectXCore* directXcore = nullptr;
};