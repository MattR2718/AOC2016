#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>
#include <algorithm>
#include <unordered_map>


#include <ctre.hpp>

#include <timer.h>

int main() {
    Timer::ScopedTimer t("Day 4");

    std::string linetxt;

    int p1{0}, p2{0};


    while(std::getline(std::cin, linetxt)){
      auto match = ctre::match<"([a-z\\-]+)-(\\d+)\\[([a-z]+)\\]">(linetxt);
      if(match){
        
        std::string name = match.get<1>().to_string();
        std::string name2 = name;

        // Remove all - from the string
        name.erase(std::remove(name.begin(), name.end(), '-'), name.end());

        int id = match.get<2>().to_number<int>();
        std::string checksum = match.get<3>().to_string();

        //std::cout << "Name: " << name << ", ID: " << id << ", Checksum: " << checksum << "\n";

        std::unordered_map<char, int> char_count;
        for(char c : name) {
            char_count[c]++;
        }

        int i = 0;
        std::vector<std::pair<char, int>> sorted_chars(char_count.begin(), char_count.end());
        std::ranges::sort(sorted_chars, [](const auto &a, const auto &b) {
            if(a.second == b.second) {
                return a.first < b.first; // Sort by character if counts are equal
            }
            return a.second > b.second; // Sort by count in descending order
        });

        for(int j = 0; j < 5 && j < sorted_chars.size(); j++){
          if(sorted_chars[j].first == checksum[i])
            i++;
        }
        if(i == 5){ 
          p1 += id;

          std::string new_name = "";
          for(char c : name2){
            if(c == '-'){
              new_name += " ";
            }else{
              char shifted_char = 'a' + (c - 'a' + id) % 26;
              new_name += shifted_char;
            }
          }
          //std::cout<<new_name<<'\n';
          if(ctre::match<"[a-z ]*north[a-z ]*">(new_name)){
            std::cout<<new_name<<'\n';
            p2 = id;
          }
        }

      }


      
    }

    

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}