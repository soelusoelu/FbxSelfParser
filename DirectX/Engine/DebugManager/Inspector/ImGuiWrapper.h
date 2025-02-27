﻿#pragma once

#include "../../../Imgui/imgui.h"
#include "../../../Math/Math.h"
#include <string>

//ImGuiラッパー関数群
namespace ImGuiWrapper {
bool dragInt(const std::string& label, int& v, float speed = 1.f, int min = 0, int max = 0, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool dragFloat(const std::string& label, float& v, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool dragVector2(const std::string& label, Vector2& v, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool dragVector3(const std::string& label, Vector3& v, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool dragVector4(const std::string& label, Vector4& v, float speed = 1.f, float min = 0.f, float max = 0.f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

bool sliderInt(const std::string& label, int& v, int min, int max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool sliderFloat(const std::string& label, float& v, float min, float max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool sliderVector2(const std::string& label, Vector2& v, float min, float max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool sliderVector3(const std::string& label, Vector3& v, float min, float max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
bool sliderVector4(const std::string& label, Vector4& v, float min, float max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

bool colorEdit3(const std::string& label, Vector3& col, ImGuiColorEditFlags flags = 0);
bool colorEdit4(const std::string& label, Vector4& col, ImGuiColorEditFlags flags = 0);
bool colorPicker3(const std::string& label, Vector3& col, ImGuiColorEditFlags flags = 0);
bool colorPicker4(const std::string& label, Vector4& col, ImGuiColorEditFlags flags = 0, const float* ref_col = nullptr);
bool colorButton(const std::string& desc_id, const Vector4& col, ImGuiColorEditFlags flags = 0, const Vector2& size = Vector2::zero);

bool radioButton(const std::string* labels, int& v, int count, bool isSameLine = true);
}
