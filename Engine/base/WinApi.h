#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

const std::wstring TitleName = L"DirectXGame";
//�E�B���h�E�T�C�Y
const int window_width = 1280;
const int window_height = 720;

struct WINDOWSIZE
{
	int window_width = 1280;
	int window_height = 720;
};

class WinApi
{
public:

	void CreateGameWindow(
		const wchar_t* title = TitleName.data(),
		UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t Width = window_width,
		int32_t Height = window_height);

	HWND GetHwnd();
	WINDOWSIZE GetWindowSize();
	WNDCLASSEX GetWndclassex(); // �E�B���h�E�N���X�̐ݒ�
	static WinApi* GetInstance();

	void Destroy();
	bool WindowMessage();

private:

	static HWND hwnd;   // �E�B���h�E�n���h��
	static WNDCLASSEX w; // �E�B���h�E�N���X
	UINT msg;
	WPARAM wparam;
	LPARAM lparam;
	WINDOWSIZE WindowSize;

	// �B��
	WinApi() = default;
	~WinApi() = default;
};
