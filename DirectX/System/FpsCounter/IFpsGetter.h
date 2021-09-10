#pragma once

class IFpsGetter {
public:
    virtual ~IFpsGetter() = default;
    virtual float getFps() const = 0;
};
