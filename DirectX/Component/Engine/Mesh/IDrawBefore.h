#pragma once

#include "../../../Math/Math.h"

//コールバック用メッシュ描画前インターフェース
class IDrawBefore {
public:
    virtual ~IDrawBefore() = default;
    //描画前にコールされる
    virtual void drawBefore(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const = 0;
};
