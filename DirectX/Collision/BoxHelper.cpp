#include "BoxHelper.h"
#include "OBB.h"

BoxPoints BoxHelper::getPoints(const OBB& obb) {
    BoxPoints points;

    const auto& center = obb.center;
    const auto& rot = obb.rotation;
    const auto& extents = obb.extents;

    Vector3 ext = extents;
    points[BoxConstantGroup::BOX_BACK_TOP_RIGHT] = Vector3::transform(ext, rot) + center;
    ext = Vector3(extents.x, -extents.y, extents.z);
    points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT] = Vector3::transform(ext, rot) + center;
    ext = Vector3(-extents.x, extents.y, extents.z);
    points[BoxConstantGroup::BOX_BACK_TOP_LEFT] = Vector3::transform(ext, rot) + center;
    ext = Vector3(-extents.x, -extents.y, extents.z);
    points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT] = Vector3::transform(ext, rot) + center;

    ext = Vector3(extents.x, extents.y, -extents.z);
    points[BoxConstantGroup::BOX_FORE_TOP_RIGHT] = Vector3::transform(ext, rot) + center;
    ext = Vector3(extents.x, -extents.y, -extents.z);
    points[BoxConstantGroup::BOX_FORE_BOTTOM_RIGHT] = Vector3::transform(ext, rot) + center;
    ext = Vector3(-extents.x, extents.y, -extents.z);
    points[BoxConstantGroup::BOX_FORE_TOP_LEFT] = Vector3::transform(ext, rot) + center;
    ext = -extents;
    points[BoxConstantGroup::BOX_FORE_BOTTOM_LEFT] = Vector3::transform(ext, rot) + center;

    return points;
}
