#include <iostream>
#include <string>
#include <numeric>
#include <set>

#include <ctre.hpp>

#include <timer.h>


struct int128{
    uint64_t high = 0, low = 0;
    int width = 0;

    void set_bit(int pos){
        if(pos < 64){
            low |= 1ull << pos;
        }else{
            high |= 1ull << (pos - 64);
        }
    }

    bool get_bit(int pos) const {
        if(pos < 64){
            return (low & (1ull << pos)) != 0;
        } else {
            return (high & (1ull << (pos - 64))) != 0;
        }
    }

    void init(const std::string& s){
        width = s.length();
        for(int i = 0; i < s.length(); i++){
            if(s[i] == '^') set_bit(i);
        }
    }

    bool get(int pos){
        if(pos < 0 || pos >= width) return 0;
        return get_bit(pos);
    }

    int count() const {
        return width - (std::popcount(high) + std::popcount(low));
    }

    void print(){
        for(int i = 0; i < width; i++){
            std::cout<<(get_bit(i) == true ? "^" : ".");
        }
    }

    void reset(){
        low = 0;
        high = 0;
    }

    int128 left_shift(int n = 1) {
        uint64_t h = high, l = low;
        if (n >= 128) {
            h = 0;
            l = 0;
        } else if (n >= 64) {
            h = l << (n - 64);
            l = 0;
        } else if (n > 0) {
            h = (h << n) | (l >> (64 - n));
            l <<= n;
        }
        return int128{h, l, width};
    }

    int128 right_shift(int n = 1) {
        uint64_t h = high, l = low;
        if (n >= 128) {
            h = 0;
            l = 0;
        } else if (n >= 64) {
            l = h >> (n - 64);
            h = 0;
        } else if (n > 0) {
            l = (l >> n) | (h << (64 - n));
            h >>= n;
        }
        return int128{h, l, width};

    }

    int128 operator|(const int128& other) const {
        return {high | other.high, low | other.low, width};
    }

    int128 operator&(const int128& other) const {
        return {high & other.high, low & other.low, width};
    }

    int128 operator~() const {
        return {~high, ~low, width};
    }
};


int main() {

    Timer::ScopedTimer t("Day 18");

    std::string linetxt;
    std::getline(std::cin, linetxt);

    int p1 = 0, p2 = 0;

    int128 num, temp;
    num.init(linetxt);
    temp.width = num.width;

    p1 = num.count();
    p2 = num.count();

    int128 mask;
    for(int i = 0; i < num.width; i++) mask.set_bit(i);

    for(int n = 1; n < 40; n++){
        temp = ((num.right_shift() & num & ~num.left_shift())
                | (~num.right_shift() & num & num.left_shift())
                | (num.right_shift() & ~num & ~num.left_shift())
                | (~num.right_shift() & ~num & num.left_shift()));
        temp = temp & mask;
        p1 += temp.count();
        num = temp;
        temp.reset();
    }

    std::cout<<"Part 1: " << p1 << '\n';

    for(int n = 40; n < 400000; n++){
        temp = ((num.right_shift() & num & ~num.left_shift())
                | (~num.right_shift() & num & num.left_shift())
                | (num.right_shift() & ~num & ~num.left_shift())
                | (~num.right_shift() & ~num & num.left_shift()));
        temp = temp & mask;
        p1 += temp.count();
        num = temp;
        temp.reset();
    }

    std::cout<<"Part 2: " << p1 << '\n';

}
