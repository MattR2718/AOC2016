#include <iostream>
#include <string>
#include <bit>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <execution>

#include <ctre.hpp>

bool wall(const std::pair<int, int> pos, int num){
    auto [x, y] = pos;
    uint32_t sum = x*x + 3*x + 2*x*y + y + y*y + num;
    return std::popcount(sum) % 2;
}

struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

int main() {
    std::string linetxt;
    std::getline(std::cin, linetxt);
    int f_num = std::stoi(linetxt);
    std::pair<int, int> goal;
    if(f_num == 10){
        goal = {7, 4};
    }else{
        goal = {31, 39};
    }



    int p1 = 0, p2 = 0;
    bool p2_done = false;
    std::unordered_set<std::pair<int, int>, PairHash> visited;
    std::queue<std::pair<std::pair<int, int>, int>> q;
    static const std::vector<std::pair<int, int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    q.push({{1, 1}, 0});
    visited.insert({1, 1});

    while (!q.empty()) {
        auto [pos, steps] = q.front();
        q.pop();

        if (!p1 && pos == goal) {
            p1 = steps; // Found shortest path to goal
        }

        if (steps <= 50) {
            p2++;
        }

        for (const auto& [dx, dy] : dirs) {
            std::pair<int, int> next = {pos.first + dx, pos.second + dy};
            if (next.first >= 0 && next.second >= 0 && !visited.count(next) && !wall(next, f_num)) {
                visited.insert(next);
                q.push({next, steps + 1});
            }
        }
    }


    std::cout << "Part 1: " << p1 << '\n';
    std::cout << "Part 2: " << p2 << '\n';

}
