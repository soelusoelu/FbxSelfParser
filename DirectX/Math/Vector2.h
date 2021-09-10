#pragma once

class Matrix3;

class Vector2 {
public:
    float x;
    float y;

public:
    Vector2();
    explicit Vector2(float inX, float inY);

    //ベクトルのそれぞれの要素を設定する
    void set(float inX, float inY);

    Vector2 operator-() const;
    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend Vector2 operator-(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(const Vector2& vec, float scalar);
    friend Vector2 operator*(float scalar, const Vector2& vec);
    friend Vector2 operator/(const Vector2& vec, float scalar);
    Vector2& operator*=(float scalar);
    Vector2& operator+=(const Vector2& right);
    Vector2& operator-=(const Vector2& right);

    //2つのベクトルがほぼ同じ値か
    static bool equal(const Vector2& left, const Vector2& right);

    //ベクトルの長さの2乗を求める
    float lengthSq() const;
    //ベクトルの長さを求める
    float length() const;
    //2つのベクトルの距離を求める
    static float distance(const Vector2& a, const Vector2& b);

    //2つのベクトルの範囲内に収める
    void clamp(const Vector2& min, const Vector2& max);
    //2つのベクトルの範囲内に収める
    static Vector2 clamp(const Vector2& value, const Vector2& min, const Vector2& max);

    //ベクトルを正規化する
    void normalize();
    //ベクトルを正規化する
    static Vector2 normalize(const Vector2& vec);

    //2つのベクトルの内積を求める
    static float dot(const Vector2& a, const Vector2& b);
    //2つのベクトルの外積を求める
    static float cross(const Vector2& a, const Vector2& b);

    //2つのベクトルを線形補間する
    static Vector2 lerp(const Vector2& a, const Vector2& b, float f);

    //反射ベクトルを求める
    static Vector2 reflect(const Vector2& v, const Vector2& n);

    //ベクトルと行列の掛け算
    static Vector2 transform(const Vector2& vec, const Matrix3& mat, float w = 1.0f);

    static const Vector2 zero;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 left;
    static const Vector2 down;
    static const Vector2 one;
};
