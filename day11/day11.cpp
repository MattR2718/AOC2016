#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

#include <ctre.hpp>
#include <boost/bimap.hpp>

#define PRINT 0

enum class ItemType{
    Microchip,
    Generator
};

struct Item{
    ItemType type;
    int id;
    int floor;

    std::string get_name(const boost::bimap<std::string, int>& name_to_id) const {
        return name_to_id.right.at(id);
    }

};

bool operator==(const Item& a, const Item& b) {
    bool same_type_and_id = a.type == b.type && a.id == b.id;
    return same_type_and_id && a.floor == b.floor;
}


int get_from_bimap(const boost::bimap<std::string, int>& name_to_id, const std::string& name) {
    auto it = name_to_id.left.find(name);
    if (it != name_to_id.left.end()) {
        return it->second;
    }
    return -1;
}

std::string get_name_from_bimap(const boost::bimap<std::string, int>& name_to_id, int id) {
    auto it = name_to_id.right.find(id);
    if (it != name_to_id.right.end()) {
        return it->second;
    }
    return "";
}

bool in_bimap(const boost::bimap<std::string, int>& name_to_id, const std::string& name) {
    return name_to_id.left.find(name) != name_to_id.left.end();
}

bool in_bimap(const boost::bimap<std::string, int>& name_to_id, int id) {
    return name_to_id.right.find(id) != name_to_id.right.end();
}

struct State{
    std::vector<Item> items;
    int elevator_position;
    const boost::bimap<std::string, int>* name_to_id;

    bool operator==(const State& other) const {
        return items == other.items && elevator_position == other.elevator_position;
    }

    void print() const {
        std::cout<<"--------------------------------------------------------\n";
        std::cout << "Elevator Position: " << elevator_position << "\n";
        auto items__sorted = items;
        std::ranges::sort(items__sorted, [](const Item& a, const Item& b) {
            return a.floor > b.floor || (a.floor == b.floor && a.id < b.id);
        });
        for (const auto& item : items__sorted) {
            std::cout << "Floor: " << item.floor << " " << item.get_name(*name_to_id) << (item.type == ItemType::Microchip ? " Microchip" : " Generator") << "\n";
        }
        std::cout<<"--------------------------------------------------------\n";

    }

    bool is_valid() const {
        // Check if the state is valid (no microchip is left with an incompatible generator)
        for (const auto& item : items) {
            if (item.type == ItemType::Microchip) {
                bool has_compatible_generator = false;
                int num_generators_on_floor = 0;
                for(const auto& item2 : items){

                    if(item2.type == ItemType::Generator && item2.id == item.id && item2.floor == item.floor){
                        has_compatible_generator = true;
                    }
                    if(item2.type == ItemType::Generator && item2.floor == item.floor){
                        num_generators_on_floor++;
                    }
                }
                if(!has_compatible_generator && num_generators_on_floor > 0) {
#if PRINT
                    std::cout<<"Invalid state due to missing compatible generator for microchip "<<item.get_name(*name_to_id)<<"\n";
                    print();
#endif
                    return false;
                }
            }
        }
        return true;
    }


    std::vector<State> next_states(){
        std::vector<State> states;

        for (int i = 0; i < items.size(); i++) {
            if(items[i].floor != elevator_position) {
                continue;
            }
            for (int direction : {-1, 1}) {
                int new_floor = elevator_position + direction;
                if (new_floor >= 0 && new_floor < 4) {
                    State new_state{this->items, new_floor, this->name_to_id};
                    new_state.items[i].floor = new_floor;
                    if(new_state.is_valid()){
                        states.push_back(new_state);
                    }
                }
            }
        }

        // Generate all pairs of items
        for (int i = 0; i < items.size(); ++i) {
            for (int j = i + 1; j < items.size(); ++j) {
                if(items[i].floor == items[j].floor && items[i].floor == elevator_position){
                    for (int direction : {-1, 1}) {
                        int new_floor = elevator_position + direction;
                        if (new_floor >= 0 && new_floor < 4) {
                            State new_state{this->items, new_floor, this->name_to_id};
                            new_state.items[i].floor = new_floor;
                            new_state.items[j].floor = new_floor;
                            if(new_state.is_valid()){
                                states.push_back(new_state);
                            }
                        }
                    }
                }
            }
        }
#if PRINT
        std::cout<<"Generated "<<states.size()<<" new states.\n";
#endif
        return states;
    }

