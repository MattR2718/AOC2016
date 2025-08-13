#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include <ctre.hpp>
#include <boost/bimap.hpp>

enum class ItemType{
    Microchip,
    Generator
};

struct Item{
    ItemType type;
    int id;

    std::string get_name(const boost::bimap<std::string, int>& name_to_id) const {
        return name_to_id.right.at(id);
    }
};

bool operator==(const Item& a, const Item& b) {
    return a.type == b.type && a.id == b.id;
}

struct State {
    std::vector<std::vector<Item>> floors;
    int elevator_position;

    bool valid_floor(std::vector<Item> floor) const {
        // Check for invalid item combinations
        std::vector<int> microchips;
        std::vector<int> generators;

        for (const auto& item : floor) {
            if (item.type == ItemType::Microchip) {
                microchips.push_back(item.id);
            } else {
                generators.push_back(item.id);
            }
        }

        // If there are no generators, the floor is valid
        if (generators.empty()) {
            return true;
        }

        // Check if all microchips have a corresponding generator
        for (const auto& chip : microchips) {
            if (std::find(generators.begin(), generators.end(), chip) == generators.end()) {
                return false;
            }
        }

        return true;
    }

    bool is_valid() const {
        for (const auto& floor : floors) {
            if (!valid_floor(floor)) {
                return false;
            }
        }
        return true;
    }

    /*
    std::vector<State> generate_next_states() const {
        std::vector<State> next_states;

        // Generate all possible moves
        for (int i = 0; i < floors.size(); ++i) {
            if (i == elevator_position) {
                // Generate moves for the current floor
                for (int j = 0; j < floors[i].size(); ++j) {
                    // Move item down one floor
                    if (i > 0) {
                        State new_state{this->floors, this->elevator_position};
                        new_state.floors[i].erase(new_state.floors[i].begin() + j);
                        new_state.floors[i - 1].push_back(floors[i][j]);
                        new_state.elevator_position--;
                        if (new_state.is_valid()) {
                            next_states.push_back(new_state);
                        }
                    }
                }

                for (int j = 0; j < floors[i].size(); ++j) {
                    // Move item up one floor
                    if (i < floors.size() - 1) {
                        State new_state{this->floors, this->elevator_position};
                        new_state.floors[i].erase(new_state.floors[i].begin() + j);
                        new_state.floors[i + 1].push_back(floors[i][j]);
                        new_state.elevator_position++;
                        if (new_state.is_valid()) {
                            next_states.push_back(new_state);
                        }
                    }
                }

                // Move two items up one floor
                for (int j = 0; j < floors[i].size(); ++j) {
                    for (int k = j + 1; k < floors[i].size(); ++k) {
                        if (i < floors.size() - 1) {
                            State new_state{this->floors, this->elevator_position};
                            new_state.floors[i].erase(new_state.floors[i].begin() + k);
                            new_state.floors[i].erase(new_state.floors[i].begin() + j);
                            new_state.floors[i + 1].push_back(floors[i][j]);
                            new_state.floors[i + 1].push_back(floors[i][k]);
                            new_state.elevator_position++;
                            if (new_state.is_valid()) {
                                next_states.push_back(new_state);
                            }
                        }
                    }
                }

                // Move two items down one floor
                for (int j = 0; j < floors[i].size(); ++j) {
                    for (int k = j + 1; k < floors[i].size(); ++k) {
                        if (i > 0) {
                            State new_state{this->floors, this->elevator_position};
                            new_state.floors[i].erase(new_state.floors[i].begin() + k);
                            new_state.floors[i].erase(new_state.floors[i].begin() + j);
                            new_state.floors[i - 1].push_back(floors[i][j]);
                            new_state.floors[i - 1].push_back(floors[i][k]);
                            new_state.elevator_position--;
                            if (new_state.is_valid()) {
                                next_states.push_back(new_state);
                            }
                        }
                    }
                }
            }
        }

        return next_states;
    }

    */

