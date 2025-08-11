#pragma once

#include <iostream>

struct str{
    char* data;
    size_t size;

    void free() {
        delete[] data;
    }

    str() : data(nullptr), size(0) {}

    void init(const char* input, size_t length) {
        data = new char[length];
        memcpy(data, input, length);
        size = length;
    }
};

inline std::ostream& operator<<(std::ostream& os, const str& s) {
    if (s.data) {
        os.write(s.data, s.size);
    }
    return os;
}


struct POS{
    uint8_t value;
    uint32_t index;
};

struct ANS{
    POS ans[8];
};

void md5(const str& input, str& p1, str& p2);

