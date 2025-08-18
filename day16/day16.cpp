#include <iostream>
#include <string>
#include <set>

#include <ctre.hpp>

#include <timer.h>

int main() {

    Timer::ScopedTimer t("Day 1");

    std::string linetxt;
    int dir{0}, x{0}, y{0};
    std::set<std::pair<int, int>> visited{{0, 0}};
    bool p2_done{false};
    while(std::getline(std::cin, linetxt)){
        // Get directions in the from ([LR])(\\d+) using ctre
        for (auto match: ctre::search_all<"([LR])(\\d+)">(linetxt)){
            if (match.get<1>() == "L") {
                dir = (dir + 3) % 4; // Turn left
            } else if (match.get<1>() == "R") {
                dir = (dir + 1) % 4; // Turn right
            }

            int steps = match.get<2>().to_number<int>();

            if(!p2_done){
                for(int i = 0; i < steps; i++){
                    switch(dir){
                        case 0: y--; break; // North
                        case 1: x++; break; // East
                        case 2: y++; break; // South
                        case 3: x--; break; // West
                    }
                    if(visited.count({x, y}) == 0) {
                        visited.insert({x, y});
                    }else{
                        p2_done = true;
                        std::cout << "Part 2: " << (std::abs(x) + std::abs(y)) << "\n";
                    }
                }
            }else{
                switch(dir){
                    case 0: y -= steps; break; // North
                    case 1: x += steps; break; // East
                    case 2: y += steps; break; // South
                    case 3: x -= steps; break; // West
                }
            }
            
        }

        std::cout << "Part 1: " << (std::abs(x) + std::abs(y)) << "\n";
    }
}