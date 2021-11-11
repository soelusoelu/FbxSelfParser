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

//エンディアン変換
inline int byteSwap(int value) {
    return (value << 24 | (value & 0x0000FF00) << 8 | (value & 0x00FF0000) >> 8 | value >> 24);
}
inline unsigned byteSwap(unsigned value) {
    return (value << 24 | (value & 0x0000FF00) << 8 | (value & 0x00FF0000) >> 8 | value >> 24);
}
inline short byteSwap(short value) {
    return (value << 8 | value >> 8);
}
inline unsigned short byteSwap(unsigned short value) {
    return (value << 8 | value >> 8);
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
