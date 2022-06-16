#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <dxgi1_6.h>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectXCore
{
private:

	//コンストラクタ・デストラクタ
	DirectXCore() = default;
	~DirectXCore() = default;

	// DirectX 初期化処理
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;


	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// バックバッファ
	std::vector<ID3D12Resource*> backBuffers;

	// フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	//ビューポート
	D3D12_VIEWPORT viewport{};

	//シザー矩形
	D3D12_RECT scissorRect{};

	//バリアーデスク
	D3D12_RESOURCE_BARRIER barrierDesc{};

	FLOAT clearColor[4] = { 0.1f,0.25f, 0.5f,0.0f }; // 黄緑色


	//DXGIまわり初期化
	HRESULT InitializeDXGI();

	//最終的なレンダーターゲットの生成
	HRESULT CreatRtv();

	//スワップチェインの生成
	HRESULT CreateSwapChain();

	//コマンドまわり初期化
	HRESULT InitializeCommand();

	//フェンス生成
	HRESULT CreateFence();

	//デバッグレイヤーを有効にする
	void EnableDebugLayer();


public:
	static DirectXCore* GetInstance();

	// DirectX毎フレーム処理ここから
	void InitializeDirectXCore();
	void DrawStart();
	void DrawEnd();
	void ExecuteCommand();
	// DirectX毎フレーム処理ここまで

	// セッター
	//背景色変更(RGBA)
	void SetBackScreenColor(float red, float green, float blue, float alpha);

	// ゲッター
	ID3D12Device* GetDevice();
	IDXGIFactory7* GetDxgiFactory();
	IDXGISwapChain4* GetSwapChain();
	ID3D12CommandAllocator* GetCmdAllocator();
	ID3D12GraphicsCommandList* GetCommandList();
	ID3D12CommandQueue* GetCommandQueue();
	ID3D12DescriptorHeap* GetRtvHeap();
	ID3D12Fence* GetFence();
	UINT64 GetFenceVal();

#pragma region テンプレート関数
	void Constant(size_t size, ID3D12Resource*& Buff);
};
