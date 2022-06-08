#include "WinApi.h"
#include <string>

HWND WinApi::hwnd = nullptr;
WNDCLASSEX WinApi::w = {};

//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄されたとき

		PostQuitMessage(0);//OSに終了を伝える
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApi::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t Width, int32_t Height)
{
	WindowSize.window_width = Width;   // 横幅
	WindowSize.window_height = Height; // 縦幅

	//ウィンドウクラス設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	// ウィンドウプロシージャを設定
	w.lpszClassName = title;				// ウィンドウクラス名（ヘッダーで名前変更可能）
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);// カーソル設定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0,0,Width,Height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, windowStyle, false);

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,// クラス名
		title,							// タイトルバーの文字
		windowStyle,					// 標準的なウィンドウスタイル（ヘッダーで変更可能）
		CW_USEDEFAULT,					// 表示X座標（OSに任せる）
		CW_USEDEFAULT,					// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,			// ウィンドウ横幅
		wrc.bottom - wrc.top,			// ウィンドウ縦幅
		nullptr,						// 親ウィンドウハンドル
		nullptr,						// メニューハンドル
		w.hInstance,					// 呼び出しアプリケーションハンドル
		nullptr);						// 追加パラメータ

	ShowWindow(hwnd, SW_SHOW);			// ウィンドウ表示
}

WinApi* WinApi::GetInstance()
{
	static WinApi WinApi_;

	return &WinApi_;
}

void WinApi::Destroy()
{
	//クラス使用しないため登録解除
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

	// アプリケーションが終わる時にmessageがWM_QUITになる
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