    std::vector<State> generate_next_states() const {
        std::vector<State> next_states;

        // Get the items on the current elevator floor
        const auto& items = floors[elevator_position];
        int floor_idx = elevator_position;

        // Helper lambda to try moving selected items to a target floor
        auto try_move = [&](const std::vector<int>& indices, int target_floor) {
            State new_state{floors, elevator_position};

            // To erase multiple indices safely, sort in descending order
            std::vector<int> sorted_indices = indices;
            std::sort(sorted_indices.rbegin(), sorted_indices.rend());

            // Remove items from current floor
            for (int idx : sorted_indices) {
                new_state.floors[floor_idx].erase(new_state.floors[floor_idx].begin() + idx);
            }
            // Add items to target floor
            for (int idx : indices) {
                new_state.floors[target_floor].push_back(items[idx]);
            }

            new_state.elevator_position = target_floor;

            if (new_state.is_valid()) {
                next_states.push_back(std::move(new_state));
            }
        };

        // Generate all 1-item moves
        for (int i = 0; i < (int)items.size(); ++i) {
            if (floor_idx > 0) try_move({i}, floor_idx - 1);
            if (floor_idx < (int)floors.size() - 1) try_move({i}, floor_idx + 1);
        }

        // Generate all 2-item moves (combinations of two different items)
        for (int i = 0; i < (int)items.size(); ++i) {
            for (int j = i + 1; j < (int)items.size(); ++j) {
                if (floor_idx > 0) try_move({i, j}, floor_idx - 1);
                if (floor_idx < (int)floors.size() - 1) try_move({i, j}, floor_idx + 1);
            }
        }

        // sort every floor of every state to make permutations easier to detect
        for (auto& state : next_states) {
            for (auto& floor : state.floors) {
                std::sort(floor.begin(), floor.end(), [](const Item& a, const Item& b) {
                    return a.id < b.id;
                });
            }
        }

        return next_states;
    }

    bool final_state() const {
        // Check if all items are on the top floor
        for (int i = 0; i < floors.size() - 1; ++i) {
            if (!floors[i].empty()) {
                return false;
            }
        }
        return true;
    }


    void print_state(const boost::bimap<std::string, int>& name_to_id) const {
        std::cout << "Elevator is on floor " << elevator_position << "\n";
        for (int i = floors.size() - 1; i >= 0; i--) {
            std::cout << "Floor " << i << ": ";
            for (const auto& item : floors[i]) {
                std::cout << "[" << item.get_name(name_to_id) << " " << (item.type == ItemType::Microchip ? "M" : "G") << "] ";
            }
            std::cout << "\n";
        }
    }

};

bool operator==(const State& a, const State& b) {
    return a.elevator_position == b.elevator_position &&
           a.floors == b.floors;
}

struct StateHash{
    size_t operator()(const State& state) const {
        size_t hash = std::hash<int>()(state.elevator_position);
        for (const auto& floor : state.floors) {
            for (const auto& item : floor) {
                hash ^= std::hash<int>()(item.id) ^ std::hash<int>()(static_cast<int>(item.type));
            }
        }
        return hash;
    }
};


int get_from_bimap(const boost::bimap<std::string, int>& name_to_id, const std::string& name) {
    auto it = name_to_id.left.find(name);
    if (it != name_to_id.left.end()) {
        return it->second;
    }
    return -1; // Not found
}

std::string get_name_from_bimap(const boost::bimap<std::string, int>& name_to_id, int id) {
    auto it = name_to_id.right.find(id);
    if (it != name_to_id.right.end()) {
        return it->second;
    }
    return ""; // Not found
}

bool in_bimap(const boost::bimap<std::string, int>& name_to_id, const std::string& name) {
    return name_to_id.left.find(name) != name_to_id.left.end();
}

bool in_bimap(const boost::bimap<std::string, int>& name_to_id, int id) {
    return name_to_id.right.find(id) != name_to_id.right.end();
}


// Generate all possible states that the floors can be in
std::vector<std::vector<std::vector<Item>>> generate_all_states(const std::vector<std::vector<Item>>& current_state, int elevator_position) {
    std::vector<std::vector<std::vector<Item>>> all_states;

    // Move every item individually up one floor
    for (int i = 0; i < current_state.size() - 1; ++i) {
        for(int j = 0; j < current_state[i].size(); j++){
            std::vector<std::vector<Item>> new_state;
        }

        
    }

    return all_states;
}







