#pragma once

#include "Bone.h"
#include <vector>

class BoneHelper {
    BoneHelper() = delete;
    ~BoneHelper() = delete;

public:
    //ルートボーンを探し取得する
    static const Bone* findRootBone(const std::vector<Bone>& bones);
};
