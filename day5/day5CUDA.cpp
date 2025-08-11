#include <iostream>
#include <vector>
#include <string>

#include "day5.cuh"

int main() {

    std::string linetxt;
    std::getline(std::cin, linetxt);

    str input, p1, p2;
    input.init(linetxt.c_str(), linetxt.size());
    md5(input, p1, p2);

    std::cout<<"Part1: " << p1 << '\n';
    std::cout<<"Part2: " << p2 << '\n';

    p1.free();
    p2.free();
    input.free();

}
