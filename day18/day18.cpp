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

    for(int n = 1; n < 40; n++){
        for(int i = 0; i < linetxt.length(); i++){
            if(num.get(i - 1) && num.get(i) && !num.get(i + 1)){ temp.set_bit(i); }
            else if(!num.get(i - 1) && num.get(i) && num.get(i + 1)){ temp.set_bit(i); }
            else if(num.get(i - 1) && !num.get(i) && !num.get(i + 1)){ temp.set_bit(i); }
            else if(!num.get(i - 1) && !num.get(i) && num.get(i + 1)){ temp.set_bit(i); }
        }
        p1 += temp.count();
        num = temp;
        temp.reset();
    }

    std::cout<<"Part 1: " << p1 << '\n';

    for(int n = 40; n < 400000; n++){
        for(int i = 0; i < linetxt.length(); i++){
            if(num.get(i - 1) && num.get(i) && !num.get(i + 1)){ temp.set_bit(i); }
            else if(!num.get(i - 1) && num.get(i) && num.get(i + 1)){ temp.set_bit(i); }
            else if(num.get(i - 1) && !num.get(i) && !num.get(i + 1)){ temp.set_bit(i); }
            else if(!num.get(i - 1) && !num.get(i) && num.get(i + 1)){ temp.set_bit(i); }
        }
        p1 += temp.count();
        num = temp;
        temp.reset();
    }

    std::cout<<"Part 2: " << p1 << '\n';

}
