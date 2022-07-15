#include "Mesh.h"

#pragma region パブリック

//コンストラクタ
Mesh::Mesh()
{
	directXCore = DirectXCore::GetInstance();

	//各種パイプライン生成
	CreatArryPipeline();

	////三角形バッファ
	//UINT vertexCount = triangleVertexCount * triangleMaxCount;
	//UINT indexCount = triangleIndexCount * triangleMaxCount;
	//triangleBuff = CreateBuff(vertexCount, indexCount);

	////線バッファ
	//vertexCount = lineVertexCount * lineMaxCount;
	//indexCount = lineIndexCount * lineMaxCount;
	//lineBuff = CreateBuff(vertexCount, indexCount);

	////四角形バッファ
	//vertexCount = boxVertexCount * boxMaxCount;
	//indexCount = boxIndexCount * boxMaxCount;
	//boxBuff = CreateBuff(vertexCount, indexCount);

	CreatConstBuff();

}

//ブレンドモードを設定する
void Mesh::SetBlendMode(BlendMode mode)
{
	blendMode = (UINT)mode;
}

//色コードを取得する
DX12Math::Vector4 Mesh::GetColor(int red, int blue, int green, int alpha)
{
	DX12Math::Vector4 color{};

	color.x = static_cast<float>(red) / 255.0f;
	color.y = static_cast<float>(blue) / 255.0f;
	color.z = static_cast<float>(green) / 255.0f;
	color.w = static_cast<float>(alpha) / 255.0f;

	return color;
}
#pragma endregion

#pragma region プライベート
//定数バッファ生成(2D座標変換行列)
void Mesh::CreatConstBuff()
{
	//定数バッファ生成
	directXCore->CreateConstBuff(constMapTransform, constBuffTransform);

	DX12Math::MakeOrthogonalL(
		0.0f, static_cast<float>(WinApi::GetInstance()->GetWindowSize().window_width),
		static_cast<float>(WinApi::GetInstance()->GetWindowSize().window_height), 0.0f,
		0.0f, 1.0f, constMapTransform->mat);

}