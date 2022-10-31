#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <array>
#include <wrl.h>
#include "WinApi.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;

class Input {
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Input() = default;
	~Input() = default;
	void Initialize();
	void Update();
	bool PushKey(uint8_t index);
	bool ReleaseKey(uint8_t index);
	bool TriggerPushKey(uint8_t index);
	bool TriggerReleaseKey(uint8_t index);

private:

	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};

