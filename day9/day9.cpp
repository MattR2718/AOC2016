#include <iostream>
#include <string>
#include <vector>

#include <ctre.hpp>

uint64_t get_length(std::string linetxt, bool p1 = false){
    uint64_t p2 = 0;
    for(int i = 0; i < linetxt.length(); i++){
        if(linetxt[i] == '('){
            auto match = ctre::search<R"(\((\d+)x(\d+)\))">(linetxt.begin() + i, linetxt.end());
            if(match){
                std::string full_match = match.get<0>().to_string();
                int num_char = match.get<1>().to_number<int>();
                int num_repeats = match.get<2>().to_number<int>();
                std::string repeated = linetxt.substr(i + full_match.length(), num_char);
                p2 += (!p1 ? get_length(repeated, false) : repeated.length()) * num_repeats;
                i += full_match.length() + num_char - 1;
            }
        }else{
            p2++;
        }
    }
    return p2;
}

int main() {
    std::string linetxt;
    int p1 = 0;
    uint64_t p2 = 0;
    while(std::getline(std::cin, linetxt)){
        p1 += get_length(linetxt, true);
        p2 += get_length(linetxt, false);
    }

    std::cout<<"Part 1: "<<p1<<'\n';
    std::cout<<"Part 2: "<<p2<<'\n';
}