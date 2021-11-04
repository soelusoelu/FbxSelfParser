#include "BoneHelper.h"

const Bone* BoneHelper::findRootBone(const std::vector<Bone>& bones) {
    const Bone* root = nullptr;

    for (const auto& b : bones) {
        if (!b.parent) {
            root = &b;
            break;
        }
    }

    return root;
}
