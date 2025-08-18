#include <iostream>
#include <string>
#include <bitset>
#include <vector>

#include <ctre.hpp>

#include <timer.h>

std::vector<char> generate_string(std::vector<char> in, int size) {
    in.reserve(size);
    int blocksize = in.size();
    while(in.size() < size){
        in.emplace_back('0');
        for(int i = in.size() - 2; i > -1; i--){
            in.emplace_back((in[i] == '1' ? '0' : '1'));
        }
    }
    in.resize(size);

    return in;
}

void ans(const std::vector<char>& in, int size){
    std::vector<char> checksum2(size);

    checksum2 = generate_string(in, size);
    int l = checksum2.size();
    while(l % 2 == 0){
        for(int i = 0; i < l - 1; i += 2){
            checksum2[i / 2] = checksum2[i] == checksum2[i + 1] ? '1' : '0';
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
    std::vector<char> in;
    std::getline(std::cin, linetxt);
    for(int i = 0; i < linetxt.length(); i++){
        if(linetxt[i] == '1'){
            in.emplace_back('1');
        }else{
            in.push_back('0');
        }
    }

    std::cout<<"Part 1: ";
    ans(in, 272);

    std::cout<<"Part 2: ";
    ans(in, 35651584);

}