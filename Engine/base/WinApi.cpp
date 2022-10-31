#include "WinApi.h"
#include <string>

HWND WinApi::hwnd = nullptr;
WNDCLASSEX WinApi::w = {};

//�E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ�Ƃ�

		PostQuitMessage(0);//OS�ɏI����`����
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApi::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t Width, int32_t Height)
{
	WindowSize.window_width = Width;   // ����
	WindowSize.window_height = Height; // �c��

	//�E�B���h�E�N���X�ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	// �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = title;				// �E�B���h�E�N���X���i�w�b�_�[�Ŗ��O�ύX�\�j
	w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);// �J�[�\���ݒ�

	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0,0,Width,Height };
	// �����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, windowStyle, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,// �N���X��
		title,							// �^�C�g���o�[�̕���
		windowStyle,					// �W���I�ȃE�B���h�E�X�^�C���i�w�b�_�[�ŕύX�\�j
		CW_USEDEFAULT,					// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,					// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,			// �E�B���h�E����
		wrc.bottom - wrc.top,			// �E�B���h�E�c��
		nullptr,						// �e�E�B���h�E�n���h��
		nullptr,						// ���j���[�n���h��
		w.hInstance,					// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);						// �ǉ��p�����[�^

	ShowWindow(hwnd, SW_SHOW);			// �E�B���h�E�\��
}

WinApi* WinApi::GetInstance()
{
	static WinApi WinApi_;

	return &WinApi_;
}

void WinApi::Destroy()
{
	//�N���X�g�p���Ȃ����ߓo�^����
	UnregisterClass(GetWndclassex().lpszClassName, GetWndclassex().hInstance);
}

HWND WinApi::GetHwnd()
{
	return hwnd;
}

WNDCLASSEX WinApi::GetWndclassex()
{
	return w;
}

bool WinApi::WindowMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	// �A�v���P�[�V�������I��鎞��message��WM_QUIT�ɂȂ�
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

WINDOWSIZE WinApi::GetWindowSize()
{
	return WindowSize;
}
