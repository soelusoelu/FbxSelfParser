#pragma once

class Vector2;
class Matrix4;
class Quaternion;

class Vector3 {
public:
    float x;
    float y;
    float z;

public:
    Vector3();
    explicit Vector3(float inX, float inY, float inZ);
    explicit Vector3(const Vector2& vec2, float inZ);

    //ベクトルのそれぞれの要素を設定する
    void set(float inX, float inY, float inZ);

    Vector3& operator=(const Vector3& vec);
    Vector3 operator-() const;
    friend Vector3 operator+(const Vector3& a, const Vector3& b);
    friend Vector3 operator-(const Vector3& a, const Vector3& b);
    friend Vector3 operator*(const Vector3& left, const Vector3& right);
    friend Vector3 operator*(const Vector3& vec, float scalar);
    friend Vector3 operator*(float scalar, const Vector3& vec);
    friend Vector3 operator/(const Vector3& vec, float scalar);
    Vector3& operator*=(float scalar);
    Vector3& operator*=(const Vector3& right);
    Vector3& operator/=(float scalar);
    Vector3& operator+=(const Vector3& right);
    Vector3& operator-=(const Vector3& right);

    //2つのベクトルがほぼ同じ値か
    static bool equal(const Vector3& left, const Vector3& right);

    //ベクトルの長さの2乗を求める
    float lengthSq() const;
    //ベクトルの長さを求める
    float length() const;
    //2つのベクトルの距離を求める
    static float distance(const Vector3& a, const Vector3& b);

    //2つのベクトルから最小のベクトルを求める
    static Vector3 Min(const Vector3& a, const Vector3& b);
    //2つのベクトルから最大のベクトルを求める
    static Vector3 Max(const Vector3& a, const Vector3& b);

    //2つのベクトルの範囲内に収める
    void clamp(const Vector3& min, const Vector3& max);
    //2つのベクトルの範囲内に収める
    static Vector3 clamp(const Vector3& value, const Vector3& min, const Vector3& max);

    //ベクトルを正規化する
    void normalize();
    //ベクトルを正規化する
    static Vector3 normalize(const Vector3& vec);

    //2つのベクトルの内積を求める
    static float dot(const Vector3& a, const Vector3& b);
    //2つのベクトルの外積を求める
    static Vector3 cross(const Vector3& a, const Vector3& b);

    //2つのベクトルを線形補間する
    static Vector3 lerp(const Vector3& a, const Vector3& b, float f);

    //反射ベクトルを求める
    static Vector3 reflect(const Vector3& v, const Vector3& n);

    //ベクトルと行列の掛け算
    static Vector3 transform(const Vector3& vec, const Matrix4& mat, float w = 1.f);
    static Vector3 transformWithPerspDiv(const Vector3& vec, const Matrix4& mat, float w = 1.f);
    //ベクトルとクォータニオンの掛け算
    static Vector3 transform(const Vector3& v, const Quaternion& q);

    static const Vector3 zero;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 down;
    static const Vector3 back;
    static const Vector3 one;
    static const Vector3 negOne;
    static const Vector3 infinity;
    static const Vector3 negInfinity;
};
