#pragma once
#include<DirectXMath.h>
#include<cmath>

////2D変換行列(定数バッファ)
//struct ConstBufferDataTransform
//{
//	DirectX::XMMATRIX mat;//3D変換行列
//};


namespace Math
{
	//π
	constexpr float PIF = 3.141592654f;
	constexpr float PI2F = 6.283185307f;
	constexpr double PI = 3.141592654;
	constexpr double PI2 = 6.283185307;

#pragma region ベクトル

	//2次元ベクトル
	class Vector2
	{
	public:
		float x;//x成分
		float y;//y成分

	public:
		// コンストラクタ
		Vector2();//ゼロベクトルとして生成
		Vector2(float x, float y);//x成分、y成分を指定しての生成

		//メンバ関数
		float length()const;//ノルムを求める
		Vector2& normalization();//正規化する
		float dot(const Vector2& v)const;//内積を求める
		float cross(const Vector2& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector2 operator+()const;
		Vector2 operator-()const;

		//代入演算子オーバーロード
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	//2項演算子
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);

	//3次元ベクトル
	class Vector3
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分

	public:
		//コンストラクタ
		Vector3();//零ベクトル
		Vector3(float x, float y, float z);//成分を指定して生成

		float length()const;//ノルムを求める
		Vector3& normalization();//正規化する
		float dot(const Vector3& v)const;//内積を求める
		Vector3 cross(const Vector3& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector3 operator+() const;
		Vector3 operator-() const;

		// 代入演算子オーバーロード
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector3 operator+(const Vector3& v1, const Vector3& v2);

	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);


	//3次元ベクトル
	class Vector4
	{
	public:
		float x;//x成分
		float y;//y成分
		float z;//z成分
		float w;//w

	public:
		//コンストラクタ
		Vector4();//零ベクトル
		Vector4(float x, float y, float z, float w);//成分を指定して生成

		float length()const;//ノルムを求める
		Vector4& normalization();//正規化する
		float dot(const Vector4& v)const;//内積を求める
		Vector4 cross(const Vector4& v)const;//外積を求める

		//単項演算子オーバーロード
		Vector4 operator+() const;
		Vector4 operator-() const;

		// 代入演算子オーバーロード
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};

	//2項演算子オーバーロード
	//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備してい
	const Vector4 operator+(const Vector4& v1, const Vector4& v2);

	const Vector4 operator-(const Vector4& v1, const Vector4& v2);
	const Vector4 operator*(const Vector4& v, float s);
	const Vector4 operator*(float s, const Vector4& v);
	const Vector4 operator/(const Vector4& v, float s);
#pragma endregion

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
	/// </summary
	Matrix4 MakeIdentity();
}