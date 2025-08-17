#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <array>

#include <timer.h>

int main() {
    Timer::ScopedTimer t("Day 7");

    std::string linetxt;
    int p1 = 0;
    int p2 = 0;

    while(std::getline(std::cin, linetxt)){
        int bracket_depth = 0;
        bool found_matching_sequence = false;
        for(int i = 0; i < linetxt.length() - 3; i++){
            char c = linetxt[i];
            if(c == '['){ bracket_depth++; }
            else if(c == ']'){ bracket_depth--; }
            if(bracket_depth < 0){ std::cout<< "Unmatched closing bracket found : " << linetxt << '\n'; return 1; }

            if(linetxt[i] == linetxt[i+ 3] && linetxt[i + 1] == linetxt[i + 2] && linetxt[i] != linetxt[i + 1]){
                if(bracket_depth){
                    found_matching_sequence = false;
                    break;
                }else{
                    found_matching_sequence = true;
                }
            }

            
        }
        if(found_matching_sequence){ 
            p1++; 
        }

        int bd = 0;
        std::vector<std::string> abas, babs;
        for(int i = 0; i < linetxt.length() - 2; i++){
            if(linetxt[i] == '[') { bd++; }
            else if(linetxt[i] == ']') { bd--; }

            if(bd == 0 && linetxt[i] == linetxt[i + 2] && linetxt[i] != linetxt[i + 1]){
                abas.push_back(linetxt.substr(i, 2));
            }else if(bd && linetxt[i] == linetxt[i + 2] && linetxt[i] != linetxt[i + 1]){
                std::string bab = linetxt.substr(i, 2);
                std::reverse(bab.begin(), bab.end());
                babs.push_back(bab);
            }
        }
        std::vector<std::string> abas_sorted = abas;
        std::vector<std::string> babs_sorted = babs;
        std::sort(abas_sorted.begin(), abas_sorted.end());
        std::sort(babs_sorted.begin(), babs_sorted.end());
        std::vector<std::string> intersection;
        std::set_intersection(
            abas_sorted.begin(), abas_sorted.end(),
            babs_sorted.begin(), babs_sorted.end(),
            std::back_inserter(intersection)
        );
        if(!intersection.empty()){
            p2++;
        }

    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}

