#include "OBBCreater.h"
#include "OBB.h"

OBB OBBCreater::create(const std::vector<Vector3>& positions) {
    auto collectMat = collectMatrix(positions);
    auto eigenvector = computeEigenvector(collectMat);

    const auto& eigenMat = eigenvector.m;
    Vector3 vec1(
        eigenMat[0][0],
        eigenMat[1][0],
        eigenMat[2][0]
    );

    Vector3 vec2(
        eigenMat[0][1],
        eigenMat[1][1],
        eigenMat[2][1]
    );

    Vector3 vec3(
        eigenMat[0][2],
        eigenMat[1][2],
        eigenMat[2][2]
    );

    //全頂点に対して内積を取り、最小値・最大値を計算する
    float min1 = FLT_MAX;
    float min2 = FLT_MAX;
    float min3 = FLT_MAX;
    float max1 = FLT_MIN;
    float max2 = FLT_MIN;
    float max3 = FLT_MIN;

    for (const auto& p : positions) {
        float dot1 = Vector3::dot(vec1, p);
        if (dot1 > max1) {
            max1 = dot1;
        }
        if (dot1 < min1) {
            min1 = dot1;
        }

        float dot2 = Vector3::dot(vec2, p);
        if (dot2 > max2) {
            max2 = dot2;
        }
        if (dot2 < min2) {
            min2 = dot2;
        }

        float dot3 = Vector3::dot(vec3, p);
        if (dot3 > max3) {
            max3 = dot3;
        }
        if (dot3 < min3) {
            min3 = dot3;
        }
    }

    OBB result{};
    Vector3 center1 = vec1 * (max1 + min1);
    Vector3 center2 = vec2 * (max2 + min2);
    Vector3 center3 = vec3 * (max3 + min3);
    result.center = (center1 + center2 + center3) / 2.f;

    auto right = vec1 * (max1 - min1) / 2.f;
    auto top = vec2 * (max2 - min2) / 2.f;
    auto forward = vec3 * (max3 - min3) / 2.f;
    result.rotation = Quaternion::lookRotation(Vector3::normalize(forward), Vector3::normalize(top));

    result.extents = Vector3(right.length(), top.length(), forward.length());

    return result;
}

Matrix3 OBBCreater::collectMatrix(const std::vector<Vector3>& positions) {
    //各成分の平均を求める
    auto avg = Vector3::zero;

    for (const auto& p : positions) {
        avg += p;
    }

    auto meshVerticesSize = positions.size();
    avg /= meshVerticesSize;

    float m00 = 0.f;
    float m11 = 0.f;
    float m22 = 0.f;
    float m01and10 = 0.f;
    float m02and20 = 0.f;
    float m12and21 = 0.f;
    for (const auto& p : positions) {
        float x = p.x - avg.x;
        float y = p.y - avg.y;
        float z = p.z - avg.z;

        m00 += x * x;
        m11 += y * y;
        m22 += z * z;
        m01and10 += x * y;
        m02and20 += x * z;
        m12and21 += y * z;
    }

    m00 /= meshVerticesSize;
    m11 /= meshVerticesSize;
    m22 /= meshVerticesSize;
    m01and10 /= meshVerticesSize;
    m02and20 /= meshVerticesSize;
    m12and21 /= meshVerticesSize;

    float m[3][3] = {
        { m00, m01and10, m02and20 },
        { m01and10, m11, m12and21 },
        { m02and20, m12and21, m22 }
    };

    return Matrix3(m);
}

Matrix3 OBBCreater::computeEigenvector(const Matrix3& mat) {
    Matrix3 matCopy{};
    memcpy_s(&matCopy, sizeof(matCopy), &mat, sizeof(mat));
    Matrix3 eigenvector{};
    while (true) {
        float max = 0.f;
        unsigned row = 0;
        unsigned column = 0;
        getMaxValueForMatrix(matCopy, max, row, column);
        if (max <= 0.05f) {
            break;
        }

        auto& m = matCopy.m;
        float rowRow = m[row][row];
        float rowColumn = m[row][column];
        float columnColumn = m[column][column];

        float alpha = (rowRow - columnColumn) / 2.f;
        float beta = -rowColumn;
        float gamma = Math::abs(alpha) / Math::sqrt(alpha * alpha + beta * beta);

        float sin = Math::sqrt((1.f - gamma) / 2.f);
        float cos = Math::sqrt((1.f + gamma) / 2.f);

        if (alpha * beta < 0.f) {
            sin *= -1.f;
        }

        for (int i = 0; i < 3; ++i) {
            float temp = cos * m[row][i] - sin * m[column][i];
            m[column][i] = sin * m[row][i] + cos * m[column][i];
            m[row][i] = temp;
        }

        for (int i = 0; i < 3; ++i) {
            m[i][row] = m[row][i];
            m[i][column] = m[column][i];
        }

        m[row][row] = cos * cos * rowRow + sin * sin * columnColumn - 2.f * sin * cos * rowColumn;
        m[row][column] = sin * cos * (rowRow - columnColumn) + (cos * cos - sin * sin) * rowColumn;
        m[column][row] = sin * cos * (rowRow - columnColumn) + (cos * cos - sin * sin) * rowColumn;
        m[column][column] = sin * sin * rowRow + cos * cos * columnColumn + 2.f * sin * cos * rowColumn;

        auto& eigenMat = eigenvector.m;
        for (int i = 0; i < 3; ++i) {
            float temp = cos * eigenMat[i][row] - sin * eigenMat[i][column];
            eigenMat[i][column] = sin * eigenMat[i][row] + cos * eigenMat[i][column];
            eigenMat[i][row] = temp;
        }
    }

    return eigenvector;
}

void OBBCreater::getMaxValueForMatrix(const Matrix3& mat, float& outMax, unsigned& outRow, unsigned& outColumn) {
    outRow = 0;
    outColumn = 1;
    float max = FLT_MIN;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            //対角線分は評価しない
            if (i == j) {
                continue;
            }

            float absMax = Math::abs(mat.m[i][j]);
            if (absMax > max) {
                max = absMax;
                outRow = i;
                outColumn = j;
            }
        }
    }

    if (outRow > outColumn) {
        std::swap(outRow, outColumn);
    }

    outMax = max;
}
