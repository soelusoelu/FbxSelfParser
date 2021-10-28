#include "FbxToDirectXMaterialConverter.h"
#include "../FbxMaterial.h"

FbxToDirectXMaterialConverter::FbxToDirectXMaterialConverter(
    const FbxMaterial& materialParser,
    const std::unordered_multimap<unsigned, unsigned>& connections
)
    : mMaterialParser(materialParser)
    , mConnections(connections)
{
}

FbxToDirectXMaterialConverter::~FbxToDirectXMaterialConverter() = default;

void FbxToDirectXMaterialConverter::convert(
    std::vector<Material>& materials
) const {
    auto materialCount = mMaterialParser.getMaterialCount();
    for (size_t i = 0; i < materialCount; ++i) {
        materials[i] = mMaterialParser.getMaterial(i);
    }
}
