#include "SpriteComponent.h"
#include "../../../Engine/DebugManager/Inspector/ImGuiWrapper.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Imgui/imgui.h"
#include "../../../Sprite/Sprite.h"
#include "../../../Sprite/SpriteManager.h"
#include "../../../Transform/Pivot.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Utility/JsonHelper.h"

SpriteComponent::SpriteComponent() :
    Component(),
    mDrawOrder(0),
    mSprite(std::make_unique<Sprite>()) {
}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::awake() {
    addToManager();
    //setActive(gameObject().getActive());
}

void SpriteComponent::lateUpdate() {
    mSprite->computeWorldTransform();
}

void SpriteComponent::finalize() {
    mSprite->destroy();
}

void SpriteComponent::onEnable(bool value) {
    setActive(value);
}

void SpriteComponent::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDrawOrder, "drawOrder", inObj, alloc, mode);
    if (mode == FileMode::SAVE) {
        JsonHelper::setString(fileName(), "fileName", inObj, alloc);
        JsonHelper::setBool(getActive(), "isActive", inObj, alloc);
        JsonHelper::setVector2(transform().getPosition(), "position", inObj, alloc);
        JsonHelper::setFloat(transform().getRotation(), "rotation", inObj, alloc);
        JsonHelper::setVector2(transform().getScale(), "scale", inObj, alloc);
        JsonHelper::setVector3(getColor(), "color", inObj, alloc);
        JsonHelper::setFloat(getAlpha(), "alpha", inObj, alloc);
        JsonHelper::setVector4(getUV(), "uv", inObj, alloc);
        std::string dst;
        PivotFunc::pivotToString(transform().getPivotEnum(), dst);
        JsonHelper::setString(dst, "pivot", inObj, alloc);
    } else {
        std::string str;
        if (JsonHelper::getString(str, "fileName", inObj)) {
            setTextureFromFileName(str);
        }
        if (bool isActive = true; JsonHelper::getBool(isActive, "isActive", inObj)) {
            setActive(isActive);
        }
        Vector2 vec2;
        if (JsonHelper::getVector2(vec2, "position", inObj)) {
            transform().setPosition(vec2);
        }
        float value = 0.f;
        if (JsonHelper::getFloat(value, "rotation", inObj)) {
            transform().setRotation(value);
        }
        if (JsonHelper::getVector2(vec2, "scale", inObj)) {
            transform().setScale(vec2);
        }
        Vector3 vec3;
        if (JsonHelper::getVector3(vec3, "color", inObj)) {
            setColor(vec3);
        }
        if (JsonHelper::getFloat(value, "alpha", inObj)) {
            setAlpha(value);
        }
        Vector4 vec4;
        if (JsonHelper::getVector4(vec4, "uv", inObj)) {
            setUV(vec4.x, vec4.y, vec4.z, vec4.w);
        }
        if (JsonHelper::getString(str, "pivot", inObj)) {
            auto pivot = Pivot::LEFT_TOP;
            PivotFunc::stringToPivot(str, pivot);
            transform().setPivot(pivot);
        }
    }
}

void SpriteComponent::drawInspector() {
    ImGui::Text("FileName: %s", fileName().c_str());
    transform().drawInspector();

    auto color = getColor();
    if (ImGuiWrapper::colorEdit3("Color", color)) {
        setColor(color);
    }

    auto alpha = getAlpha();
    if (ImGuiWrapper::sliderFloat("Alpha", alpha, 0.f, 1.f)) {
        setAlpha(alpha);
    }

    auto uv = getUV();
    if (ImGuiWrapper::sliderVector4("UV", uv, 0.f, 1.f)) {
        setUV(uv.x, uv.y, uv.z, uv.w);
    }
}

void SpriteComponent::draw(const Matrix4& proj) const {
    mSprite->draw(proj);
}

Transform2D& SpriteComponent::transform() const {
    return mSprite->transform();
}

void SpriteComponent::setColor(const Vector3& color) {
    mSprite->setColor(color);
}

void SpriteComponent::setColor(float r, float g, float b) {
    mSprite->setColor(r, g, b);
}

const Vector3& SpriteComponent::getColor() const {
    return mSprite->getColor();
}

void SpriteComponent::setAlpha(float alpha) {
    mSprite->setAlpha(alpha);
}

float SpriteComponent::getAlpha() const {
    return mSprite->getAlpha();
}

void SpriteComponent::setUV(float l, float t, float r, float b) {
    mSprite->setUV(l, t, r, b);
}

const Vector4& SpriteComponent::getUV() const {
    return mSprite->getUV();
}

const Vector2& SpriteComponent::getTextureSize() const {
    return mSprite->getTextureSize();
}

void SpriteComponent::setActive(bool value) {
    mSprite->setActive(value);
}

bool SpriteComponent::getActive() const {
    return mSprite->getActive();
}

bool SpriteComponent::isDead() const {
    return mSprite->isDead();
}

void SpriteComponent::setTextureFromFileName(const std::string& fileName) {
    mSprite->setTextureFromFileName(fileName);
}

void SpriteComponent::setTexture(const std::shared_ptr<Texture>& texture) {
    mSprite->setTexture(texture);
}

const Texture& SpriteComponent::texture() const {
    return mSprite->texture();
}

int SpriteComponent::getTextureID() const {
    return mSprite->getTextureID();
}

const Shader& SpriteComponent::shader() const {
    return mSprite->shader();
}

const std::string& SpriteComponent::fileName() const {
    return mSprite->fileName();
}

void SpriteComponent::setDrawOrder(int order) {
    mDrawOrder = order;
    mSpriteManager->sort();
}

int SpriteComponent::getDrawOrder() const {
    return mDrawOrder;
}

bool SpriteComponent::compare(const std::shared_ptr<SpriteComponent>& lhs, const std::shared_ptr<SpriteComponent>& rhs) {
    return (lhs->mDrawOrder < rhs->mDrawOrder);
}

void SpriteComponent::setSpriteManager(SpriteManager* manager) {
    mSpriteManager = manager;
}

void SpriteComponent::addToManager() {
    if (mSpriteManager) {
        mSpriteManager->addComponent(shared_from_this());
    }
}
