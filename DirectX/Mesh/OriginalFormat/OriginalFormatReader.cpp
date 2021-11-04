#include "OriginalFormatReader.h"
#include "OriginalFormatToDirectXConverter.h"
#include "../../System/Json/JsonInputStream.h"
#include "../../System/Json/JsonObject.h"
#include "../../System/Json/JsonReader.h"

OriginalFormatReader::OriginalFormatReader() = default;

OriginalFormatReader::~OriginalFormatReader() = default;

void OriginalFormatReader::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<int>& materialIDs,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones
) {
    JsonInputStream stream(filePath.c_str());
    JsonReader reader;
    JsonObject root;
    reader.parse(stream, root);

    OriginalFormatToDirectXConverter converter;
    converter.convert(
        meshesVertices,
        meshesVerticesPosition,
        meshesIndices,
        materialIDs,
        bones,
        motions,
        filePath,
        root
    );
}
