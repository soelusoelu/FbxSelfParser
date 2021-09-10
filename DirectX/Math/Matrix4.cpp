#include "Matrix4.h"
#include "MathUtility.h"
#include "Quaternion.h"
#include "Vector3.h"
#include <memory>

Matrix4::Matrix4() {
    *this = Matrix4::identity;
}

Matrix4::Matrix4(float inMat[4][4]) {
    memcpy(m, inMat, 16 * sizeof(float));
}

Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
    Matrix4 retVal;
    // row 0
    retVal.m[0][0] =
        a.m[0][0] * b.m[0][0] +
        a.m[0][1] * b.m[1][0] +
        a.m[0][2] * b.m[2][0] +
        a.m[0][3] * b.m[3][0];

    retVal.m[0][1] =
        a.m[0][0] * b.m[0][1] +
        a.m[0][1] * b.m[1][1] +
        a.m[0][2] * b.m[2][1] +
        a.m[0][3] * b.m[3][1];

    retVal.m[0][2] =
        a.m[0][0] * b.m[0][2] +
        a.m[0][1] * b.m[1][2] +
        a.m[0][2] * b.m[2][2] +
        a.m[0][3] * b.m[3][2];

    retVal.m[0][3] =
        a.m[0][0] * b.m[0][3] +
        a.m[0][1] * b.m[1][3] +
        a.m[0][2] * b.m[2][3] +
        a.m[0][3] * b.m[3][3];

    // row 1
    retVal.m[1][0] =
        a.m[1][0] * b.m[0][0] +
        a.m[1][1] * b.m[1][0] +
        a.m[1][2] * b.m[2][0] +
        a.m[1][3] * b.m[3][0];

    retVal.m[1][1] =
        a.m[1][0] * b.m[0][1] +
        a.m[1][1] * b.m[1][1] +
        a.m[1][2] * b.m[2][1] +
        a.m[1][3] * b.m[3][1];

    retVal.m[1][2] =
        a.m[1][0] * b.m[0][2] +
        a.m[1][1] * b.m[1][2] +
        a.m[1][2] * b.m[2][2] +
        a.m[1][3] * b.m[3][2];

    retVal.m[1][3] =
        a.m[1][0] * b.m[0][3] +
        a.m[1][1] * b.m[1][3] +
        a.m[1][2] * b.m[2][3] +
        a.m[1][3] * b.m[3][3];

    // row 2
    retVal.m[2][0] =
        a.m[2][0] * b.m[0][0] +
        a.m[2][1] * b.m[1][0] +
        a.m[2][2] * b.m[2][0] +
        a.m[2][3] * b.m[3][0];

    retVal.m[2][1] =
        a.m[2][0] * b.m[0][1] +
        a.m[2][1] * b.m[1][1] +
        a.m[2][2] * b.m[2][1] +
        a.m[2][3] * b.m[3][1];

    retVal.m[2][2] =
        a.m[2][0] * b.m[0][2] +
        a.m[2][1] * b.m[1][2] +
        a.m[2][2] * b.m[2][2] +
        a.m[2][3] * b.m[3][2];

    retVal.m[2][3] =
        a.m[2][0] * b.m[0][3] +
        a.m[2][1] * b.m[1][3] +
        a.m[2][2] * b.m[2][3] +
        a.m[2][3] * b.m[3][3];

    // row 3
    retVal.m[3][0] =
        a.m[3][0] * b.m[0][0] +
        a.m[3][1] * b.m[1][0] +
        a.m[3][2] * b.m[2][0] +
        a.m[3][3] * b.m[3][0];

    retVal.m[3][1] =
        a.m[3][0] * b.m[0][1] +
        a.m[3][1] * b.m[1][1] +
        a.m[3][2] * b.m[2][1] +
        a.m[3][3] * b.m[3][1];

    retVal.m[3][2] =
        a.m[3][0] * b.m[0][2] +
        a.m[3][1] * b.m[1][2] +
        a.m[3][2] * b.m[2][2] +
        a.m[3][3] * b.m[3][2];

    retVal.m[3][3] =
        a.m[3][0] * b.m[0][3] +
        a.m[3][1] * b.m[1][3] +
        a.m[3][2] * b.m[2][3] +
        a.m[3][3] * b.m[3][3];

    return retVal;
}

Matrix4& Matrix4::operator*=(const Matrix4& right) {
    *this = *this * right;
    return *this;
}

void Matrix4::transpose() {
    float temp[4][4] = { 0 };
    memcpy_s(temp, sizeof(temp), m, sizeof(m));

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = temp[j][i];
        }
    }
}

Matrix4 Matrix4::transpose(const Matrix4& src) {
    auto temp = src;
    temp.transpose();
    return temp;
}

