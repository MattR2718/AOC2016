#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <array>


int main() {
    std::string linetxt;
    std::string p1 = "", p2 = "";

    std::array<std::array<int, 26>, 8> counts{0};

    while(std::getline(std::cin, linetxt)){
        for(int i = 0; i < linetxt.length(); i++){
            counts[i][linetxt[i] - 'a']++;
        }
    }

    for(const auto& col : counts){
        auto max_elem = std::ranges::max_element(col);
        p1 += (max_elem != col.end()) ? (char)(std::distance(col.begin(), max_elem) + 'a') : ' ';

        auto min_elem = std::ranges::min_element(col, [](int a, int b) {
            return a != 0 && (b == 0 || a < b);
        });
        p2 += (min_elem != col.end()) ? (char)(std::distance(col.begin(), min_elem) + 'a') : ' ';
    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}
