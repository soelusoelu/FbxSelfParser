#include "OriginalFormatWriter.h"
#include "../BoneHelper.h"
#include "../VertexType.h"
#include "../Fbx/FbxBone.h"
#include "../Fbx/FbxMaterial.h"
#include "../Fbx/FbxMesh.h"
#include "../Fbx/FbxParser.h"
#include "../Fbx/FbxWeight.h"
#include "../../System/AssetsManager.h"
#include "../../System/Json/JsonValue.h"
#include "../../System/Json/JsonWriter.h"
#include "../../Utility/FileUtil.h"
#include <cassert>
#include <unordered_set>

OriginalFormatWriter::OriginalFormatWriter() = default;

OriginalFormatWriter::~OriginalFormatWriter() = default;

void OriginalFormatWriter::writeFbxToOriginal(
    const std::string& filePath,
    const std::vector<MeshVertices>& meshesVertices,
    const std::vector<Indices>& meshesIndices,
    const std::vector<int>& materialIDs,
    const std::vector<Bone>& bones,
    const std::vector<Motion>& motions
) const {
    assert(FileUtil::getFileExtension(filePath) == ".fbx");

    JsonObject meshRoot;
    bool hasBone = (bones.size() > 0);
    writeMeshes(meshRoot, meshesVertices, meshesIndices, materialIDs, hasBone);
    if (hasBone) {
        writeBones(meshRoot, bones);
    }
    if (motions.size() > 0) {
        writeAnimations(meshRoot, motions);
    }

    writeMaterials(filePath, materialIDs);

    //jsonに書き込む
    JsonWriter writer;
    auto path = filePath.substr(0, filePath.length() - 4);
    writer.write((path + ".tknmesh").c_str(), meshRoot);
}

void OriginalFormatWriter::writeMeshes(
    JsonObject& out,
    const std::vector<MeshVertices>& meshesVertices,
    const std::vector<Indices>& meshesIndices,
    const std::vector<int>& materialIDs,
    bool hasBone
) const {
    auto meshValue = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    auto& meshObj = meshValue->getObject();

    //頂点タイプを登録する
    auto vertexType = VertexType::POS_NORM_UV;
    if (hasBone) {
        vertexType = VertexType::POS_NORM_UV_WEIGHT;
    }
    meshObj.setValue("vertexType", static_cast<int>(vertexType));

    //メッシュ数をjsonオブジェクトに登録する
    auto meshCount = static_cast<int>(meshesVertices.size());
    meshObj.setValue("meshCount", meshCount);

    auto meshes = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& a = meshes->a;
    a.resize(meshCount);

    //全メッシュを登録する
    for (int i = 0; i < meshCount; ++i) {
        auto& outObj = a[i].setObject();

        writeMesh(outObj, meshesVertices[i], meshesIndices[i], hasBone);

        //メッシュに対応するマテリアル名を登録する
        std::string materialName = "default";
        const auto& tmpMat = AssetsManager::instance().getMaterialFromID(materialIDs[i]);
        const auto& tmpMatName = tmpMat.name;
        if (tmpMatName.length() > 0) {
            materialName = tmpMatName;
        }
        outObj.setValue("material", materialName);
    }

    meshObj.setValue("meshes", meshes);

    out.setValue("mesh", meshValue);
}

void OriginalFormatWriter::writeMesh(
    JsonObject& out,
    const MeshVertices& meshVertices,
    const Indices& meshIndices,
    bool hasBone
) const {
    //頂点数を登録する
    auto verticesCount = static_cast<int>(meshVertices.size());
    out.setValue("verticesCount", verticesCount);

    //登録する各種配列
    auto verticesValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto indicesValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto normalsValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto uvsValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);

    for (int i = 0; i < verticesCount; ++i) {
        const auto& v = meshVertices[i];

        const auto& p = v.pos;
        verticesValue->pushBack(p.x);
        verticesValue->pushBack(p.y);
        verticesValue->pushBack(p.z);

        indicesValue->pushBack(static_cast<int>(meshIndices[i]));

        const auto& n = v.normal;
        normalsValue->pushBack(n.x);
        normalsValue->pushBack(n.y);
        normalsValue->pushBack(n.z);

        const auto& uv = v.uv;
        uvsValue->pushBack(uv.x);
        uvsValue->pushBack(uv.y);
    }

    out.setValue("vertices", verticesValue);
    out.setValue("indices", indicesValue);
    out.setValue("normals", normalsValue);
    out.setValue("uvs", uvsValue);

    //ボーンが有るならウェイトを追加登録
    if (hasBone) {
        writeWeights(out, meshVertices);
    }
}

void OriginalFormatWriter::writeWeights(
    JsonObject& out,
    const MeshVertices& meshVertices
) const {
    auto weightsValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto weightsIndexValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);

    for (const auto& v : meshVertices) {
        const auto& w = v.weight;
        weightsValue->pushBack(w[0]);
        weightsValue->pushBack(w[1]);
        weightsValue->pushBack(w[2]);
        weightsValue->pushBack(w[3]);

        const auto& idx = v.index;
        weightsIndexValue->pushBack(idx[0]);
        weightsIndexValue->pushBack(idx[1]);
        weightsIndexValue->pushBack(idx[2]);
        weightsIndexValue->pushBack(idx[3]);
    }

    out.setValue("weights", weightsValue);
    out.setValue("weightIndexes", weightsIndexValue);
}

