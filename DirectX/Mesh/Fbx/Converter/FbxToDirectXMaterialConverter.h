#pragma once

#include "../../Material.h"
#include <unordered_map>
#include <vector>

class FbxMaterial;

class FbxToDirectXMaterialConverter {
public:
    FbxToDirectXMaterialConverter(
        const FbxMaterial& materialParser,
        const std::unordered_multimap<unsigned, unsigned>& connections
    );
    ~FbxToDirectXMaterialConverter();
    FbxToDirectXMaterialConverter(const FbxToDirectXMaterialConverter&) = delete;
    FbxToDirectXMaterialConverter& operator=(const FbxToDirectXMaterialConverter&) = delete;

    void convert(
        std::vector<Material>& materials
    ) const;

private:
    const FbxMaterial& mMaterialParser;
    const std::unordered_multimap<unsigned, unsigned>& mConnections;
};
