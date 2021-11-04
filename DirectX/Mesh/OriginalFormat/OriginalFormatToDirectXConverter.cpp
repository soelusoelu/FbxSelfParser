#include "OriginalFormatToDirectXConverter.h"
#include "../../System/Json/JsonInputStream.h"
#include "../../System/Json/JsonReader.h"
#include "../../System/Json/JsonValue.h"
#include "../../System/AssetsManager.h"
#include "../../Utility/FileUtil.h"

OriginalFormatToDirectXConverter::OriginalFormatToDirectXConverter() = default;

OriginalFormatToDirectXConverter::~OriginalFormatToDirectXConverter() = default;

void OriginalFormatToDirectXConverter::convert(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<int>& materialIDs,
    std::vector<Bone>& bones,
    std::vector<Motion>& motions,
    const std::string & filePath,
    const JsonObject& rootObj
) const {
    auto directoryPath = FileUtil::getDirectryFromFilePath(filePath);
    convertMeshes(
        meshesVertices,
        meshesVerticesPosition,
        meshesIndices,
        materialIDs,
        directoryPath,
        rootObj
    );
    convertBones(bones, rootObj);
    convertAnimations(motions, rootObj);
}

void OriginalFormatToDirectXConverter::convertMeshes(
    std::vector<MeshVertices>& meshesVertices,
    std::vector<MeshVerticesPosition>& meshesVerticesPosition,
    std::vector<Indices>& meshesIndices,
    std::vector<int>& materialIDs,
    const std::string& directoryPath,
    const JsonObject& rootObj
) const {
    const auto& meshObj = rootObj.getValue("mesh").getObject();
    //頂点タイプ
    int vertexTypeInt = meshObj.getValue("vertexType").getInt();
    auto vertexType = static_cast<VertexType>(vertexTypeInt);
    //メッシュ数
    int meshCount = meshObj.getValue("meshCount").getInt();
    meshesVertices.resize(meshCount);
    meshesVerticesPosition.resize(meshCount);
    meshesIndices.resize(meshCount);
    materialIDs.resize(meshCount);

    const auto& meshesArray = meshObj.getValue("meshes").getArray();
    for (int i = 0; i < meshCount; ++i) {
        convertMesh(
            meshesVertices[i],
            meshesVerticesPosition[i],
            meshesIndices[i],
            materialIDs[i],
            directoryPath,
            meshesArray[i].getObject(),
            vertexType
        );
    }
}

void OriginalFormatToDirectXConverter::convertMesh(
    MeshVertices& meshVertices,
    MeshVerticesPosition& meshVerticesPosition,
    Indices& meshIndices,
    int& materialID,
    const std::string& directoryPath,
    const JsonObject& meshObj,
    VertexType vertexType
) const {
    int verticesCount = meshObj.getValue("verticesCount").getInt();
    meshVertices.resize(verticesCount);
    meshVerticesPosition.resize(verticesCount);
    meshIndices.resize(verticesCount);

    const auto& srcVertices = meshObj.getValue("vertices").getArray();
    const auto& srcIndices = meshObj.getValue("indices").getArray();
    const auto& srcNormals = meshObj.getValue("normals").getArray();
    const auto& srcUVs = meshObj.getValue("uvs").getArray();

    for (int i = 0; i < verticesCount; ++i) {
        //Vector2, 3用インデックス
        auto idx2 = i * 2;
        auto idx3 = i * 3;

        auto& v = meshVertices[i];
        v.pos.x = srcVertices[idx3].getFloat();
        v.pos.y = srcVertices[idx3 + 1].getFloat();
        v.pos.z = srcVertices[idx3 + 2].getFloat();

        meshVerticesPosition[i] = v.pos;

        meshIndices[i] = srcIndices[i].getInt();

        v.normal.x = srcNormals[idx3].getFloat();
        v.normal.y = srcNormals[idx3 + 1].getFloat();
        v.normal.z = srcNormals[idx3 + 2].getFloat();

        v.uv.x = srcUVs[idx2].getFloat();
        v.uv.x = srcUVs[idx2 + 1].getFloat();
    }

    //頂点タイプがウェイトなら追加読み込み
    if (vertexType == VertexType::POS_NORM_UV_WEIGHT) {
        convertWeights(meshVertices, meshObj);
    }

    const auto& materialName = meshObj.getValue("material").getString();
    convertMaterial(materialID, directoryPath, materialName);
}

void OriginalFormatToDirectXConverter::convertWeights(
    MeshVertices& meshVertices,
    const JsonObject& meshObj
) const {
    int verticesCount = static_cast<int>(meshVertices.size());
    const auto& srcWeights = meshObj.getValue("weights").getArray();
    const auto& srcWeightIndexes = meshObj.getValue("weightIndexes").getArray();

    for (int i = 0; i < verticesCount; ++i) {
        auto idx = i * 4;

        auto& v = meshVertices[i];
        v.weight[0] = srcWeights[idx].getFloat();
        v.weight[1] = srcWeights[idx + 1].getFloat();
        v.weight[2] = srcWeights[idx + 2].getFloat();
        v.weight[3] = srcWeights[idx + 3].getFloat();

        v.index[0] = srcWeightIndexes[idx].getInt();
        v.index[1] = srcWeightIndexes[idx + 1].getInt();
        v.index[2] = srcWeightIndexes[idx + 2].getInt();
        v.index[3] = srcWeightIndexes[idx + 3].getInt();
    }
}

