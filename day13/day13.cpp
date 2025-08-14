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

int bi_bfs(const std::pair<int, int> start_pos, const std::pair<int, int>& goal_pos, int f_num){
    std::unordered_set<std::pair<int, int>, PairHash> vs, ve;
    std::queue<std::pair<int, int>> qs, qe;
    qs.push(start_pos);
    qe.push(goal_pos);
    vs.insert(start_pos);
    ve.insert(goal_pos);
    std::unordered_map<std::pair<int, int>, int, PairHash> dist;
    dist[start_pos] = 0;
    dist[goal_pos] = 0;

    static const std::vector<std::pair<int, int>> dirs{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while(!qs.empty() && !qe.empty()){

        if(qs.size() <= qe.size()){
            auto [x, y] = qs.front();
            qs.pop();

            for(const auto& [dx, dy] : dirs){
                std::pair<int, int> next = {x + dx, y + dy};
                if(ve.count(next)) return dist[next] + dist[{x, y}] + 1;
                if(!vs.count(next) && !wall(next, f_num)){
                    vs.insert(next);
                    dist[next] = dist[{x, y}] + 1;
                    qs.push(next);
                }
            }
        }else{
            auto [x, y] = qe.front();
            qe.pop();

            for(const auto& [dx, dy] : dirs){
                std::pair<int, int> next = {x + dx, y + dy};
                if(vs.count(next)) return dist[next] + dist[{x, y}] + 1;
                if(!ve.count(next) && !wall(next, f_num)){
                    ve.insert(next);
                    dist[next] = dist[{x, y}] + 1;
                    qe.push(next);
                }
            }
        }
    }

    return -1;
}



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

    std::cout<<"Part 1: " << bi_bfs({1, 1}, goal, f_num) << '\n';

    constexpr auto x_pos = std::views::iota(0, 50);
    auto all_possible_positions = std::views::cartesian_product(x_pos, x_pos) 
                                | std::views::transform([](auto t){ return std::pair{std::get<0>(t), std::get<1>(t)}; })
                                | std::views::filter([&f_num](const auto& pos) { return !wall(pos, f_num); });


    auto p2 = std::transform_reduce(
        std::execution::par,
        all_possible_positions.begin(),
        all_possible_positions.end(),
        0,
        std::plus<>{},
        [&](const std::pair<int, int>& pos) {
            int dist = bi_bfs({1, 1}, pos, f_num);
            return (dist != -1 && dist <= 50) ? 1 : 0;
        }
    );

    std::cout<<"Part 2: " << p2 << '\n';

}
