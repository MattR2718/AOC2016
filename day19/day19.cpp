#include <iostream>
#include <string>
#include <list>
#include <deque>

#include <timer.h>

struct Monkey{
    int id = -1, num_presents = 1;
    int next_monkey;
    int prev_monkey;

    void print(){
        std::cout<<"Monkey: " << id - 1 << " Next_M: " << next_monkey << " Prev_M: " << prev_monkey << " Num_P: " << num_presents << '\n';
    }
};

struct M2{
    int id = -1, num_presents = 1;

    bool operator<(const M2& other) const {
        return id < other.id;
    }

    bool operator==(const M2& other) const {
        return id == other.id;
    }

    void print() const{
        std::cout<<"\tMonkey: " << id + 1 << " Num_P: " << num_presents << '\n';
    }
};



int main() {

    Timer::ScopedTimer t("Day 19");

    std::string linetxt;
    std::getline(std::cin, linetxt);
    int num = std::stoi(linetxt);

    std::vector<Monkey> monkeys(num);
    int num_monkeys = num;
    for(int i = 0; i < num; i++){
        monkeys[i].id = i + 1;
        if(i == num - 1){ 
            monkeys[i].next_monkey = 0;
            monkeys[i].prev_monkey =  i - 1;
        }else if(i == 0){
            monkeys[i].next_monkey = i + 1;
            monkeys[i].prev_monkey = monkeys.size() - 1;
        }else{
            monkeys[i].next_monkey = i + 1;
            monkeys[i].prev_monkey = i - 1;
        }
    }

    int curr_monkey = 0;
    while(num_monkeys > 1){
        //std::cout<<num_monkeys<<'\n';
        if(monkeys[curr_monkey].num_presents == 0){
            monkeys[monkeys[curr_monkey].prev_monkey].next_monkey = monkeys[curr_monkey].next_monkey;
            monkeys[monkeys[curr_monkey].next_monkey].prev_monkey = monkeys[curr_monkey].prev_monkey;
            num_monkeys--;
        }else{
            monkeys[curr_monkey].num_presents += monkeys[monkeys[curr_monkey].next_monkey].num_presents;
            monkeys[monkeys[curr_monkey].next_monkey].num_presents = 0;
        }
        curr_monkey = monkeys[curr_monkey].next_monkey;
    }

    std::cout<<"Part 1: " << curr_monkey + 1 << '\n';

    
    std::deque<int> fh;
    std::deque<int> sh;

    for(int i = 0; i < num / 2; i++){
        fh.emplace_back(i + 1);
        
    }
    for(int i = num / 2; i < num; i++){
        sh.emplace_back(i + 1);
    }

    while(fh.size() + sh.size() > 1){
        bool s = fh.size() == sh.size();
        sh.pop_front();
        sh.push_back(fh.front());
        fh.pop_front();
        if(!s){
            fh.push_back(sh.front());
            sh.pop_front();
        }
    }

    std::cout<<"Part 2: " << sh.front() << '\n';
    
}