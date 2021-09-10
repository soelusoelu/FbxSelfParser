#include "DebugManager.h"
#include "DebugUtility/DebugUtility.h"
#include "../../Device/DrawString.h"

DebugManager::DebugManager()
    : FileOperator("DebugManager")
    , mStringDrawer(std::make_unique<DrawString>())
{
}

DebugManager::~DebugManager() {
    DebugUtility::instance().finalize();
}

void DebugManager::initialize() {
    mStringDrawer->initialize();
    DebugUtility::instance().initialize();
}

void DebugManager::preUpdateProcess() {
    //描画済み文字列を削除する
    mStringDrawer->clear();
    DebugUtility::instance().preUpdateProcess();
}

void DebugManager::draw2D(const Renderer& renderer, Matrix4& proj) const {
    DebugUtility::instance().drawLine2D(renderer, proj);
}

void DebugManager::drawDebug2D(Matrix4& proj) const {
    DebugUtility::instance().draw(*mStringDrawer);
    mStringDrawer->drawAll(proj);
}

void DebugManager::draw3D(
    const Renderer& renderer,
    const Matrix4& viewProj
) const {
    DebugUtility::instance().draw3D(renderer, viewProj);
}

void DebugManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::LOAD) {
        mStringDrawer->writeAndRead(inObj, alloc, mode);
    }
    DebugUtility::instance().writeAndRead(inObj, alloc, mode);
}
