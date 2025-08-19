#include <iostream>
#include <string>
#include <bitset>
#include <queue>

#include <ctre.hpp>

#include <timer.h>
#include <md5Hash.h>

struct Pos{
    int x = -1, y = -1;

    // Up, Down, Left, Right
    std::bitset<4> get_possible_next(std::string hash_in){
        std::bitset<4> r{0};
        MD5Hash::Hash h = MD5Hash::md5(hash_in);
        auto valid = [](char c){
            return c =='b' || c == 'c' || c == 'd' || c == 'e' || c == 'f';
        };
        if(y > 0 && valid(h.get_hex_char(0))) r[0] = true;
        if(y < 3 && valid(h.get_hex_char(1))) r[1] = true;
        if(x > 0 && valid(h.get_hex_char(2))) r[2] = true;
        if(x < 3 && valid(h.get_hex_char(3))) r[3] = true;

        return r;
    }
};

std::string bfs(std::string salt){
    std::queue<std::pair<Pos, std::string>> q;
    q.push({{0, 0}, ""});

    while(!q.empty()){
        auto [pos, route] = q.front();
        q.pop();

        if(pos.x == 3 && pos.y == 3){
            return route;
        }

        auto nei = pos.get_possible_next(salt + route);
        if(nei[0]) q.push({{pos.x, pos.y - 1}, route + "U"});
        if(nei[1]) q.push({{pos.x, pos.y + 1}, route + "D"});
        if(nei[2]) q.push({{pos.x - 1, pos.y}, route + "L"});
        if(nei[3]) q.push({{pos.x + 1, pos.y}, route + "R"});
    }

    return "";
}

int bfs_2(std::string salt){
    std::queue<std::pair<Pos, std::string>> q;
    q.push({{0, 0}, ""});
    int longest = 0;

    while(!q.empty()){
        auto [pos, route] = q.front();
        q.pop();

        if(pos.x == 3 && pos.y == 3){
            if(route.length() > longest) longest = route.length();
            continue;
        }

        auto nei = pos.get_possible_next(salt + route);
        if(nei[0]) q.push({{pos.x, pos.y - 1}, route + "U"});
        if(nei[1]) q.push({{pos.x, pos.y + 1}, route + "D"});
        if(nei[2]) q.push({{pos.x - 1, pos.y}, route + "L"});
        if(nei[3]) q.push({{pos.x + 1, pos.y}, route + "R"});
    }

    return longest;
}

int main() {

    Timer::ScopedTimer t("Day 17");

    std::string linetxt;
    std::getline(std::cin, linetxt);

    std::cout<<"Part 1: " << bfs(linetxt) << '\n';

    std::cout<<"Part 2: " << bfs_2(linetxt) << '\n';


}