    bool is_goal(){
        return std::all_of(items.begin(), items.end(), [](const Item& item) {
            return item.floor == 3;
        });
    }

};


struct StateHash {
    std::size_t operator()(const State& s) const noexcept {
        std::size_t h = std::hash<int>{}(s.elevator_position);

        for (const auto& item : s.items) {
            // Hash each part of the Item
            std::size_t item_hash = 0;
            hash_combine(item_hash, std::hash<int>{}(item.floor));
            hash_combine(item_hash, std::hash<int>{}(item.id));
            hash_combine(item_hash, std::hash<int>{}(static_cast<int>(item.type)));

            // Mix it into the State's hash
            hash_combine(h, item_hash);
        }
        return h;
    }

private:
    template <typename T>
    static void hash_combine(std::size_t& seed, const T& value) {
        seed ^= std::hash<T>{}(value) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
    }
};


int bfs(State initial_state){
    std::queue<std::pair<State, int>> queue;
    std::unordered_set<State, StateHash> visited;

    queue.push({initial_state, 0});
    visited.insert(initial_state);

    while (!queue.empty()) {
        State current = queue.front().first;
        int steps = queue.front().second;
        queue.pop();

        if (current.is_goal()) {
            return steps;
            break;
        }

        for (const auto& next : current.next_states()) {
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                queue.push({next, steps + 1});
            }
        }
    }
    return -1;
}



int main() {
    std::string linetxt;
    int p1 = 0, p2 = 0;

    // name -> id = name_to_id.left(name)
    // id -> name = name_to_id.right(id)
    boost::bimap<std::string, int> name_to_id;
    int id_counter = 0;
    int floor = 0;
    State initial_state{ {}, 0 };
    while(std::getline(std::cin, linetxt)){
        
        if(auto match = ctre::search<"The [a-zA-Z]+ floor contains nothing relevant.">(linetxt)){
            // Do nothing
        }else {
            for (auto match : ctre::search_all<"([a-zA-Z]+)-compatible microchip|([a-zA-Z]+) generator">(linetxt)) {
                std::string name;
                if (auto chip = match.get<1>(); chip) {
                    name = chip.to_string();
                    if(in_bimap(name_to_id, name)){
                        initial_state.items.push_back({ItemType::Microchip, get_from_bimap(name_to_id, name), floor});
                    }else{
                        initial_state.items.push_back({ItemType::Microchip, id_counter, floor});
                    }
                    
                } else if (auto gen = match.get<2>(); gen) {
                    name = gen.to_string();
                    if(in_bimap(name_to_id, name)){
                        initial_state.items.push_back({ItemType::Generator, get_from_bimap(name_to_id, name), floor});
                    }else{
                        initial_state.items.push_back({ItemType::Generator, id_counter, floor});
                    }
                }
                if(name_to_id.left.find(name) == name_to_id.left.end()) {
                    name_to_id.insert({name, id_counter++});
                }
            }
        }
        floor++;
    }

    initial_state.name_to_id = &name_to_id;
#if PRINT
    for (const auto& [name, id] : name_to_id.left) {
        std::cout << "Name: " << name << ", ID: " << id << "\n";
    }

    for(int i = 0; i < name_to_id.size(); i++){
        std::cout << "ID: " << i << ", Name: " << name_to_id.right.at(i) << "\n";
    }

    std::cout<<"\n\n\n\n";
    initial_state.print();
#endif


    p1 = bfs(initial_state);
    std::cout<<"Part 1: "<< p1 << "\n";


    name_to_id.insert({"elerium", id_counter++});
    name_to_id.insert({"dilithium", id_counter++});
    initial_state.elevator_position = 0;
    initial_state.items.push_back({ItemType::Generator, get_from_bimap(name_to_id, "elerium"), 0});
    initial_state.items.push_back({ItemType::Generator, get_from_bimap(name_to_id, "dilithium"), 0});
    initial_state.items.push_back({ItemType::Microchip, get_from_bimap(name_to_id, "elerium"), 0});
    initial_state.items.push_back({ItemType::Microchip, get_from_bimap(name_to_id, "dilithium"), 0});

    p2 = bfs(initial_state);
    std::cout<<"Part 2: "<< p2 << "\n";
}