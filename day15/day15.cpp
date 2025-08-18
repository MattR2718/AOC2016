#include <iostream>
#include <string>
#include <algorithm>

#include <ctre.hpp>

#include <timer.h>

struct Disc{
    int num = -1;
    int num_positions = -1;
    int start_pos = -1;

    friend std::ostream& operator<<(std::ostream& os, const Disc& d){
        os << "Disc: " << d.num << " Positions: " << d.num_positions << " Start: " << d.start_pos <<'\n';
        return os;
    }
};

int main() {

    Timer::ScopedTimer t("Day 15");
    std::vector<Disc> discs;
    std::string linetxt;
    while(std::getline(std::cin, linetxt)){
        auto match = ctre::match<"Disc #(\\d+) has (\\d+) positions; at time=0, it is at position (\\d+).">(linetxt);
        if(match){
            Disc d{match.get<1>().to_number<int>(), match.get<2>().to_number<int>(), match.get<3>().to_number<int>()};
            discs.push_back(d);
        }else{
            std::cout<<"Line not matched: [" << linetxt << "]\n";
        }
    }

    int p1 = 0;
    while(!std::all_of(discs.begin(), discs.end(), [&p1](Disc d){
        return !((p1 + d.num + d.start_pos) % d.num_positions);
    })){
        p1++;
    }

    discs.emplace_back(Disc{static_cast<int>(discs.size() + 1), 11, 0});

    int p2 = 0;
    while(!std::all_of(discs.begin(), discs.end(), [&p2](Disc d){
        return !((p2 + d.num + d.start_pos) % d.num_positions);
    })){
        p2++;
    }


    std::cout<<"Part 1: " << p1 <<'\n';
    std::cout<<"Part 2: " << p2 <<'\n';

    
}