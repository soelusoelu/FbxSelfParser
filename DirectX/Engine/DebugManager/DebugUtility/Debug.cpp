#include "Debug.h"
#include "DebugUtility.h"
#include "LineRenderer/LineInstancingDrawer.h"
#include "LineRenderer/LineRenderer2D.h"
#include "Log.h"
#include "PointRenderer.h"
#include "../../../System/SystemInclude.h"

void Debug::windowMessage(const std::string& message) {
    MessageBoxA(0, message.c_str(), 0, MB_OK);
}

void Debug::log(const std::string& message) {
    DebugUtility::instance().log().log(message);
}

void Debug::logError(const std::string& message) {
    DebugUtility::instance().log().logError(message);
}

void Debug::logWarning(const std::string& message) {
    DebugUtility::instance().log().logWarning(message);
}

void Debug::logClear() {
    DebugUtility::instance().log().clear();
}

void Debug::renderPoint(const Vector3& point, const Vector3& color) {
    DebugUtility::instance().pointRenderer().renderPoint(point, color);
}

void Debug::renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color) {
    DebugUtility::instance().lineRenderer2D().renderLine(p1, p2, color);
}

void Debug::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    DebugUtility::instance().lineRenderer3D().add(p1, p2, color);
}
