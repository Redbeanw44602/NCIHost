//
// Created by cinit on 2021-06-09.
//

#include "ArgList.h"
#include <mutex>
#include "SharedBuffer.h"
#include "Rva.h"

using namespace std;

using uchar = ArgList::uchar;

void ArgList::Builder::pushRawInline(uchar type, uint64_t value) {
    mArgTypes.put(mCount, type);
    mArgInlineVars.put(mCount, value);
    mCount++;
}

void ArgList::Builder::pushRawInlineArray(uchar type, const uint64_t *values, int count) {
    mArgTypes.put(mCount, type);
    mArgInlineVars.put(mCount, count);
    SharedBuffer buffer;
    buffer.ensureCapacity(count * 8);
    memcpy(buffer.get(), values, count * 8);
    mCount++;
}

void ArgList::Builder::pushRawBuffer(uchar type, const void *buffer, size_t size) {
    mArgTypes.put(mCount, type);
    mArgInlineVars.put(mCount, size);
    SharedBuffer sb;
    sb.ensureCapacity(size ? size : 8);
    memcpy(sb.get(), buffer, size);
    mArgBuffers.put(mCount, sb);
    mCount++;
}

ArgList::Builder &ArgList::Builder::push(const string &value) {
    const char *buffer = nullptr;
    size_t size = 0;
    extractStringBuffer(value, &buffer, &size);
    if (buffer == nullptr) {
        pushRawInline(Types::TYPE_STRING, 0);
    } else {
        pushRawBuffer(Types::TYPE_STRING, (void *) buffer, size + 1);
    }
    return *this;
}

ArgList::Builder &ArgList::Builder::push(const char *value) {
    const char *buffer = nullptr;
    size_t size = 0;
    extractStringBuffer(value, &buffer, &size);
    if (buffer == nullptr) {
        pushRawInline(Types::TYPE_STRING, 0);
    } else {
        pushRawBuffer(Types::TYPE_STRING, (void *) buffer, size + 1);
    }
    return *this;
}

SharedBuffer ArgList::Builder::build() const {
    SharedBuffer buffer;
    int regStart = 8 + (mCount + 7) / 8 * 8;
    int poolStart = regStart + 8 * mCount;
    buffer.ensureCapacity(poolStart);
    *buffer.at<uint32_t>(4) = mCount;
    size_t pos = poolStart;
    for (int i = 0; i < mCount; i++) {
        uchar type = *mArgTypes.get(i);
        *buffer.at<uchar>(8 + i) = type;
        if (mArgBuffers.containsKey(i)) {
            const SharedBuffer *argbuf = mArgBuffers.get(i);
            size_t size = argbuf->size();
            if (size == 0 || argbuf->get() == nullptr) {
                *buffer.at<uint32_t>(regStart + i * 8) = (uint32_t) pos;
                *buffer.at<uint32_t>(regStart + i * 8 + 4) = 0;
                buffer.ensureCapacity(pos + 8);
                pos += 8;
            } else {
                *buffer.at<uint32_t>(regStart + i * 8) = (uint32_t) pos;
                *buffer.at<uint32_t>(regStart + i * 8 + 4) =
                        mArgInlineVars.containsKey(i) ? ((uint32_t) *mArgInlineVars.get(i)) : ((uint32_t) size);
                size_t allocSize = (size + 7) / 8 * 8;
                buffer.ensureCapacity(pos + allocSize);
                memcpy(buffer.at<uchar>(pos), argbuf->get(), size);
                pos += allocSize;
            }
        } else {
            uint64_t value = *mArgInlineVars.get(i);
            *buffer.at<uint64_t>(regStart + i * 8) = value;
        }
    }
    *buffer.at<uint32_t>(0) = (uint32_t) buffer.size();
    return buffer;
}

void ArgList::Builder::reset() {
    mCount = 0;
    mArgTypes.clear();
    mArgInlineVars.clear();
    mArgBuffers.clear();
}

ArgList::ArgList() = default;

ArgList::ArgList(const void *buffer, size_t size)
        : mBuffer(buffer), mLength(size) {
    Rva rva(mBuffer, mLength);
    if (buffer != nullptr && size >= 8 && *rva.at<uint32_t>(0) <= mLength) {
        mCount = (int) *rva.at<uint32_t>(4);
        mRegOffset = 8 + (mCount + 7) / 8 * 8;
        mPoolOffset = mRegOffset + 8 * mCount;
        if (*rva.at<uint32_t>(0) >= mPoolOffset) {
            mIsValid = true;
        }
    }
}

bool ArgList::readRawInlineValue(uint64_t *out, int index) const noexcept {
    if (!mIsValid || out == nullptr || index < 0 || index >= mCount) {
        return false;
    }
    Rva rva(mBuffer, mLength);
    *out = *rva.at<uint64_t>(mRegOffset + index * 8);
    return true;
}
