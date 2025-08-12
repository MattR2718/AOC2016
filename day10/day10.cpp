#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <cstdint>
#include <climits>

#include <ctre.hpp>

struct Bot{
    int id = -1;
    int low = -1, high = -1;
    Bot* low_recipient = nullptr;
    Bot* high_recipient = nullptr;
    int output_value = -1;
    bool output = false;

    void send(){
        if(low_recipient) low_recipient->receive(low);
        if(high_recipient) high_recipient->receive(high);
    }

    void receive(int value){

        if(output) {
            output_value = value;
            return;
        }

        if(low == -1) low = value;
        else if(high == -1 && low > value){
            high = low;
            low = value;
        }
        else if(high == -1) high = value;
        else{
            std::cout << "Bot is already full!\n";
        }
    }

    void print() const {
        if(output){
            std::cout<<"Output: " << id << "\n";
            std::cout<<"\tValue: "<<output_value<<"\n";
            return;
        }

        std::cout<<"Bot: "<<id<<"\n";
        std::cout<<"\tLow: "<<low<<"\n";
        std::cout<<"\tHigh: "<<high<<"\n";
        if(low_recipient) std::cout<<"\tLow Recipient: "<< (low_recipient->output ? "Output" : "Bot") << low_recipient->id<<"\n";
        if(high_recipient) std::cout<<"\tHigh Recipient: "<< (high_recipient->output ? "Output" : "Bot") << high_recipient->id<<"\n";
    }
};

struct Output{
    std::vector<int> values;
};

int main(int argc, char** argv) {
    std::string linetxt;
    int p1 = 0;
    int p2 = 0;
    
    int max_bot = 0;
    int max_output = 0;

    if (argc > 1) {
        freopen(argv[1], "r", stdin);
    }
    
    std::vector<std::string> instructions;
    std::vector<std::string> init;
    while(std::getline(std::cin, linetxt)){
        //instructions.push_back(linetxt);
        if(auto match = ctre::search<"value (\\d+) goes to bot (\\d+)">(linetxt)){
            int value = match.get<1>().to_number<int>();
            int bot = match.get<2>().to_number<int>();
            if(bot > max_bot) max_bot = bot;
            init.emplace_back(linetxt);
        }else if(auto match = ctre::search<"bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)">(linetxt)){
            instructions.emplace_back(linetxt);
            int bot = match.get<1>().to_number<int>();
            std::string low_recipient = match.get<2>().to_string();
            int low = match.get<3>().to_number<int>();
            std::string high_recipient = match.get<4>().to_string();
            int high = match.get<5>().to_number<int>();

            if(bot > max_bot) max_bot = bot;
            if(low_recipient == "output"){
                if(low > max_output) max_output = low;
            }else if(low_recipient == "bot"){
                if(low > max_bot) max_bot = low;
            }else{
                std::cout << "Unknown low recipient: " << low_recipient << "\n";
            }

            if(high_recipient == "output"){
                if(high > max_output) max_output = high;
            }else if(high_recipient == "bot"){
                if(high > max_bot) max_bot = high;
            }else{
                std::cout << "Unknown high recipient: " << high_recipient << "\n";
            }

        }
    }

    //std::ranges::sort(instructions, [](const std::string& a, const std::string& b) {
    //    return a[0] > b[0];
    //});


    std::vector<Bot> bots(max_bot + 1);
    for(int i = 0; i < bots.size(); ++i){
        bots[i].id = i;
    }

    std::vector<Bot> outputs(max_output + 1);
    for(int i = 0; i < outputs.size(); ++i){
        outputs[i].id = i;
        outputs[i].output = true;
    }

    for(const auto& init_line : init){
        if(auto match = ctre::search<"value (\\d+) goes to bot (\\d+)">(init_line)){
            int value = match.get<1>().to_number<int>();
            int bot = match.get<2>().to_number<int>();
            bots[bot].receive(value);
        }else{
            std::cout << "Unknown initialization: " << init_line << "\n";
        }
    }

    for(const auto& instr : instructions){
        if(auto match = ctre::search<"bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)">(instr)){
            int bot = match.get<1>().to_number<int>();
            std::string low_recipient = match.get<2>().to_string();
            int low = match.get<3>().to_number<int>();
            std::string high_recipient = match.get<4>().to_string();
            int high = match.get<5>().to_number<int>();

            if(low_recipient == "output"){
                bots[bot].low_recipient = &outputs[low];
            }else if(low_recipient == "bot"){
                bots[bot].low_recipient = &bots[low];
            }else{
                std::cout << "Unknown low recipient: " << low_recipient << "\n";
            }

            if(high_recipient == "output"){
                bots[bot].high_recipient = &outputs[high];
            }else if(high_recipient == "bot"){
                bots[bot].high_recipient = &bots[high];
            }else{
                std::cout << "Unknown high recipient: " << high_recipient << "\n";
            }

        }
    }

    while(!std::all_of(bots.begin(), bots.end(), [](const Bot& bot) {
        return bot.low == -1 && bot.high == -1 || bot.low == -1 && bot.high != -1 || bot.low != -1 && bot.high == -1;
    })) {

        std::ranges::for_each(bots, [&p1](Bot& bot) {
            if(bot.low == 17 && bot.high == 61) {
                p1 = bot.id;
            }
            if(bot.low != -1 && bot.high != -1) {
                bot.send();
                bot.low = -1;
                bot.high = -1;
            }
        });
    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << outputs[0].output_value * outputs[1].output_value * outputs[2].output_value << "\n";
}