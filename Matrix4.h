#pragma once
#include "ErrorException.h"
#include "Vector3.h"

namespace DX12Math
{
	class Matrix4
	{
	public:
		float m[4][4];

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <returns>単位行列として初期化されます</returns>
		Matrix4();

		Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4(const DirectX::XMMATRIX& Matrix4);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="scale"> : 拡大率</param>
		void MakeScaling(const Vector3& scale);

		/// <summary>
		/// 拡大行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 拡大率</param>
		void MakeScaling(float x, float y, float z);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="trans"> : 平行移動</param>
		void MakeTranslation(const Vector3& trans);

		/// <summary>
		/// 平行移動行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 平行移動</param>
		void MakeTranslation(float x, float y, float z);

		/// <summary>
		/// Y軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationY(float angle);

		/// <summary>
		/// Z軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationZ(float angle);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="angle"> : 回転角度(単位：ラジアン)</param>
		void MakeRotationX(float angle);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="Rotat"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(const Vector3& Rotat);

		/// <summary>
		/// X軸周りの回転行列を作成
		/// </summary>
		/// <param name="x,y,z"> : 回転角度(単位：ラジアン)</param>
		void MakeRotation(float x, float y, float z);

		/// <summary>
		/// DirectX::XMMATRIX型への暗黙の型変換。
		/// </summary>
		operator DirectX::XMMATRIX() const;

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4& operator=(const Matrix4& _m);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4& operator+=(const Matrix4& mat);

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4& operator-=(const Matrix4& mat);

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4& operator*=(const Matrix4& mat);

		/// <summary>
		/// 加算
		/// </summary>
		Matrix4 operator+(const Matrix4& mat) const;

		/// <summary>
		/// 減算
		/// </summary>
		Matrix4 operator-(const Matrix4& mat) const;

		/// <summary>
		/// 乗算
		/// </summary>
		Matrix4 operator*(const Matrix4& mat) const;
	};

	/// <summary>
	/// 単位行列
	/// </summary>
	Matrix4 MakeIdentity();

	/// <summary>
	/// 逆行列を求める
	/// </summary>
	/// <param name="mat">逆行列にしたい行列</param>
	/// <returns>逆行列or単位行列</returns>
	Matrix4 MakeInverse(Matrix4& mat);

	/// <summary>
	/// 平行投影変換行列を生成(左手系)
	/// </summary>
	/// <param name="left"> : 左側</param>
	/// <param name="right"> : 右側</param>
	/// <param name="bottom"> : 下側</param>
	/// <param name="top"> : 上側</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix);

	/// <summary>
	/// 平行投影変換行列を生成(右手系)
	/// </summary>
	/// <param name="left"> : 左側</param>
	/// <param name="right"> : 右側</param>
	/// <param name="bottom"> : 下側</param>
	/// <param name="top"> : 上側</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix);

	/// <summary>
	/// 透視投影変換行列を生成(左手系)
	/// </summary>
	/// <param name="fovAngleY"> : 上下画角</param>
	/// <param name="aspect"> : アスペクト比(画面横幅/画面縦幅)</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakePerspectiveL(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix);

	/// <summary>
	/// 透視投影変換行列を生成(右手系)
	/// </summary>
	/// <param name="fovAngleY"> : 上下画角</param>
	/// <param name="aspect"> : アスペクト比(画面横幅/画面縦幅)</param>
	/// <param name="near_"> : ニアクリップ距離</param>
	/// <param name="far_"> : ファークリップ距離</param>
	/// <param name="matrix"> : 代入する行列</param>
	void MakePerspectiveR(float fovAngleY, float aspect, float near_, float far_, Matrix4& matrix);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	/// <param name="x">掛けられるベクトルのX座標</param>
	/// <param name="y">掛けられるベクトルのY座標</param>
	/// <param name="z">掛けられるベクトルのZ座標</param>
	/// <param name="mat">掛ける行列</param>
	void CoordinateTransformation3D(float& x, float& y, float& z, Matrix4& mat);

	/// <summary>
	/// ビュー変換行列を生成
	/// </summary>
	/// <param name="eye"></param>
	/// <param name="target"></param>
	/// <param name="up"></param>
	void MakeLookL(Vector3& eye, Vector3& target, Vector3& up, Matrix4& mat);
};