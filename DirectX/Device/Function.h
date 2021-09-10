#pragma once

#include <list>
#include <functional>

template<typename T>
class Function;

template<typename Ret, typename... Args>
class Function<Ret(Args...)>
{
    using FuncType = std::function<Ret(Args...)>;

public:
    Function() = default;
    ~Function() = default;

    //関数を登録する
    void operator+=(const FuncType& f) {
        mFunctions.emplace_back(f);
    }

    //Retrunが機能していない
    void operator()(Args... args) const {
        for (const auto& f : mFunctions) {
            f(args...);
        }
    }

private:
    std::list<FuncType> mFunctions;
};
