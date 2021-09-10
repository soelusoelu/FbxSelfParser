#pragma once

#include "Vector3.h"

class Quaternion {
public:
    float x;
    float y;
    float z;
    float w;

public:
    Quaternion();

    explicit Quaternion(float inX, float inY, float inZ, float inW);
    explicit Quaternion(const Vector3& axis, float angle);
    explicit Quaternion(const Vector3& euler);

    void set(float inX, float inY, float inZ, float inW);

    friend Quaternion operator*(const Quaternion& q, const Quaternion& p);
    Quaternion& operator*=(const Quaternion& right);

    void conjugate();

    float lengthSq() const;

    float length() const;

    void normalize();

    //オイラー角をもとにクォータニオンを設定する
    void setEuler(const Vector3& euler);

    //クォータニオンからオイラー角を取得する
    Vector3 euler() const;

    //正規化
    static Quaternion normalize(const Quaternion& q);

    //線形補間
    static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f);

    //内積
    static float dot(const Quaternion& a, const Quaternion& b);

    //球面線形補間
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f);

    //指定された forward と upwards 方向に回転したクォータニオンを取得する
    static Quaternion lookRotation(const Vector3& forward, const Vector3& upwards = Vector3::up);

    static const Quaternion identity;
};