void Matrix4::inverse() {
    //転置行列
    const auto& t = transpose(*this);

    float src[16] = { 0 };
    memcpy_s(src, sizeof(src), t.m, sizeof(t.m));

    //補因子を求める
    float cofactors[12] = { 0 };
    cofactors[0] = src[10] * src[15];
    cofactors[1] = src[11] * src[14];
    cofactors[2] = src[9] * src[15];
    cofactors[3] = src[11] * src[13];
    cofactors[4] = src[9] * src[14];
    cofactors[5] = src[10] * src[13];
    cofactors[6] = src[8] * src[15];
    cofactors[7] = src[11] * src[12];
    cofactors[8] = src[8] * src[14];
    cofactors[9] = src[10] * src[12];
    cofactors[10] = src[8] * src[13];
    cofactors[11] = src[9] * src[12];

    float dst[16] = { 0 };
    dst[0] = cofactors[0] * src[5] + cofactors[3] * src[6] + cofactors[4] * src[7];
    dst[0] -= cofactors[1] * src[5] + cofactors[2] * src[6] + cofactors[5] * src[7];
    dst[1] = cofactors[1] * src[4] + cofactors[6] * src[6] + cofactors[9] * src[7];
    dst[1] -= cofactors[0] * src[4] + cofactors[7] * src[6] + cofactors[8] * src[7];
    dst[2] = cofactors[2] * src[4] + cofactors[7] * src[5] + cofactors[10] * src[7];
    dst[2] -= cofactors[3] * src[4] + cofactors[6] * src[5] + cofactors[11] * src[7];
    dst[3] = cofactors[5] * src[4] + cofactors[8] * src[5] + cofactors[11] * src[6];
    dst[3] -= cofactors[4] * src[4] + cofactors[9] * src[5] + cofactors[10] * src[6];
    dst[4] = cofactors[1] * src[1] + cofactors[2] * src[2] + cofactors[5] * src[3];
    dst[4] -= cofactors[0] * src[1] + cofactors[3] * src[2] + cofactors[4] * src[3];
    dst[5] = cofactors[0] * src[0] + cofactors[7] * src[2] + cofactors[8] * src[3];
    dst[5] -= cofactors[1] * src[0] + cofactors[6] * src[2] + cofactors[9] * src[3];
    dst[6] = cofactors[3] * src[0] + cofactors[6] * src[1] + cofactors[11] * src[3];
    dst[6] -= cofactors[2] * src[0] + cofactors[7] * src[1] + cofactors[10] * src[3];
    dst[7] = cofactors[4] * src[0] + cofactors[9] * src[1] + cofactors[10] * src[2];
    dst[7] -= cofactors[5] * src[0] + cofactors[8] * src[1] + cofactors[11] * src[2];

    cofactors[0] = src[2] * src[7];
    cofactors[1] = src[3] * src[6];
    cofactors[2] = src[1] * src[7];
    cofactors[3] = src[3] * src[5];
    cofactors[4] = src[1] * src[6];
    cofactors[5] = src[2] * src[5];
    cofactors[6] = src[0] * src[7];
    cofactors[7] = src[3] * src[4];
    cofactors[8] = src[0] * src[6];
    cofactors[9] = src[2] * src[4];
    cofactors[10] = src[0] * src[5];
    cofactors[11] = src[1] * src[4];

    dst[8] = cofactors[0] * src[13] + cofactors[3] * src[14] + cofactors[4] * src[15];
    dst[8] -= cofactors[1] * src[13] + cofactors[2] * src[14] + cofactors[5] * src[15];
    dst[9] = cofactors[1] * src[12] + cofactors[6] * src[14] + cofactors[9] * src[15];
    dst[9] -= cofactors[0] * src[12] + cofactors[7] * src[14] + cofactors[8] * src[15];
    dst[10] = cofactors[2] * src[12] + cofactors[7] * src[13] + cofactors[10] * src[15];
    dst[10] -= cofactors[3] * src[12] + cofactors[6] * src[13] + cofactors[11] * src[15];
    dst[11] = cofactors[5] * src[12] + cofactors[8] * src[13] + cofactors[11] * src[14];
    dst[11] -= cofactors[4] * src[12] + cofactors[9] * src[13] + cofactors[10] * src[14];
    dst[12] = cofactors[2] * src[10] + cofactors[5] * src[11] + cofactors[1] * src[9];
    dst[12] -= cofactors[4] * src[11] + cofactors[0] * src[9] + cofactors[3] * src[10];
    dst[13] = cofactors[8] * src[11] + cofactors[0] * src[8] + cofactors[7] * src[10];
    dst[13] -= cofactors[6] * src[10] + cofactors[9] * src[11] + cofactors[1] * src[8];
    dst[14] = cofactors[6] * src[9] + cofactors[11] * src[11] + cofactors[3] * src[8];
    dst[14] -= cofactors[10] * src[11] + cofactors[2] * src[8] + cofactors[7] * src[9];
    dst[15] = cofactors[10] * src[10] + cofactors[4] * src[8] + cofactors[9] * src[9];
    dst[15] -= cofactors[8] * src[9] + cofactors[11] * src[10] + cofactors[5] * src[8];

    // Calculate determinant
    float det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

    // Inverse of matrix is divided by determinant
    det = 1 / det;
    for (int j = 0; j < 16; j++) {
        dst[j] *= det;
    }

    // Set it back
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = dst[i * 4 + j];
        }
    }
}