void OriginalFormatWriter::writeBones(
    JsonObject& out,
    const std::vector<Bone>& bones
) const {
    auto boneValue = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    auto& boneObj = boneValue->getObject();

    //ボーン数をjsonオブジェクトに登録する
    auto boneCount = static_cast<int>(bones.size());
    boneObj.setValue("boneCount", boneCount);

    auto outBones = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& a = outBones->a;
    a.reserve(boneCount);

    //ルートボーンから全ボーンを再帰的に登録する
    const auto root = BoneHelper::findRootBone(bones);
    writeBone(a, *root);

    boneObj.setValue("bones", outBones);

    out.setValue("bone", boneValue);
}

void OriginalFormatWriter::writeBone(
    std::vector<JsonValue>& out,
    const Bone& bone
) const {
    auto& outObj = out.emplace_back().setObject();

    outObj.setValue("name", bone.name);
    outObj.setValue("index", bone.number);
    int parentID = -1;
    if (bone.parent) {
        parentID = bone.parent->number;
    }
    outObj.setValue("parent", parentID);

    auto initMatrixValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    const auto& initMat = bone.initMat.m;
    for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
        for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
            initMatrixValue->pushBack(initMat[y][x]);
        }
    }
    outObj.setValue("initMatrix", initMatrixValue);

    auto offsetMatrixValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    const auto& offsetMat = bone.offsetMat.m;
    for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
        for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
            offsetMatrixValue->pushBack(offsetMat[y][x]);
        }
    }
    outObj.setValue("offsetMatrix", offsetMatrixValue);

    //ボーンの子を再帰的に呼び出す
    for (const auto& child : bone.children) {
        writeBone(out, *child);
    }
}

void OriginalFormatWriter::writeAnimations(
    JsonObject& out,
    const std::vector<Motion>& motions
) const {
    auto animationValue = std::make_shared<JsonValue>(JsonValueFlag::OBJECT);
    auto& animationObj = animationValue->getObject();

    //アニメーション数をjsonオブジェクトに登録する
    auto animationCount = static_cast<int>(motions.size());
    animationObj.setValue("animationCount", animationCount);

    auto animations = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& a = animations->a;
    a.resize(animationCount);

    //全アニメーションを登録する
    for (int i = 0; i < animationCount; ++i) {
        writeAnimations(a[i].setObject(), motions[i]);
    }

    animationObj.setValue("animations", animations);

    out.setValue("animation", animationValue);
}

void OriginalFormatWriter::writeAnimations(
    JsonObject& out,
    const Motion& motion
) const {
    out.setValue("name", motion.name);

    int numFrame = motion.numFrame;
    out.setValue("numFrame", numFrame);

    const auto& frameMatrix = motion.frameMatrix;
    auto boneCount = frameMatrix.size();

    auto allMatricesValue = std::make_shared<JsonValue>(JsonValueFlag::ARRAY);
    auto& allMatricesArray = allMatricesValue->a;
    allMatricesArray.resize(boneCount);

    for (int i = 0; i < boneCount; ++i) {
        const auto& matrices = frameMatrix[i];

        JsonValue matricesValue(JsonValueFlag::ARRAY);
        auto& a = matricesValue.a;
        a.resize(numFrame * Matrix4::COLUMN_COUNT * Matrix4::ROW_COUNT);
        for (int j = 0; j < numFrame; ++j) {
            auto& matrix = matrices[j];
            int idx = j * Matrix4::COLUMN_COUNT * Matrix4::ROW_COUNT;

            for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
                for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
                    a[idx + y * Matrix4::COLUMN_COUNT + x].setFloat(matrix.m[y][x]);
                }
            }
        }

        allMatricesArray[i] = matricesValue;
    }

    out.setValue("matrices", allMatricesValue);
}

void OriginalFormatWriter::writeMaterials(
    const std::string& filePath,
    const std::vector<int>& materialIDs
) const {
    //ファイルパスからファイルを書き出す階層を取得する
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);

    //マテリアル数
    auto materialCount = static_cast<int>(materialIDs.size());

    //全マテリアルを登録する
    std::unordered_set<int> writenMaterialIDs;
    JsonWriter writer;
    for (size_t i = 0; i < materialCount; ++i) {
        int id = materialIDs[i];
        const auto& m = AssetsManager::instance().getMaterialFromID(id);
        const auto& name = m.name;

        //デフォルトマテリアルなら次へ
        if (name.empty() || name == "default") {
            continue;
        }
        //すでに書き出し済みなら次へ
        if (writenMaterialIDs.find(id) != writenMaterialIDs.end()) {
            continue;
        }

        //マテリアルIDを登録
        writenMaterialIDs.emplace(id);

        //Jsonオブジェクトにマテリアル情報を登録する
        JsonObject root;
        writeMaterial(root, m);

        //ディレクトリパス + マテリアル名で書き出し
        writer.write((directoryPath + name + ".tknmat").c_str(), root);
    }
}

void OriginalFormatWriter::writeMaterial(
    JsonObject& out,
    const Material& material
) const {
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
