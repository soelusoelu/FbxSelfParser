#pragma once

#include "../System/SystemInclude.h"

enum class Usage {
    DEFAULT, //GPUによる読み書き
    IMMUTABLE, //GPUの読み込みのみ可能
    DYNAMIC, //GPUの読み込みとCPUの書き込みが可能
    STAGING //GPUからCPUへのデータ転送をサポート
};

D3D11_USAGE toUsage(Usage usage);
