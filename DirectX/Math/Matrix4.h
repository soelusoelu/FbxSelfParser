#pragma once

class Vector3;
class Quaternion;

class Matrix4 {
public:
    float m[4][4];

public:
    Matrix4();

    explicit Matrix4(float inMat[4][4]);

    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);
    Matrix4& operator*=(const Matrix4& right);

    //転置行列
    void transpose();
    static Matrix4 transpose(const Matrix4& src);

    //逆行列
    void inverse();
    static Matrix4 inverse(const Matrix4& right);

    //行列の平行移動成分を取得する
    Vector3 getTranslation() const;

    //行列のX軸を取得する
    Vector3 getXAxis() const;

    //行列のY軸を取得する
    Vector3 getYAxis() const;

    //行列のZ軸を取得する
    Vector3 getZAxis() const;

    //行列からクォータニオンを取得する
    Quaternion getQuaternion() const;

    //行列のスケール成分を取得する
    Vector3 getScale() const;

    //スケール行列を作成する
    static Matrix4 createScale(float xScale, float yScale, float zScale);

    //スケール行列を作成する
    static Matrix4 createScale(const Vector3& scaleVector);

    //均一にスケール行列を作成する
    static Matrix4 createScale(float scale);

    //X軸を中心とした回転行列を作成する
    static Matrix4 createRotationX(float theta);

    //Y軸を中心とした回転行列を作成する
    static Matrix4 createRotationY(float theta);

    //Z軸を中心とした回転行列を作成する
    static Matrix4 createRotationZ(float theta);

    //クォータニオンから回転行列を作成する
    static Matrix4 createFromQuaternion(const Quaternion& q);

    //平行移動行列を作成する
    static Matrix4 createTranslation(const Vector3& trans);

    //ビュー行列を作成する
    static Matrix4 createLookAt(const Vector3& pos, const Vector3& lookAt, const Vector3& up);

    //透視投影行列を作成する
    static Matrix4 createPerspectiveFOV(int width, int height, float fov, float nearClip, float farClip);
    static Matrix4 createPerspectiveFOV(float aspectYDivX, float fov, float nearClip, float farClip);

    //z値が線形な透視投影行列を作成する
    static Matrix4 createPerspectiveLinearFOV(int width, int height, float fov, float nearClip, float farClip);
    static Matrix4 createPerspectiveLinearFOV(float aspectYDivX, float fov, float nearClip, float farClip);

    //正射影行列を作成する
    static Matrix4 createOrtho(int width, int height, float nearClip, float farClip);

private:
    //逆行列で使用する余因子を求める
    float createCofactor(const float src[4][4], int row, int column);
    //行列式を求める
    float createDeterminant(const float src[3][3]);

public:
    static const Matrix4 identity;
};
