#include "FBX.h"
#include "FbxObjects.h"
#include "FbxParser.h"
#include "../../Utility/FileUtil.h"
#include <fstream>
#include <sstream>

FBX::FBX() = default;

FBX::~FBX() = default;

void FBX::parse(
    const std::string& filePath,
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<Material>& materials,
    std::vector<Motion>& motions,
    std::vector<Bone>& bones
) {
    //fbxファイルを開いて内容を読み込む
    std::ifstream ifs(filePath, std::ios_base::in | std::ios_base::binary);
    if (ifs.fail()) {
        return;
    }

    //現状1つ
    meshesVertices.resize(1);
    meshesVerticesPosition.resize(1);
    meshesIndices.resize(1);
    materials.resize(1);

    //解析開始
    FbxParser parser;
    parser.parse(filePath);

    const auto& obj = parser.getObjects();
    const auto& vertices = obj.getVertices();
    meshesVerticesPosition[0] = vertices;
    meshesIndices[0] = obj.getIndices();

    auto vertSize = vertices.size();
    auto& meshVertices = meshesVertices[0];
    meshVertices.resize(vertSize);
    for (size_t i = 0; i < vertSize; ++i) {
        meshVertices[i].pos = vertices[i];
    }
}

bool FBX::isSkip(const std::string& line) const {
    //文字列が空ならスキップ
    if (line.empty()) {
        return true;
    }
    //文字列の先頭がセミコロン(コメント)ならスキップ
    if (line[0] == ';') {
        return true;
    }

    //スキップしない
    return false;
}
