#pragma once

#include <stdint.h>

class StatusMixin {
public:
    uint16_t statusFlags = 0;

    template<typename T> void setStatus(T flag)   { statusFlags |=  static_cast<uint16_t>(flag); }
    template<typename T> void clearStatus(T flag) { statusFlags &= ~static_cast<uint16_t>(flag); }
    template<typename T> bool hasStatus(T flag)   { return statusFlags & static_cast<uint16_t>(flag); }
    void resetStatus() { statusFlags = 0; }
};