void OriginalFormatToDirectXConverter::convertMaterial(
    int& materialID,
    const std::string& directoryPath,
    const std::string& materialName
) const {
    //既にマテリアルが読み込まれていればIDを取得して終了
    materialID = AssetsManager::instance().getMaterialIDFromName(materialName);
    if (materialID >= 0) {
        return;
    }

    //まだ読み込まれていなければ新たに読み込む
    JsonInputStream stream((directoryPath + materialName + ".tknmat").c_str());
    JsonReader reader;
    JsonObject root;
    reader.parse(stream, root);

    Material material;
    material.name = materialName;

    const auto& ambientArray = root.getValue("ambient").getArray();
    auto& ambient = material.ambient;
    ambient.x = ambientArray[0].getFloat();
    ambient.y = ambientArray[1].getFloat();
    ambient.z = ambientArray[2].getFloat();

    const auto& diffuseArray = root.getValue("diffuse").getArray();
    auto& diffuse = material.diffuse;
    diffuse.x = diffuseArray[0].getFloat();
    diffuse.y = diffuseArray[1].getFloat();
    diffuse.z = diffuseArray[2].getFloat();

    const auto& specularArray = root.getValue("specular").getArray();
    auto& specular = material.specular;
    specular.x = specularArray[0].getFloat();
    specular.y = specularArray[1].getFloat();
    specular.z = specularArray[2].getFloat();

    const auto& emissiveArray = root.getValue("emissive").getArray();
    auto& emissive = material.emissive;
    emissive.x = emissiveArray[0].getFloat();
    emissive.y = emissiveArray[1].getFloat();
    emissive.z = emissiveArray[2].getFloat();

    const auto& bumpArray = root.getValue("bump").getArray();
    auto& bump = material.bump;
    bump.x = bumpArray[0].getFloat();
    bump.y = bumpArray[1].getFloat();
    bump.z = bumpArray[2].getFloat();

    material.transparency = root.getValue("transparency").getFloat();

    material.shininess = root.getValue("shininess").getFloat();

    materialID = AssetsManager::instance().createMaterial(material);
}

void OriginalFormatToDirectXConverter::convertBones(
    std::vector<Bone>& bones,
    const JsonObject& rootObj
) const {
    //boneオブジェクトがなければ終了
    if (rootObj.values.find("bone") == rootObj.values.end()) {
        return;
    }

    const auto& boneObj = rootObj.getValue("bone").getObject();
    //ボーン数
    int boneCount = boneObj.getValue("boneCount").getInt();
    bones.resize(boneCount);

    const auto& bonesArray = boneObj.getValue("bones").getArray();

    //全ボーンを読み込む
    for (int i = 0; i < boneCount; ++i) {
        convertBone(bones[i], bones, bonesArray[i].getObject());
    }
}

void OriginalFormatToDirectXConverter::convertBone(
    Bone& bone,
    std::vector<Bone>& bones,
    const JsonObject& boneObj
) const {
    bone.name = boneObj.getValue("name").getString();
    bone.number = boneObj.getValue("index").getInt();

    int parentIndex = boneObj.getValue("parent").getInt();
    if (parentIndex >= 0) {
        auto& parent = bones[parentIndex];
        bone.parent = &parent;
        parent.children.emplace_back(&bone);
    }

    const auto& initMatrix = boneObj.getValue("initMatrix").getArray();
    for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
        for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
            bone.initMat.m[y][x] = initMatrix[y * Matrix4::COLUMN_COUNT + x].getFloat();
        }
    }

    const auto& offsetMatrix = boneObj.getValue("offsetMatrix").getArray();
    for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
        for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
            bone.offsetMat.m[y][x] = offsetMatrix[y * Matrix4::COLUMN_COUNT + x].getFloat();
        }
    }
}

void OriginalFormatToDirectXConverter::convertAnimations(
    std::vector<Motion>& motions,
    const JsonObject& rootObj
) const {
    //animationオブジェクトがなければ終了
    if (rootObj.values.find("animation") == rootObj.values.end()) {
        return;
    }

    const auto& animationObj = rootObj.getValue("animation").getObject();
    //アニメーション数
    int animationCount = animationObj.getValue("animationCount").getInt();
    motions.resize(animationCount);

    const auto& animations = animationObj.getValue("animations").getArray();

    //全アニメーションを読み込む
    for (int i = 0; i < animationCount; ++i) {
        convertAnimation(motions[i], animations[i].getObject());
    }
}

void OriginalFormatToDirectXConverter::convertAnimation(
    Motion& motion,
    const JsonObject& animationObj
) const {
    motion.name = animationObj.getValue("name").getString();

    int numFrame = animationObj.getValue("numFrame").getInt();
    motion.numFrame = numFrame;

    const auto& srcRootMatrices = animationObj.getValue("matrices").getArray();
    int boneCount = static_cast<int>(srcRootMatrices.size());

    auto& frameMatrix = motion.frameMatrix;
    frameMatrix.resize(boneCount);

    for (int i = 0; i < boneCount; ++i) {
        const auto& srcMatrices = srcRootMatrices[i].getArray();
        auto& matrices = frameMatrix[i];
        matrices.resize(numFrame);

        for (int j = 0; j < numFrame; ++j) {
            int idx = j * Matrix4::COLUMN_COUNT * Matrix4::ROW_COUNT;

            for (int y = 0; y < Matrix4::COLUMN_COUNT; ++y) {
                for (int x = 0; x < Matrix4::ROW_COUNT; ++x) {
                    matrices[j].m[y][x] = srcMatrices[idx + y * Matrix4::COLUMN_COUNT + x].getFloat();
                }
            }
        }
    }
}
