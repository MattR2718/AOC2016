#pragma once

#include <iostream>
#include <cstring>

struct str{
    char* data;
    size_t size;

    void free() {
        delete[] data;
    }

    str() : data(nullptr), size(0) {}

    void init(const char* input, size_t length) {
        data = new char[length];
	    std::memcpy(data, input, length);
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
    int32_t index;
};

struct ANS{
    POS ans[8];
};

std::pair<std::string, std::string> md5(const str& input);

