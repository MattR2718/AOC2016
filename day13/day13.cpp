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
#include <chrono>

#include <ctre.hpp>

inline bool wall(const uint64_t pos, int num){
    uint32_t sum = (pos >> 32)*(pos >> 32) + 3*(pos >> 32) + 2*(pos >> 32)*(pos & 0xFFFFFFFF) + (pos & 0xFFFFFFFF) + (pos & 0xFFFFFFFF)*(pos & 0xFFFFFFFF) + num;
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
    uint64_t goal = 0;
    if(f_num == 10){
        goal = 7ull << 32 | 4;
    }else{
        goal = 31ull << 32 | 39;
    }

    int p1 = 0, p2 = 0;
    bool p2_done = false;
    std::array<bool, 10000> visited{0};
    std::queue<std::pair<uint64_t, int>> q;
    static const std::array<std::pair<int, int>, 4> dirs{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

    auto idx = [](uint64_t num){
        return (num & 0xFFFFFFFF) * 100 + (num >> 32);
    };

    q.push({(1ull << 32 | 1), 0});
    visited[idx((1ull << 32 | 1))] = true;

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
            if(!(pos >> 32) && dx == -1 || !(pos & 0xFFFFFFFF) && dy == -1) continue;
            uint64_t next = (((pos >> 32) + dx) << 32 | (pos & 0xFFFFFFFF) + dy);
            if (next >> 32 >= 0 && (next & 0xFFFFFFFF) >= 0 && !visited[idx(next)] && !wall(next, f_num)) {
                visited[idx(next)] = true;
                q.push({next, steps + 1});
            }
        }
    }

    std::cout << "Part 1: " << p1 << '\n';
    std::cout << "Part 2: " << p2 << '\n';

}
