#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include "day5.cuh"

int main() {

    std::string linetxt;
    std::getline(std::cin, linetxt);

    str input;
    std::string p1, p2;
    input.init(linetxt.c_str(), linetxt.size());
    std::tie(p1, p2) = md5(input);
    
    std::cout<<"Part1: " << p1 << '\n';
    std::cout<<"Part2: " << p2 << '\n';

    input.free();

}
