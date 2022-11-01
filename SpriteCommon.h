#pragma once
#include "DirectXCore.h"

class SpriteCommon
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCore* directXcore);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	DirectXCore* directXcore = nullptr;
};