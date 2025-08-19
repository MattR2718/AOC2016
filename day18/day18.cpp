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
};


int main() {

    Timer::ScopedTimer t("Day 18");

    std::string linetxt;
    std::getline(std::cin, linetxt);

    std::vector<int128> p1(40);
    std::vector<int128> p2(400000);

    p1[0].init(linetxt);
    p2[0] = p1[0];

    for(auto& num : p1){
        num.width = p1[0].width;
    }

    for(auto& num : p2){
        num.width = p1[0].width;
    }

    

    for(int n = 1; n < 40; n++){
        int128& last = p1[n - 1];
        for(int i = 0; i < linetxt.length(); i++){
            if(last.get(i - 1) && last.get(i) && !last.get(i + 1)){ p1[n].set_bit(i); }
            else if(!last.get(i - 1) && last.get(i) && last.get(i + 1)){ p1[n].set_bit(i); }
            else if(last.get(i - 1) && !last.get(i) && !last.get(i + 1)){ p1[n].set_bit(i); }
            else if(!last.get(i - 1) && !last.get(i) && last.get(i + 1)){ p1[n].set_bit(i); }

        }
    }

    std::cout<<"Part 1: " << std::accumulate(p1.begin(), p1.end(), 0, [](int acc, int128 i){
        return acc + i.count();
    }) << '\n';

    for(int n = 1; n < 400000; n++){
        int128& last = p2[n - 1];
        for(int i = 0; i < linetxt.length(); i++){
            if(last.get(i - 1) && last.get(i) && !last.get(i + 1)){ p2[n].set_bit(i); }
            else if(!last.get(i - 1) && last.get(i) && last.get(i + 1)){ p2[n].set_bit(i); }
            else if(last.get(i - 1) && !last.get(i) && !last.get(i + 1)){ p2[n].set_bit(i); }
            else if(!last.get(i - 1) && !last.get(i) && last.get(i + 1)){ p2[n].set_bit(i); }

        }
    }

    std::cout<<"Part 2: " << std::accumulate(p2.begin(), p2.end(), 0, [](int acc, int128 i){
        return acc + i.count();
    }) << '\n';

}
