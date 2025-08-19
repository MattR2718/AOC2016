#include <iostream>
#include <string>
#include <numeric>
#include <set>

#include <ctre.hpp>

#include <timer.h>


int main() {

    Timer::ScopedTimer t("Day 18");

    std::string linetxt;
    std::getline(std::cin, linetxt);


    std::vector<std::string> m{linetxt};
    while(m.size() < 40){
        std::string t = "";
        std::string last = m[m.size() - 1];
        for(int i = 0; i < m[m.size() - 1].length(); i++){
            if((i > 0 && last[i - 1] == '^') && (last[i] == '^') && (i == last.length() - 1 || i < last.length() - 1 && last[i + 1] == '.')){ t += "^"; }
            else if((i == 0 || i > 0 && last[i - 1] == '.') && last[i] == '^' && (i < last.length() - 1 && last[i + 1] == '^')){ t += "^"; }
            else if((i > 0 && last[i - 1] == '^') && last[i] == '.' && (i == last.length() - 1 || i < last.length() - 1 && last[i + 1] == '.')){ t += "^"; }
            else if((i == 0 || i > 0 && last[i - 1] == '.') && last[i] == '.' && (i < last.length() - 1 && last[i + 1] == '^')){ t += "^"; }
            else{ t += "."; }

        }
        m.push_back(t);
    }

    std::cout<<"Part 1: " << std::accumulate(m.begin(), m.end(), 0, [](int acc, std::string s){
        return acc + std::count_if(s.begin(), s.end(), [](char c){
            return c == '.';
        });
    }) << '\n';

    m.clear();
    m.emplace_back(linetxt);
    std::set<std::string> set{};
    while(m.size() < 400000){
        std::string t = "";
        std::string last = m[m.size() - 1];
        for(int i = 0; i < m[m.size() - 1].length(); i++){
            if((i > 0 && last[i - 1] == '^') && (last[i] == '^') && (i == last.length() - 1 || i < last.length() - 1 && last[i + 1] == '.')){ t += "^"; }
            else if((i == 0 || i > 0 && last[i - 1] == '.') && last[i] == '^' && (i < last.length() - 1 && last[i + 1] == '^')){ t += "^"; }
            else if((i > 0 && last[i - 1] == '^') && last[i] == '.' && (i == last.length() - 1 || i < last.length() - 1 && last[i + 1] == '.')){ t += "^"; }
            else if((i == 0 || i > 0 && last[i - 1] == '.') && last[i] == '.' && (i < last.length() - 1 && last[i + 1] == '^')){ t += "^"; }
            else{ t += "."; }

        }
        m.push_back(t);
    }

    std::cout<<"Part 1: " << std::accumulate(m.begin(), m.end(), 0, [](int acc, std::string s){
        return acc + std::count_if(s.begin(), s.end(), [](char c){
            return c == '.';
        });
    }) << '\n';

}

// 1985 low