#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <dinput.h>
#include <array>

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

	//キーボードデバイス
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};