#pragma once
#include "DirectXCore.h"

class SpriteCommon
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(DirectXCore* directXcore);

	/// <summary>
	/// XV
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

private:
	DirectXCore* directXcore = nullptr;
};