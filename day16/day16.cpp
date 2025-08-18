#include <iostream>
#include <string>
#include <bitset>
#include <vector>

#include <ctre.hpp>

#include <timer.h>

bool generate_bit(const std::vector<bool>& in, int n){
    int len = in.size();

    if(n < len) return in[n];

    int v = len;
    while(v * 2 + 1 <= n){
        v *= 2;
        v++;
    }
    if(v == n) return 0;
    return !generate_bit(in, 2 * v - n);
}

bool generate_bit_it(const std::vector<bool>& in, int n){
    int len = in.size();
    bool invert = false;

    while (n >= len) {
        int v = len;
        while (v * 2 + 1 <= n) {
            v = v * 2 + 1;
        }
        if (v == n) {
            return invert;
        }
        n = 2 * v - n;
        invert = !invert;
    }

    return invert ? !in[n] : in[n];
}

void ans(const std::vector<bool>& in, int size){
    std::vector<bool> checksum2(size);

    for(int i = 0; i < size; i++){
        checksum2[i] = generate_bit_it(in, i);
    }
    int l = checksum2.size();
    while(l % 2 == 0){
        for(int i = 0; i < l - 1; i += 2){
            checksum2[i / 2] = checksum2[i] == checksum2[i + 1] ? 1 : 0;
        }
        l /= 2;
    }

    for(int f = 0; f < l; f++){
        std::cout<<checksum2[f];
    }
    std::cout<<'\n';
}


int main() {

    Timer::ScopedTimer t("Day 16");

    std::string linetxt;
    constexpr int P1_DISK_SIZE = 272;
    std::vector<bool> in;
    std::getline(std::cin, linetxt);
    for(int i = 0; i < linetxt.length(); i++){
        if(linetxt[i] == '1'){
            in.emplace_back(1);
        }else{
            in.push_back(0);
        }
    }

    std::cout<<"Part 1: ";
    ans(in, 272);

    std::cout<<"Part 2: ";
    ans(in, 35651584);

}