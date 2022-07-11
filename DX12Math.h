#pragma once
#include "ErrorException.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

////2D変換行列(定数バッファ)
//struct ConstBufferDataTransform
//{
//	DirectX::XMMATRIX mat;//3D変換行列
//};


namespace DX12Math
{
	//π
	constexpr float AX_PIF = 3.141592654f;
	constexpr float AX_2PIF = 6.283185307f;
	constexpr double AX_PI = 3.141592654;
	constexpr double AX_2PI = 6.283185307;

	/// <summary>
	/// Sin、Cosを両方出す
	/// </summary>
	/// <param name="sin_"> : sinの結果を代入する</param>
	/// <param name="cos_"> : cosの結果を代入する</param>
	/// <param name="angle"> : ラジアン角</param>
	void SinCos(float& sin_, float& cos_, float angle);

	/// <summary>
	/// 度数法からラジアン変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeRadians(const float& angle);

	/// <summary>
	/// ラジアンから度数法変換
	/// </summary>
	/// <param name="angle"> : 角度</param>
	float ChangeDira(const float& angle);

	std::vector<std::string> getFileNames(std::string folderPath);

	template<typename T>
	T Clamp(T Value, const T low, const T high)
	{
		if (high < Value)
		{
			Value = high;
		}
		if (Value < low)
		{
			Value = low;
		}
		return Value;
	}
}

// 座標変換行列
struct ConstBufferDataTransform
{
	DX12Math::Matrix4 mat; // 3D変換行列
};

// 座標・UV
struct PosUvColor
{
	DX12Math::Vector3 pos; // 座標
	DX12Math::Vector3 normal; // 法線ベクトル
	DX12Math::Vector2 uv; // uv座標
	DX12Math::Vector4 color; // カラー
};

typedef struct
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	//スプライト数
	UINT nextIndex;
}ModelShareVaria;