#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <ctre.hpp>

#include <timer.h>

void run(const std::vector<std::string>& instructions, std::map<std::string, int>& registers){
    for(int i = 0; i < instructions.size(); i++){
        if(auto m = ctre::match<"cpy ([a-zA-Z]+) (\\w+)">(instructions[i])){
            std::string from = m.get<1>().to_string();
            std::string to = m.get<2>().to_string();
            registers[to] = registers[from];
        }else if(auto m = ctre::match<R"(cpy ((?:\+|-)?[0-9]+) (\w+))">(instructions[i])){
            int from = m.get<1>().to_number<int>();
            std::string to = m.get<2>().to_string();
            registers[to] = from;
        } else if(auto m = ctre::match<"inc (\\w+)">(instructions[i])){
            registers[m.get<1>().to_string()]++;        
        } else if(auto m = ctre::match<"dec (\\w+)">(instructions[i])){
            registers[m.get<1>().to_string()]--;        
        } else if (auto m = ctre::match<R"(jnz ((?:\+|-)?[0-9]+) ((?:\+|-)?[0-9]+))">(instructions[i])) {
            int val = m.get<1>().to_number<int>();
            if(val != 0) {
                i += m.get<2>().to_number<int>() - 1;
            }
        } else if (auto m = ctre::match<R"(jnz ([a-zA-Z]+) ((?:\+|-)?[0-9]+))">(instructions[i])) {
            if(registers[m.get<1>().to_string()] != 0) {
                i += m.get<2>().to_number<int>() - 1;
            }
        }else{
            std::cout<<"Unknown Instruction: "<<instructions[i]<<"\n";
        }
    }
}

int main() {
    Timer::ScopedTimer t("Day 12");

    std::string linetxt;
    std::vector<std::string> instructions;
    while(std::getline(std::cin, linetxt)){
        instructions.push_back(linetxt);
    }

    std::map<std::string, int> r1;
    run(instructions, r1);
    std::cout<<"Part 1: "<<r1["a"]<<"\n";

    std::map<std::string, int> r2{{"c", 1}};
    run(instructions, r2);
    std::cout<<"Part 2: "<<r2["a"]<<"\n";

}