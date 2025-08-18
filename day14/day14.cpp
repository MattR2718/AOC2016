#include <iostream>
#include <string>
#include <set>
#include <climits>
#include <map>

#include <ctre.hpp>

#include <md5Hash.h>
#include <timer.h>

std::map<std::string, MD5Hash::Hash> cache;
std::map<std::string, MD5Hash::Hash> long_cache;

uint8_t three(const MD5Hash::Hash& h){
    for(int i = 0; i < 32 - 2; i++){
        if(h.get_hex_value(i) == h.get_hex_value(i + 1) 
        && h.get_hex_value(i + 1) == h.get_hex_value(i + 2))
        {
            return h.get_hex_value(i);
        }
    }
    return UINT8_MAX;
}

bool five(const MD5Hash::Hash& h, uint8_t v){
    for(int i = 0; i < 32 - 4; i++){
        bool found = true;
        for(int j = i; j < i + 5; j++){
            if(h.get_hex_value(j) != v){
                found = false;
                break;
            }
        }
        if(found){
            return true;
        }
    }
    return false;
}

template <typename f>
int solve(const std::string& salt, f func){
    auto make_hash_input = [&](int index) {
        char numbuf[32];
        auto len = sprintf(numbuf, "%d", index);
        return salt + std::string(numbuf, len);
    };

    int num_found = 0;
    int i = 0;
    int p1 = 0;
    while(num_found < 64){
        std::string in = make_hash_input(i);
        uint8_t val = three(func(in));
        if(val < UINT8_MAX){
            for(int j = 1; j < 1001; j++){
                std::string in2 = make_hash_input(i + j);
                if(five(func(in2), val)){
                    num_found++;
                    p1 = i;
                    break;
                }
            }
        }
        i++;
    }

    return p1;
}

int main() {
    Timer::ScopedTimer t("Day 14");
    std::string salt;
    std::getline(std::cin, salt);

    int p1 = solve(salt, [](std::string str){
        if(cache.contains(str)){
            return cache.at(str);
        }else{
            auto h = MD5Hash::md5(str);
            cache[str] = h;
            return h;
        }
    });

    std::cout<<"Part 1: " << p1 <<'\n';

    
    int p2 = solve(salt, [](std::string str){
        if(long_cache.contains(str)){
            return long_cache.at(str);
        }else{
            MD5Hash::Hash h = MD5Hash::md5(str);
            for(int i = 0; i < 2016; i++){
                h = MD5Hash::md5(h.to_string());
            }
            long_cache[str] = h;
            return h;
        }
    });

    std::cout<<"Part 2: " << p2 <<'\n';
    
}
