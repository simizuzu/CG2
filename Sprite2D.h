#pragma once
#include "SpriteCommon.h"

class Sprite2D
{
public:
	void Initialize(SpriteCommon* spriteCommon);

	void Update();

	void Draw();

private:
	SpriteCommon* spriteCommon = nullptr;
};

