#pragma once
#include<DirectXMath.h>
#include<cmath>

////2D�ϊ��s��(�萔�o�b�t�@)
//struct ConstBufferDataTransform
//{
//	DirectX::XMMATRIX mat;//3D�ϊ��s��
//};


namespace Math
{
	//��
	constexpr float PIF = 3.141592654f;
	constexpr float PI2F = 6.283185307f;
	constexpr double PI = 3.141592654;
	constexpr double PI2 = 6.283185307;

#pragma region �x�N�g��

	//2�����x�N�g��
	class Vector2
	{
	public:
		float x;//x����
		float y;//y����

	public:
		// �R���X�g���N�^
		Vector2();//�[���x�N�g���Ƃ��Đ���
		Vector2(float x, float y);//x�����Ay�������w�肵�Ă̐���

		//�����o�֐�
		float length()const;//�m���������߂�
		Vector2& normalization();//���K������
		float dot(const Vector2& v)const;//���ς����߂�
		float cross(const Vector2& v)const;//�O�ς����߂�

		//�P�����Z�q�I�[�o�[���[�h
		Vector2 operator+()const;
		Vector2 operator-()const;

		//������Z�q�I�[�o�[���[�h
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	//2�����Z�q
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator/(const Vector2& v, float s);

	//3�����x�N�g��
	class Vector3
	{
	public:
		float x;//x����
		float y;//y����
		float z;//z����

	public:
		//�R���X�g���N�^
		Vector3();//��x�N�g��
		Vector3(float x, float y, float z);//�������w�肵�Đ���

		float length()const;//�m���������߂�
		Vector3& normalization();//���K������
		float dot(const Vector3& v)const;//���ς����߂�
		Vector3 cross(const Vector3& v)const;//�O�ς����߂�

		//�P�����Z�q�I�[�o�[���[�h
		Vector3 operator+() const;
		Vector3 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	//2�����Z�q�I�[�o�[���[�h
	//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă�
	const Vector3 operator+(const Vector3& v1, const Vector3& v2);

	const Vector3 operator-(const Vector3& v1, const Vector3& v2);
	const Vector3 operator*(const Vector3& v, float s);
	const Vector3 operator*(float s, const Vector3& v);
	const Vector3 operator/(const Vector3& v, float s);


	//3�����x�N�g��
	class Vector4
	{
	public:
		float x;//x����
		float y;//y����
		float z;//z����
		float w;//w

	public:
		//�R���X�g���N�^
		Vector4();//��x�N�g��
		Vector4(float x, float y, float z, float w);//�������w�肵�Đ���

		float length()const;//�m���������߂�
		Vector4& normalization();//���K������
		float dot(const Vector4& v)const;//���ς����߂�
		Vector4 cross(const Vector4& v)const;//�O�ς����߂�

		//�P�����Z�q�I�[�o�[���[�h
		Vector4 operator+() const;
		Vector4 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4& operator*=(float s);
		Vector4& operator/=(float s);
	};

	//2�����Z�q�I�[�o�[���[�h
	//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă�
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
		/// �R���X�g���N�^
		/// </summary>
		/// <returns>�P�ʍs��Ƃ��ď���������܂�</returns>
		Matrix4();

		Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Matrix4(const DirectX::XMMATRIX& Matrix4);

		/// <summary>
		/// �g��s����쐬
		/// </summary>
		/// <param name="scale"> : �g�嗦</param>
		void MakeScaling(const Vector3& scale);

		/// <summary>
		/// �g��s����쐬
		/// </summary>
		/// <param name="x,y,z"> : �g�嗦</param>
		void MakeScaling(float x, float y, float z);

		/// <summary>
		/// ���s�ړ��s����쐬
		/// </summary>
		/// <param name="trans"> : ���s�ړ�</param>
		void MakeTranslation(const Vector3& trans);

		/// <summary>
		/// ���s�ړ��s����쐬
		/// </summary>
		/// <param name="x,y,z"> : ���s�ړ�</param>
		void MakeTranslation(float x, float y, float z);

		/// <summary>
		/// Y������̉�]�s����쐬
		/// </summary>
		/// <param name="angle"> : ��]�p�x(�P�ʁF���W�A��)</param>
		void MakeRotationY(float angle);

		/// <summary>
		/// Z������̉�]�s����쐬
		/// </summary>
		/// <param name="angle"> : ��]�p�x(�P�ʁF���W�A��)</param>
		void MakeRotationZ(float angle);

		/// <summary>
		/// X������̉�]�s����쐬
		/// </summary>
		/// <param name="angle"> : ��]�p�x(�P�ʁF���W�A��)</param>
		void MakeRotationX(float angle);

		/// <summary>
		/// X������̉�]�s����쐬
		/// </summary>
		/// <param name="Rotat"> : ��]�p�x(�P�ʁF���W�A��)</param>
		void MakeRotation(const Vector3& Rotat);

		/// <summary>
		/// X������̉�]�s����쐬
		/// </summary>
		/// <param name="x,y,z"> : ��]�p�x(�P�ʁF���W�A��)</param>
		void MakeRotation(float x, float y, float z);

		/// <summary>
		/// ���s���e�ϊ��s��𐶐�(����n)
		/// </summary>
		/// <param name="left"> : ����</param>
		/// <param name="right"> : �E��</param>
		/// <param name="bottom"> : ����</param>
		/// <param name="top"> : �㑤</param>
		/// <param name="near_"> : �j�A�N���b�v����</param>
		/// <param name="far_"> : �t�@�[�N���b�v����</param>
		/// <param name="matrix"> : �������s��</param>
		void MakeOrthogonalL(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix);

		/// <summary>
		/// ���s���e�ϊ��s��𐶐�(�E��n)
		/// </summary>
		/// <param name="left"> : ����</param>
		/// <param name="right"> : �E��</param>
		/// <param name="bottom"> : ����</param>
		/// <param name="top"> : �㑤</param>
		/// <param name="near_"> : �j�A�N���b�v����</param>
		/// <param name="far_"> : �t�@�[�N���b�v����</param>
		/// <param name="matrix"> : �������s��</param>
		void MakeOrthogonalR(float left, float right, float bottom, float top, float near_, float far_, Matrix4& matrix);

		/// <summary>
		/// DirectX::XMMATRIX�^�ւ̈Öق̌^�ϊ��B
		/// </summary>
		operator DirectX::XMMATRIX() const;

		/// <summary>
		/// ������Z�q
		/// </summary>
		Matrix4& operator=(const Matrix4& _m);

		/// <summary>
		/// ���Z
		/// </summary>
		Matrix4& operator+=(const Matrix4& mat);

		/// <summary>
		/// ���Z
		/// </summary>
		Matrix4& operator-=(const Matrix4& mat);

		/// <summary>
		/// ��Z
		/// </summary>
		Matrix4& operator*=(const Matrix4& mat);

		/// <summary>
		/// ���Z
		/// </summary>
		Matrix4 operator+(const Matrix4& mat) const;

		/// <summary>
		/// ���Z
		/// </summary>
		Matrix4 operator-(const Matrix4& mat) const;

		/// <summary>
		/// ��Z
		/// </summary>
		Matrix4 operator*(const Matrix4& mat) const;
	};

	/// <summary>
	/// �P�ʍs��
	/// </summary
	Matrix4 MakeIdentity();
}