int main() {
    std::string linetxt;
    int p1 = 0, p2 = 0;

    // name -> id = name_to_id.left(name)
    // id -> name = name_to_id.right(id)
    boost::bimap<std::string, int> name_to_id;
    int id_counter = 0;
    
    std::vector<std::vector<Item>> floor_contents;
    while(std::getline(std::cin, linetxt)){
        std::vector<Item> floor;
        if(auto match = ctre::search<"The [a-zA-Z]+ floor contains nothing relevant.">(linetxt)){
            // Do nothing
        }else {
            for (auto match : ctre::search_all<"([a-zA-Z]+)-compatible microchip|([a-zA-Z]+) generator">(linetxt)) {
                std::string name;
                if (auto chip = match.get<1>(); chip) {
                    name = chip.to_string();
                    std::cout << "Found microchip: " << name << "\n";
                    if(in_bimap(name_to_id, name)){
                        floor.push_back({ItemType::Microchip, get_from_bimap(name_to_id, name)});
                    }else{
                        floor.push_back({ItemType::Microchip, id_counter});
                    }
                    
                } else if (auto gen = match.get<2>(); gen) {
                    name = gen.to_string();
                    std::cout << "Found generator: " << name << "\n";
                    if(in_bimap(name_to_id, name)){
                        floor.push_back({ItemType::Generator, get_from_bimap(name_to_id, name)});
                    }else{
                        floor.push_back({ItemType::Generator, id_counter});
                    }
                }
                if(name_to_id.left.find(name) == name_to_id.left.end()) {
                    name_to_id.insert({name, id_counter++});
                }
            }
        }
        floor_contents.push_back(floor);
    }

    for (const auto& [name, id] : name_to_id.left) {
        std::cout << "Name: " << name << ", ID: " << id << "\n";
    }

    for(int i = 0; i < name_to_id.size(); i++){
        std::cout << "ID: " << i << ", Name: " << name_to_id.right.at(i) << "\n";
    }

    //std::cout<<"lithium: " << name_to_id.left.at("lithium") << "\n";

    int floor_counter = 0;
    for (const auto& floor : floor_contents) {
        for (const auto& item : floor) {
            std::cout << "Floor " << floor_counter << ": " << item.get_name(name_to_id) << " " << (item.type == ItemType::Microchip ? "Microchip" : "Generator") << "\n";
        }
        floor_counter++;
    }


    std::cout<<"\n\n\n\n";

    int elevator_position = 0;
    State initial_state{floor_contents, elevator_position};
    //std::queue<std::tuple<State, int, std::vector<State>>> states_to_explore;
    //states_to_explore.push({initial_state, 0, {initial_state}});

    std::queue<std::tuple<State, int>> states_to_explore;
    states_to_explore.push({initial_state, 0});

    std::vector<State> visited_states;
    std::vector<State> state_path;
    

    while (!states_to_explore.empty()) {
        if(states_to_explore.size() % 1000 == 0){
            std::cout<<"States to explore: " << states_to_explore.size() << "  States Visited: " << visited_states.size() << "\n";
        }
        State current_state = std::get<0>(states_to_explore.front());
        int steps = std::get<1>(states_to_explore.front());
        //std::vector<State> previous_states = std::get<2>(states_to_explore.front());

        // Check if we have already visited this state
        if (std::find(visited_states.begin(), visited_states.end(), current_state) != visited_states.end()) {
            states_to_explore.pop();
            continue;
        }

        visited_states.push_back(current_state);

        if(current_state.final_state()){
            p1 = steps;
            //state_path = previous_states;
            break;
        }
        states_to_explore.pop();

        // Generate next states
        std::vector<State> next_states = current_state.generate_next_states();
        for (auto& state : next_states) {
            //auto new_history = previous_states;
            //new_history.push_back(state);
            //states_to_explore.push({state, steps + 1, new_history});
            states_to_explore.push({state, steps + 1});
        
        }
    }


    // std::cout<<"State Path: \n";
    // for (const auto& state : state_path) {
    //     std::cout<<"\n";
    //     state.print_state(name_to_id);
    //     std::cout<<"\n";
        
    // }

    std::cout<<"Part 1: "<< p1 << "\n";
    std::cout<<"Part 2: "<< p2 << "\n";
}