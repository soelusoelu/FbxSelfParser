#include "OriginalFormatWriter.h"
#include "../Fbx/FbxMaterial.h"
#include "../Fbx/FbxMesh.h"
#include "../Fbx/FbxParser.h"
#include "../../System/AssetsManager.h"
#include "../../System/Json/JsonValue.h"
#include "../../System/Json/JsonWriter.h"
#include "../../Utility/FileUtil.h"
#include <cassert>

OriginalFormatWriter::OriginalFormatWriter() = default;

OriginalFormatWriter::~OriginalFormatWriter() = default;

void OriginalFormatWriter::writeFbxToOriginal(const std::string& filePath, const FbxParser& fbx) const {
    assert(FileUtil::getFileExtension(filePath) == ".fbx");

    JsonObject meshRoot;
    writeMeshes(meshRoot, fbx.getMeshParser());

    JsonObject matRoot;
    writeMaterials(matRoot, fbx.getMaterialParser());

    //jsonに書き込む
    JsonWriter writer;
    auto path = filePath.substr(0, filePath.length() - 4);
    writer.write((path + ".tknmesh").c_str(), meshRoot);

    writer.write((path + ".tknmat").c_str(), matRoot);
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

void OriginalFormatWriter::writeMaterials(JsonObject& out, const FbxMaterial& material) const {
    auto matValue = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    auto& matObj = matValue->getObject();

    //マテリアル数をjsonオブジェクトに登録する
    auto materialCount = material.getMaterialCount();
    out.setValue("materialCount", static_cast<int>(materialCount));

    auto materials = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& a = materials->a;
    a.resize(materialCount);

    //全マテリアルを登録する
    for (size_t i = 0; i < materialCount; ++i) {
        a[i].setObject();
        writeMaterial(a[i].getObject(), material.getMaterial(i));
    }

    out.setValue("materials", materials);
}

void OriginalFormatWriter::writeMaterial(JsonObject& out, const Material& material) const {
    const auto& ambient = material.ambient;
    auto ambientValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    ambientValue->pushBack(ambient.x);
    ambientValue->pushBack(ambient.y);
    ambientValue->pushBack(ambient.z);
    out.setValue("ambient", ambientValue);

    const auto& diffuse = material.diffuse;
    auto diffuseValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    diffuseValue->pushBack(diffuse.x);
    diffuseValue->pushBack(diffuse.y);
    diffuseValue->pushBack(diffuse.z);
    out.setValue("diffuse", diffuseValue);

    const auto& specular = material.specular;
    auto specularValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    specularValue->pushBack(specular.x);
    specularValue->pushBack(specular.y);
    specularValue->pushBack(specular.z);
    out.setValue("specular", specularValue);

    const auto& emissive = material.emissive;
    auto emissiveValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    emissiveValue->pushBack(emissive.x);
    emissiveValue->pushBack(emissive.y);
    emissiveValue->pushBack(emissive.z);
    out.setValue("emissive", emissiveValue);

    const auto& bump = material.bump;
    auto bumpValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    bumpValue->pushBack(bump.x);
    bumpValue->pushBack(bump.y);
    bumpValue->pushBack(bump.z);
    out.setValue("bump", bumpValue);

    out.setValue("transparency", material.transparency);
    out.setValue("shininess", material.shininess);

    auto texID = material.textureID;
    if (texID != Material::INVALID_ID) {
        const auto& filePath = AssetsManager::instance().getTextureFilePathFromID(texID);
        auto filename = FileUtil::getFileNameFromDirectry(filePath);
        out.setValue("texture", filename);
    }
}