Matrix4 Matrix4::inverse(const Matrix4& right) {
    auto temp = right;
    temp.inverse();
    return temp;
}

Vector3 Matrix4::getTranslation() const {
    return Vector3(m[3][0], m[3][1], m[3][2]);
}

Vector3 Matrix4::getXAxis() const {
    return Vector3::normalize(Vector3(m[0][0], m[0][1], m[0][2]));
}

Vector3 Matrix4::getYAxis() const {
    return Vector3::normalize(Vector3(m[1][0], m[1][1], m[1][2]));
}

Vector3 Matrix4::getZAxis() const {
    return Vector3::normalize(Vector3(m[2][0], m[2][1], m[2][2]));
}

Quaternion Matrix4::getQuaternion() const {
    //最大成分を検索
    float elem[4] = {
        m[0][0] - m[1][1] - m[2][2],
        -m[0][0] + m[1][1] - m[2][2],
        -m[0][0] - m[1][1] + m[2][2],
        m[0][0] + m[1][1] + m[2][2]
    }; // 0:x, 1:y, 2:z, 3:w

    unsigned biggestIndex = 0;
    for (unsigned i = 1; i < 4; ++i) {
        if (elem[i] > elem[biggestIndex]) {
            biggestIndex = i;
        }
    }

    //最大要素の値を算出
    float v = sqrtf(elem[biggestIndex] + 1.f) * 0.5f;
    float mult = 0.25f / v;

    Quaternion result;
    if (biggestIndex == 0) {
        result.x = v;
        result.y = (m[0][1] + m[1][0]) * mult;
        result.z = (m[2][0] + m[0][2]) * mult;
        result.w = (m[1][2] - m[2][1]) * mult;
    } else if (biggestIndex == 1) {
        result.x = (m[0][1] + m[1][0]) * mult;
        result.y = v;
        result.z = (m[1][2] + m[2][1]) * mult;
        result.w = (m[2][0] - m[0][2]) * mult;
    } else if (biggestIndex == 2) {
        result.x = (m[2][0] + m[0][2]) * mult;
        result.y = (m[1][2] + m[2][1]) * mult;
        result.z = v;
        result.w = (m[0][1] - m[1][0]) * mult;
    } else if (biggestIndex == 3) {
        result.x = (m[1][2] - m[2][1]) * mult;
        result.y = (m[2][0] - m[0][2]) * mult;
        result.z = (m[0][1] - m[1][0]) * mult;
        result.w = v;
    }

    return result;
}

Vector3 Matrix4::getScale() const {
    Vector3 retVal;
    retVal.x = Vector3(m[0][0], m[0][1], m[0][2]).length();
    retVal.y = Vector3(m[1][0], m[1][1], m[1][2]).length();
    retVal.z = Vector3(m[2][0], m[2][1], m[2][2]).length();
    return retVal;
}

