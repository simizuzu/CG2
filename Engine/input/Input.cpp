#include "Input.h"


void Input::Initialize()//������
{

	HRESULT result;

	WinApi* app = WinApi::GetInstance();

	//DirectInput�̏�����
	result = DirectInput8Create(app->GetWndclassex().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(app->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	// �S�L�[�̓��͏�Ԃ��擾����

	for (int i = 0; i < _countof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(uint8_t index) { // ���������
	return key[index];
}

bool Input::ReleaseKey(uint8_t index) { // ���������
	return !key[index] && !oldkey[index];
}

bool Input::TriggerPushKey(uint8_t index) { // �������u��
	return key[index] && !oldkey[index];
}

bool Input::TriggerReleaseKey(uint8_t index) { // �������u��
	return !key[index] && oldkey[index];
}