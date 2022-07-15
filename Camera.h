#pragma once
#include "DX12Math.h"
#include "WinApi.h"

enum UpdateProMatFunc
{
	UpdateProMatFunc_Perspective,	// 透視射影行列
	UpdateProMatFunc_Ortho,			// 平行投影
};

class Camera
{
private:

	// ビュー行列
	DX12Math::Matrix4 viewMatrix;
	// プロジェクション行列
	DX12Math::Matrix4 proMatrix;
	// ビュープロジェクション行列
	DX12Math::Matrix4 viewProMatrix;
	// ビュー行列の逆行列
	DX12Math::Matrix4 viewMatrixInv;
	// カメラの回転行列
	DX12Math::Matrix4 cameraRot;

	// 視点座標
	DX12Math::Vector3 eye = { 0.0f,0.0f,-100.0f };
	// 注視点座標
	DX12Math::Vector3 target = { 0.0f,0.0f,0.0f };
	// 上方向ベクトル
	DX12Math::Vector3 up = { 0.0f,1.0f,0.0f };

	// 画角(ラジアン度)
	float fovAngleY = 0.0f;
	// アスペクト比(画面横幅/画面縦幅)
	float aspect = 0.0f;
	// ニアクリップ(前端)
	float near_ = 0.0f;
	// ファークリップ(奥端)
	float far_ = 0.0f;
	// 画面左側
	float left = 0.0f;
	// 画面右側
	float right = 0.0f;
	// 画面上側
	float top = 0.0f;
	// 画面下側
	float bottom = 0.0f;

	// プロジェクション行列の更新フラグ
	bool updateProMatrix = true;
	// ビュー行列の更新フラグ
	bool updateViewMatrix = true;
	char PADDING[2]{};

	// 注視点と視点の距離
	float tgtToPosLen = 0.0f;

	// カメラの正面ベクトル
	DX12Math::Vector3 forward = { 0.0f,0.0f,0.0f };

	UpdateProMatFunc matrixFunc = UpdateProMatFunc_Perspective;

public:

	// コンストラクタ・デストラクタ
	Camera() = default;
	~Camera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="matFunc">透視射影or平行投影</param>
	void Initialize(UpdateProMatFunc matFunc);

	/// <summary>
	/// ビュー行列・プロジェクション行列を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを動かす
	/// </summary>
	/// <param name="move">動かす量</param>
	void Move(const DX12Math::Vector3& move);

	/// <summary>
	/// 注視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveTarget(const DX12Math::Vector3& move);

	/// <summary>
	/// 視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MovePos(const DX12Math::Vector3& move);

#pragma region セッター

	/// <summary>
	/// アスペクト比を設定する
	/// </summary>
	/// <param name="aspect_">アスペクト比</param>
	void SetAspect(float aspect_);

	/// <summary>
	/// カメラの座標を設定する
	/// </summary>
	/// <param name="pos"> : 設定したい座標</param>
	void SetPos(const DX12Math::Vector3& pos);

	/// <summary>
	/// カメラの座標を固定する
	/// </summary>
	/// <param name="x,y,z"> : カメラの座標を設定</param>
	void SetPos(float x, float y, float z);


	/// <summary>
	/// 注視点を設定
	/// </summary>
	/// <param name="target"> : 設定したいターゲットの座標</param>
	void SetTarget(const DX12Math::Vector3& target);

	/// <summary>
	/// 注視点を設定
	/// </summary>
	/// <param name="x,y,z"> : 設定したい座標</param>
	void SetTarget(float x, float y, float z);

	/// <summary>
	/// カメラの上方向を設定
	/// </summary>
	void SetUp(const DX12Math::Vector3& upVec);

	/// <summary>
	/// カメラの上方向を設定
	/// </summary>
	void SetUp(float x, float y, float z);

	/// <summary>
	/// 遠平面までの距離を設定
	/// </summary>
	void SetFar(float fFar);

	/// <summary>
	/// 近平面までの距離を設定
	/// </summary>
	void SetNear(float fNear);

	/// <summary>
	/// 行列の計算方法を設定
	/// </summary>
	/// <param name="func"></param>
	void SetUpdateProMatrixFunc(UpdateProMatFunc func);

	/// <summary>
	/// 画角を設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Perspectiveが設定されているときに使用される。
	/// </remarks>
	/// <param name="viewAngle">画角。単位ラジアン</param>
	void SetFovAngleY(float fovAngle);

	/// <summary>
	/// 平行投影の左端を設定
	/// </summary>
	/// <param name="left_">左端</param>
	void SetLeft(float left_);

	/// <summary>
	/// 平行投影の右端を設定
	/// </summary>
	/// <param name="right_">右端</param>
	void SetRight(float right_);

	/// <summary>
	/// 平行投影の上端を設定
	/// </summary>
	/// <param name="top_">上端</param>
	void SetTop(float top_);

	/// <summary>
	/// 平行投影の下端を設定
	/// </summary>
	/// <param name="bottom_">下端</param>
	void SetBottom(float bottom_);
#pragma endregion

#pragma region ゲッター

	/// <summary>
	/// 注視点を取得
	/// </summary>
	const DX12Math::Vector3& GetTarget() const;

	/// <summary>
	/// カメラの座標を取得
	/// </summary>
	const DX12Math::Vector3& GetPos() const;

	/// <summary>
	/// カメラの上方向を取得
	/// </summary>
	const DX12Math::Vector3& GetUp() const;

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	const DX12Math::Matrix4& GetViewMatrix();

	/// <summary>
	/// ビュー行列の逆行列を取得
	/// </summary>
	const DX12Math::Matrix4& GetViewMatrixInv();

	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	const DX12Math::Matrix4& GetProjectionMatrix();

	/// <summary>
	/// ビュー×プロジェクション行列を取得
	/// </summary>
	const DX12Math::Matrix4& GetViewProjectionMatrix();

	/// <summary>
	/// カメラの回転行列を取得
	/// </summary>
	const DX12Math::Matrix4& GetCameraRotation();

	/// <summary>
	/// 遠平面までの距離を取得
	/// </summary>
	float GetFar() const;

	/// <summary>
	/// 近平面までの距離を取得
	/// </summary>
	float GetNear() const;

	/// <summary>
	/// アスペクト比を取得
	/// </summary>
	float GetAspect() const;

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角。単位ラジアン</returns>
	float GetFovAngleY() const;

	/// <summary>
	/// 注視点と視点の距離を取得
	/// </summary>
	/// <returns></returns>
	float GetTargetToPositionLength() const;

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	const DX12Math::Vector3& GetForward() const;

	/// <summary>
	/// 平行投影の下端を取得
	/// </summary>
	float GetBottom();

	/// <summary>
	/// 平行投影の上端を取得
	/// </summary>
	float GetTop();

	/// <summary>
	/// 平行投影の右端を取得
	/// </summary>
	float GetRight();

	/// <summary>
	/// 平行投影の左端を取得
	/// </summary>
	float GetLeft();

#pragma endregion
};

