#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

const std::wstring TitleName = L"DirectXGame";
//ウィンドウサイズ
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
	WNDCLASSEX GetWndclassex(); // ウィンドウクラスの設定
	static WinApi* GetInstance();

	void Destroy();
	bool WindowMessage();

private:

	static HWND hwnd;   // ウィンドウハンドル
	static WNDCLASSEX w; // ウィンドウクラス
	UINT msg;
	WPARAM wparam;
	LPARAM lparam;
	WINDOWSIZE WindowSize;

	// 隠し
	WinApi() = default;
	~WinApi() = default;
};
