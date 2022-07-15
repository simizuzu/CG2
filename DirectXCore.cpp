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

	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

	//Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
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

	return S_OK;
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

	return result;
}

HRESULT DirectXCore::CreateDepthHeap() {
	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width;   //レンダーターゲットに合わせる
	depthResourceDesc.Height = window_height; //レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // デプスステンシル

	// 深度地用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 深度地のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 深度地1.0f（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度地フォーマット

	// リソース生成
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度地書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	if (FAILED(result)) {
		return result;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result)) {
		return result;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度地フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
	return result;
}

HRESULT DirectXCore::CreateFence() {
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	return result;
}

void DirectXCore::EnableDebugLayer() {
	//デバックレイヤーをオンに
	ID3D12Debug* debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
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
	if (FAILED(CreateDepthHeap())) 
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

	//1バックバッファ番号を取得
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	//書き込み可能に変更

	barrierDesc.Transition.pResource = backBuffers[bbIndex];//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	//2描画先変更
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += (static_cast<unsigned long long>(bbIndex)) * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3画面クリア
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 4.描画コマンドここから
	//ビューポート設定
	viewport.Width = (FLOAT)WinApi::GetInstance()->GetWindowSize().window_width;
	viewport.Height = (FLOAT)WinApi::GetInstance()->GetWindowSize().window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);

	// シザー矩形設定
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + WinApi::GetInstance()->GetWindowSize().window_width; // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + WinApi::GetInstance()->GetWindowSize().window_height; // 切り抜き座標下

	// シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRect);
}

void DirectXCore::DrawEnd() {

	//5.元に戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	ExecuteCommand();
}

void DirectXCore::ExecuteCommand() {

	//命令のクローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* commandListts[] = { commandList };
	commandQueue->ExecuteCommandLists(1, commandListts);

	//フリップ
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//コマンド実行完了を待つ
	commandQueue->Signal(fence, ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}

	//キューをクリア
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	//コマンドリストを貯める準備
	if (commandList != 0)
	{
		result = commandList->Reset(cmdAllocator, nullptr);
		assert(SUCCEEDED(result));
	}
	else
	{
		assert(SUCCEEDED(0));
	}
}

void DirectXCore::SetBackScreenColor(float red, float green, float blue, float alpha)
{
	clearColor[0] = red;
	clearColor[1] = green;
	clearColor[2] = blue;
	clearColor[3] = alpha;
}
#pragma region ゲッター
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
#pragma endregion