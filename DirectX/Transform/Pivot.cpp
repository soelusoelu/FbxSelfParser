#include "Pivot.h"

void PivotFunc::stringToPivot(const std::string& src, Pivot& dst) {
    if (src == "LeftTop") {
        dst = Pivot::LEFT_TOP;
    } else if (src == "CenterTop") {
        dst = Pivot::CENTER_TOP;
    } else if (src == "RightTop") {
        dst = Pivot::RIGHT_TOP;
    } else if (src == "CenterLeft") {
        dst = Pivot::CENTER_LEFT;
    } else if (src == "Center") {
        dst = Pivot::CENTER;
    } else if (src == "CenterRight") {
        dst = Pivot::CENTER_RIGHT;
    } else if (src == "LeftBottom") {
        dst = Pivot::LEFT_BOTTOM;
    } else if (src == "CenterBottom") {
        dst = Pivot::CETNER_BOTTOM;
    } else if (src == "RightBottom") {
        dst = Pivot::RIGHT_BOTTOM;
    }
}

void PivotFunc::pivotToString(const Pivot& src, std::string& dst) {
    if (src == Pivot::LEFT_TOP) {
        dst = "LeftTop";
    } else if (src == Pivot::CENTER_TOP) {
        dst = "CenterTop";
    } else if (src == Pivot::RIGHT_TOP) {
        dst = "RightTop";
    } else if (src == Pivot::CENTER_LEFT) {
        dst = "CenterLeft";
    } else if (src == Pivot::CENTER) {
        dst = "Center";
    } else if (src == Pivot::CENTER_RIGHT) {
        dst = "CenterRight";
    } else if (src == Pivot::LEFT_BOTTOM) {
        dst = "LeftBottom";
    } else if (src == Pivot::CENTER_LEFT) {
        dst = "CenterBottom";
    } else if (src == Pivot::RIGHT_BOTTOM) {
        dst = "RightBottom";
    }
}
