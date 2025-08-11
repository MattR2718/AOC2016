#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>
#include <algorithm>


#include <ctre.hpp>

int main() {
    std::string linetxt;

    int p1{0}, p2{0};

    std::vector<int> nums21;
    std::vector<int> nums22;
    std::vector<int> nums23;

    while(std::getline(std::cin, linetxt)){
      auto match = ctre::match<"\\s*(\\d+)\\s+(\\d+)\\s+(\\d+)">(linetxt);
      if(match){
        std::vector<int> nums{ match.get<1>().to_number<int>(), match.get<2>().to_number<int>(), match.get<3>().to_number<int>() };

        nums21.push_back(nums[0]);
        nums22.push_back(nums[1]);
        nums23.push_back(nums[2]);
        
        std::ranges::sort(nums);
        p1 += (nums[0] + nums[1] > nums[2]) ? 1 : 0;


        

        if(nums21.size() == 3){
          std::ranges::sort(nums21);
          std::ranges::sort(nums22);
          std::ranges::sort(nums23);
          p2 += (nums21[0] + nums21[1] > nums21[2]) ? 1 : 0;
          p2 += (nums22[0] + nums22[1] > nums22[2]) ? 1 : 0;
          p2 += (nums23[0] + nums23[1] > nums23[2]) ? 1 : 0;
          nums21.clear();
          nums22.clear();
          nums23.clear();
        }
      }

    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}