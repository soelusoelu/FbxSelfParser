#pragma once

template<typename T>
class IThisGetter {
public:
    virtual ~IThisGetter() = default;
    virtual T& getThis() = 0;
    virtual const T& getThis() const = 0;
};
