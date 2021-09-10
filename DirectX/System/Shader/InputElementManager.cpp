#include "InputElementManager.h"
#include "../SystemInclude.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"

InputElementManager::InputElementManager() {
    mInputLayouts.emplace("Texture.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SpriteInstancing.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        //入力アセンブラにジオメトリ処理用の行列を追加設定する
        { "MATRIX", 0, VertexType::FLOAT4, 1, 0, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 1, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 2, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 3, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "COLOR", 0, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "TEXCOORD", 1, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
    });

    mInputLayouts.emplace("Texture3D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Mesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("NormalMap.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TANGENT", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SkinMesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, VertexType::FLOAT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, VertexType::USHORT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("GBuffer.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Deferred.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Point3D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Line2D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT2, 0, 0, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Line3D.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Line3DInstancing.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        //入力アセンブラにジオメトリ処理用の行列を追加設定する
        { "MATRIX", 0, VertexType::FLOAT4, 1, 0, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 1, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 2, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "MATRIX", 3, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 },
        { "COLOR", 0, VertexType::FLOAT4, 1, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::INSTANCE_DATA, 1 }
    });

    mInputLayouts.emplace("PointLight.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SimpleMesh.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SimpleMeshTexture.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("OutLine.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SkinMeshOutLine.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, VertexType::FLOAT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, VertexType::USHORT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("ShadowDepthTextureCreater.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("Shadow.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });

    mInputLayouts.emplace("SkinMeshShadow.hlsl", InputLayout{
        { "POSITION", 0, VertexType::FLOAT3, 0, 0, SlotClass::VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::FLOAT3, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::FLOAT2, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, VertexType::FLOAT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, VertexType::USHORT4, 0, D3D11_APPEND_ALIGNED_ELEMENT, SlotClass::VERTEX_DATA, 0 }
    });
}

InputElementManager::~InputElementManager() = default;

const std::vector<InputElementDesc>& InputElementManager::createInputLayout(const std::string& shaderName) const {
    auto itr = mInputLayouts.find(shaderName);
    //マップの中に存在している
    if (itr != mInputLayouts.end()) {
        return itr->second;
    } else {
        Debug::windowMessage(shaderName + "のInputLayoutが設定されていません");
    }
}