Matrix4 Matrix4::createScale(float xScale, float yScale, float zScale) {
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, zScale, 0.f },
        { 0.f, 0.f, 0.f, 1.f }
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createScale(const Vector3& scaleVector) {
    return createScale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Matrix4 Matrix4::createScale(float scale) {
    return createScale(scale, scale, scale);
}

Matrix4 Matrix4::createRotationX(float theta) {
    float temp[4][4] = {
        { 1.f, 0.f, 0.f , 0.f },
        { 0.f, Math::cos(theta), Math::sin(theta), 0.f },
        { 0.f, -Math::sin(theta), Math::cos(theta), 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createRotationY(float theta) {
    float temp[4][4] = {
        { Math::cos(theta), 0.f, -Math::sin(theta), 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { Math::sin(theta), 0.f, Math::cos(theta), 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createRotationZ(float theta) {
    float temp[4][4] = {
        { Math::cos(theta), Math::sin(theta), 0.f, 0.f },
        { -Math::sin(theta), Math::cos(theta), 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { 0.f, 0.f, 0.f, 1.f },
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createFromQuaternion(const Quaternion& q) {
    float xx = 2.f * q.x * q.x;
    float yy = 2.f * q.y * q.y;
    float zz = 2.f * q.z * q.z;
    float xy = 2.f * q.x * q.y;
    float xz = 2.f * q.x * q.z;
    float yz = 2.f * q.y * q.z;
    float wx = 2.f * q.w * q.x;
    float wy = 2.f * q.w * q.y;
    float wz = 2.f * q.w * q.z;

    float mat[4][4] = {
        1.f - yy - zz, xy + wz, xz - wy, 0.f,
        xy - wz, 1.f - xx - zz, yz + wx, 0.f,
        xz + wy, yz - wx, 1.f - xx - yy, 0.f,
        0.f, 0.f, 0.f, 1.f
    };

    return Matrix4(mat);
}

Matrix4 Matrix4::createTranslation(const Vector3& trans) {
    float temp[4][4] = {
        { 1.f, 0.f, 0.f, 0.f },
        { 0.f, 1.f, 0.f, 0.f },
        { 0.f, 0.f, 1.f, 0.f },
        { trans.x, trans.y, trans.z, 1.f }
    };
    return Matrix4(temp);
}

Matrix4 Matrix4::createLookAt(const Vector3& pos, const Vector3& lookAt, const Vector3& up) {
    auto zaxis = Vector3::normalize(lookAt - pos);
    auto xaxis = Vector3::normalize(Vector3::cross(up, zaxis));
    auto yaxis = Vector3::normalize(Vector3::cross(zaxis, xaxis));
    Vector3 trans(
        -Vector3::dot(xaxis, pos),
        -Vector3::dot(yaxis, pos),
        -Vector3::dot(zaxis, pos)
    );

    float temp[4][4] = {
        { xaxis.x, yaxis.x, zaxis.x, 0.f },
        { xaxis.y, yaxis.y, zaxis.y, 0.f },
        { xaxis.z, yaxis.z, zaxis.z, 0.f },
        { trans.x, trans.y, trans.z, 1.f }
    };

    return Matrix4(temp);
}

Matrix4 Matrix4::createPerspectiveFOV(int width, int height, float fov, float nearClip, float farClip) {
    return createPerspectiveFOV(static_cast<float>(height) / static_cast<float>(width), fov, nearClip, farClip);
}

Matrix4 Matrix4::createPerspectiveFOV(float aspectYDivX, float fov, float nearClip, float farClip) {
    float yScale = Math::cot(fov / 2.f);
    float xScale = yScale * aspectYDivX;
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, farClip / (farClip - nearClip), 1.f },
        { 0.f, 0.f, -nearClip * farClip / (farClip - nearClip), 0.f }
    };

    return Matrix4(temp);
}

Matrix4 Matrix4::createPerspectiveLinearFOV(int width, int height, float fov, float nearClip, float farClip) {
    return createPerspectiveLinearFOV(static_cast<float>(height) / static_cast<float>(width), fov, nearClip, farClip);
}

Matrix4 Matrix4::createPerspectiveLinearFOV(float aspectYDivX, float fov, float nearClip, float farClip) {
    float yScale = Math::cot(fov / 2.f);
    float xScale = yScale * aspectYDivX;
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, 1.f / (farClip - nearClip), 1.f },
        { 0.f, 0.f, -nearClip / (farClip - nearClip), 0.f }
    };

    return Matrix4(temp);
}

Matrix4 Matrix4::createOrtho(int width, int height, float nearClip, float farClip) {
    float w = static_cast<float>(width);
    float h = static_cast<float>(height);

    float temp[4][4] = {
        { 2.f / w, 0.f, 0.f, 0.f },
        { 0.f, 2.f / h, 0.f, 0.f },
        { 0.f, 0.f, 1.f / (farClip - nearClip), 0.f },
        { 0.f, 0.f, -nearClip / (farClip - nearClip), 1.f }
    };
    return Matrix4(temp);
}

float Matrix4::createCofactor(const float src[4][4], int row, int column) {
    float small[3][3] = { 0 };

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            small[i][j] = src[i][j];
        }
        for (int m = column + 1; m < 4; ++m) {
            small[i][m - 1] = src[i][m];
        }
    }

    for (int i = row + 1; i < 4; ++i) {
        for (int j = 0; j < column; ++j) {
            small[i - 1][j] = src[i][j];
        }
        for (int m = column + 1; m < 4; ++m) {
            small[i - 1][m - 1] = src[i][m];
        }
    }

    //行列式を取得する
    float det = createDeterminant(small);

    //行+列が奇数なら
    if ((row + column) % 2 == 1) {
        det *= -1.f;
    }

    return det;
}

float Matrix4::createDeterminant(const float src[3][3]) {
    return src[0][0] * src[1][1] * src[2][2]
        + src[0][1] * src[1][2] * src[2][0]
        + src[0][2] * src[1][0] * src[2][1]
        - src[0][2] * src[1][1] * src[2][0]
        - src[0][1] * src[1][0] * src[2][2]
        - src[0][0] * src[1][2] * src[2][1];
}

float m4Ident[4][4] = {
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f }
};
const Matrix4 Matrix4::identity(m4Ident);
