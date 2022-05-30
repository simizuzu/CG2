#include <cassert>

#include "DirectXCore.h"
#include "WinApi.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace std;

HRESULT DirectXCore::InitializeDXGI() {
	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	//DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//アダプターの列挙用
	std::vector < IDXGIAdapter4*>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmpAdapter = nullptr;

	//パフォーマンスが高いものから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		//動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;

	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//アダプターの情報を収録する
		adapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	return result;
}

HRESULT DirectXCore::CreatRtv() {
	//デスクリプタヒープの設定
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	//スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		//スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒ－プのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}

	return S_OK;
}

HRESULT DirectXCore::CreateSwapChain() {
	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//生成
	if (commandQueue != 0)
	{
		result = dxgiFactory->CreateSwapChainForHwnd(commandQueue, WinApi::GetInstance()->GetHwnd(), &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)&swapChain);
		if (FAILED(result))
		{
			return result;
		}
	}
	else
	{
		assert(SUCCEEDED(0));
	}
}

DirectXCore* DirectXCore::GetInstance() {
	static DirectXCore DirectXCore_;
	return &DirectXCore_;
}

HRESULT DirectXCore::InitializeCommand() {

	//コマンドアロケータの生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{}; //構造体の中身を0でクリアしている
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

HRESULT DirectXCore::CreateFence() {
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCore::EnableDebugLayer() {
	//デバックレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
}

void DirectXCore::EnableDebugLayer()
{
	ID3D12Debug* debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
} 

void DirectXCore::InitializeDirectXCore() {
#ifdef _DEBUG
	EnableDebugLayer();
#endif

	//DirectX12関連初期化
	if (FAILED(InitializeDXGI()))
	{
		assert(0);
		return;
	}
	if (FAILED(InitializeCommand()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateSwapChain()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreatRtv()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateFence()))
	{
		assert(0);
		return;
	}
}

void DirectXCore::DrawStart() {

}

void DirectXCore::DrawEnd() {

}

void DirectXCore::ExecuteCommand() {

}

ID3D12Device* DirectXCore::GetDevice() {
	return device;
}

IDXGIFactory7* DirectXCore::GetDxgiFactory() {
	return dxgiFactory;
}

IDXGISwapChain4* DirectXCore::GetSwapChain() {
	return swapChain;
}

ID3D12CommandAllocator* DirectXCore::GetCmdAllocator() {
	return cmdAllocator;
}

ID3D12GraphicsCommandList* DirectXCore::GetCommandList() {
	return commandList;
}

ID3D12CommandQueue* DirectXCore::GetCommandQueue() {
	return commandQueue;
}

ID3D12DescriptorHeap* DirectXCore::GetRtvHeap() {
	return rtvHeap;
}

ID3D12Fence* DirectXCore::GetFence() {
	return fence;
}

UINT64 DirectXCore::GetFenceVal() {
	return fenceVal;
}