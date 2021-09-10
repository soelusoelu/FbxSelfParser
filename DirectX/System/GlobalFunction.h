#pragma once

#include <cassert>

template <typename T>
inline void safeDelete(T*& p) {
    delete p;
    p = nullptr;
}

template <typename T>
inline void safeDeleteArray(T*& p) {
    delete[] p;
    p = nullptr;
}

template <typename T>
inline void safeRelease(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

template<typename To, typename From>
inline To checkedCast(From obj) {
#if _DEBUG
    if (obj) {
        To ret = dynamic_cast<To>(obj);
        assert(ret);
        return ret;
    }
#endif // _DEBUG
    return static_cast<To>(obj);
}
