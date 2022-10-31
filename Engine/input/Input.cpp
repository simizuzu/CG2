#include "Input.h"


void Input::Initialize()//初期化
{

	HRESULT result;

	WinApi* app = WinApi::GetInstance();

	//DirectInputの初期化
	result = DirectInput8Create(app->GetWndclassex().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(app->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	// 全キーの入力状態を取得する

	for (int i = 0; i < _countof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}
	//キーボード情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(uint8_t index) { // 押した状態
	return key[index];
}

bool Input::ReleaseKey(uint8_t index) { // 離した状態
	return !key[index] && !oldkey[index];
}

bool Input::TriggerPushKey(uint8_t index) { // 押した瞬間
	return key[index] && !oldkey[index];
}

bool Input::TriggerReleaseKey(uint8_t index) { // 離した瞬間
	return !key[index] && oldkey[index];
}