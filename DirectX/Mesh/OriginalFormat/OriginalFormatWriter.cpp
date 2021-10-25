#include "OriginalFormatWriter.h"
#include "../Fbx/FbxMesh.h"
#include "../Fbx/FbxParser.h"
#include "../../System/Json/JsonValue.h"
#include "../../System/Json/JsonWriter.h"
#include "../../Utility/FileUtil.h"
#include <cassert>

OriginalFormatWriter::OriginalFormatWriter() = default;

OriginalFormatWriter::~OriginalFormatWriter() = default;

void OriginalFormatWriter::writeFbxToOriginal(const std::string& filePath, const FbxParser& fbx) const {
    assert(FileUtil::getFileExtension(filePath) == ".fbx");

    JsonObject root;
    writeMeshes(root, fbx.getMeshParser());

    //jsonに書き込む
    JsonWriter writer;
    auto path = filePath.substr(0, filePath.length() - 4) + ".tknmesh";
    writer.write(path.c_str(), root);
}

void OriginalFormatWriter::writeMeshes(JsonObject& out, const FbxMesh& mesh) const {
    auto meshValue = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    auto& meshObj = meshValue->getObject();

    //if (mesh.) {

    //}
    //out.setValue("vertexType", )

    //メッシュ数をjsonオブジェクトに登録する
    auto meshCount = mesh.getMeshCount();
    meshObj.setValue("meshCount", meshCount);

    auto meshes = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& a = meshes->a;
    a.resize(meshCount);

    //全メッシュを登録する
    for (int i = 0; i < meshCount; ++i) {
        a[i].setObject();
        writeMesh(
            a[i].getObject(),
            mesh.getVertices(i),
            mesh.getIndices(i),
            mesh.getNormals(i),
            mesh.getUVs(i),
            mesh.getUVIndices(i)
        );
    }

    meshObj.setValue("meshes", meshes);

    out.setValue("mesh", meshValue);
}

void OriginalFormatWriter::writeMesh(
    JsonObject& out,
    const std::vector<Vector3>& vertices,
    const std::vector<unsigned short>& indices,
    const std::vector<Vector3>& normals,
    const std::vector<Vector2>& uvs,
    const std::vector<unsigned short>& uvIndices
) const {
    auto indicesCount = static_cast<int>(indices.size());
    out.setValue("verticesCount", indicesCount);

    auto verticesValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto indicesValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto normalsValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto uvsValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);

    for (int i = 0; i < indicesCount; ++i) {
        const auto& v = vertices[indices[i]];
        verticesValue->pushBack(v.x);
        verticesValue->pushBack(v.y);
        verticesValue->pushBack(v.z);

        indicesValue->pushBack(i);

        const auto& n = normals[i];
        normalsValue->pushBack(n.x);
        normalsValue->pushBack(n.y);
        normalsValue->pushBack(n.z);

        const auto& uv = uvs[uvIndices[i]];
        uvsValue->pushBack(uv.x);
        uvsValue->pushBack(uv.y);
    }

    out.setValue("vertices", verticesValue);
    out.setValue("indices", indicesValue);
    out.setValue("normals", normalsValue);
    out.setValue("uvs", uvsValue);
}
