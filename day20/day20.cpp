#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>


#include <ctre.hpp>

#include <timer.h>

struct Range{
    uint32_t from, to;
};

int main() {

    Timer::ScopedTimer t("Day 20");

    std::string linetxt;
    std::vector<Range> ranges;
    while(std::getline(std::cin, linetxt)){
        auto match = ctre::match<"(\\d+)-(\\d+)">(linetxt);
        if(match){
            ranges.emplace_back(Range{match.get<1>().to_number<uint32_t>(), match.get<2>().to_number<uint32_t>()});
        }else{
            std::cout<<"No match found\n";
        }
    }

    std::ranges::sort(ranges, [](auto r1, auto r2){ return r1.from < r2.from; });

    std::vector<Range> merged_ranges;
    for(const auto& r : ranges){
        if(merged_ranges.size() && merged_ranges[merged_ranges.size() - 1].to >= r.from - 1){
            merged_ranges[merged_ranges.size() - 1].to = std::max(r.to, merged_ranges[merged_ranges.size() - 1].to);
        }else{
            merged_ranges.emplace_back(r);
        }
    }

    std::cout<<"Part 1: " << ((merged_ranges[0].from == 0) ? merged_ranges[0].to + 1 : 0 )<< '\n';
    
    uint32_t p2 = 0;
    
    for(int i = 0; i < merged_ranges.size() - 1; i++){
        p2 += merged_ranges[i + 1].from - merged_ranges[i].to - 1;
    }
    p2 += UINT32_MAX - merged_ranges[merged_ranges.size() - 1].to;
    
    std::cout<<"Part 2: " << p2<< '\n';
}

//218 high