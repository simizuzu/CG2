#pragma once
#include "ErrorException.h"

#include "DirectXCore.h"
#include "WinApi.h"
#include "DX12Math.h"

#pragma comment(lib, "d3dcompiler.lib")

// 座標・カラー
struct PosColor
{
	DX12Math::Vector3 pos; // 座標
	DX12Math::Vector4 color; // カラー
};

// パイプライン・ルートシグネチャセット
struct PipelineSet
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

//バッファ関連
struct Buff
{
	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	//頂点マップ
	PosColor* vertMap;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	////インデックスバッファをマッピング
	uint16_t* indexMap;
	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
};

//ブレンドモード
enum class BlendMode
{
	BLENDMODE_NOBLEND,//ノーブレンド（デフォルト）
	BLENDMODE_ALPHA,//αブレンド
	BLENDMODE_ADD,//加算ブレンド
	BLENDMODE_SUB,//減算ブレンド
	BLENDMODE_MULA,//乗算ブレンド
	BLENDMODE_INVSRC,//反転ブレンド

	BLENDMODE_MAX//ブレンド種類数
};

class Mesh
{
public:
	// コンストラクタ
	Mesh();

	///<summary>
	///ブレンドモードを設定する
	///</summary>
	///<param name="mode">: ブレンドモード</param>
	void SetBlendMode(BlendMode mode);

	///<summary>
	///色コードを取得する
	///</summary>
	///<param name="red">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="blue">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="green">: 取得したい色の輝度値 : 初期値255 (0～255)</param>
	///<param name="alpha">: 取得したい色の透過率 : 初期値255 (0～255)</param>
	///<returns>色コード</returns>
	DX12Math::Vector4 GetColor(int red = 255, int blue = 255, int green = 255, int alpha = 255);

private:
	DirectXCore* directXCore = nullptr;

	//定数バッファのGPUリソースのポインタ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	//定数バッファのマッピング用ポインタ
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ブレンドモード
	UINT blendMode;
	char PADDING[4];

	//パイプライン作成
	std::unique_ptr < PipelineSet> CreatPipeline(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode mode);

	///<summary>
	///バッファ作成
	///</summary>
	///<param name="vertexCount">: 頂点数</param>
	///<param name="indexCount">: インデックス数</param>
	///<returns>バッファ</returns>
	std::unique_ptr <Buff> CreateBuff(UINT vertexCount, UINT indexCount);

	//各種パイプラインセット生成
	void CreatArryPipeline();

	//定数バッファ生成(2D座標変換行列)
	void CreatConstBuff();

	//コピーコンストラクタ・代入演算子削除
	Mesh& operator=(const Mesh&) = delete;
	Mesh(const Mesh&) = delete;
};

