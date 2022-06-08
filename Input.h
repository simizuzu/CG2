#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <DirectXMath.h>
#include <Windows.h>
#include <dinput.h>
#include <array>
#include "WinApi.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;

class Input {
public:
	Input() = default;
	~Input() = default;
	void Initialize();
	void Update();
	bool PushKey(uint8_t index);
	bool ReleaseKey(uint8_t index);
	bool TriggerPushKey(uint8_t index);
	bool TriggerReleaseKey(uint8_t index);

private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};

