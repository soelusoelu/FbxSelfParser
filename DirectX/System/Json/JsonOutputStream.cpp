#include "JsonOutputStream.h"

JsonOutputStream::JsonOutputStream()
    : mWritePos(0)
{
}

JsonOutputStream::~JsonOutputStream() = default;

void JsonOutputStream::push(char c) {
    mStr[mWritePos] = c;
    ++mWritePos;
}

void JsonOutputStream::addSize(unsigned size) {
    mStr.resize(mStr.size() + size);
}

const char* JsonOutputStream::getStringBufer() const {
    return mStr.data();
}
