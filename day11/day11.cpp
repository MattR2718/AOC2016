#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#include <ctre.hpp>
#include <boost/bimap.hpp>

#include <timer.h>

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

std::string get_from_bimap(const boost::bimap<std::string, int>& name_to_id, int id) {
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
    int elevator;
    std::vector<std::pair<int, int>> chip_floor_gen_floor;

    void print(boost::bimap<std::string, int>& name_to_id) const {
        std::cout << "Elevator is on floor: " << elevator << "\n";
        int i = 0;
        for (const auto& [chip_floor, gen_floor] : chip_floor_gen_floor) {
            std::cout << get_from_bimap(name_to_id, i++) << ": \n";
            std::cout << "\tChip is on floor: " << chip_floor << ", Generator is on floor: " << gen_floor << "\n";
        }
    }

    bool is_valid() const {
        std::vector<int> gen_floors(4, 0);
        for (const auto& [chip_floor, gen_floor] : chip_floor_gen_floor) {
            gen_floors[gen_floor]++;
        }
        for (const auto& [chip_floor, gen_floor] : chip_floor_gen_floor) {
            if (chip_floor != -1 && gen_floor != -1 && chip_floor != gen_floor && gen_floors[chip_floor] > 0) {
                return false;
            }
        }
        return true;
    }

    std::vector<State> get_next_states() {
        std::vector<State> next_states;

        // Collect indexes of chips and generators on the current elevator floor
        std::vector<int> chip_on_floor, gen_on_floor;
        for (int i = 0; i < chip_floor_gen_floor.size(); i++) {
            const auto& [chip_floor, gen_floor] = chip_floor_gen_floor[i];
            if (chip_floor == elevator) chip_on_floor.push_back(i);
            if (gen_floor == elevator) gen_on_floor.push_back(i);
        }

        // Function to try moving a set of items by delta floors
        auto try_move = [&](const std::vector<std::pair<int, bool>>& items, int delta) {
            State new_state = *this;
            new_state.elevator += delta;
            for (auto [index, is_chip] : items) {
                if (is_chip) new_state.chip_floor_gen_floor[index].first += delta;
                else         new_state.chip_floor_gen_floor[index].second += delta;
            }
            if (new_state.is_valid()) next_states.push_back(new_state);
        };

        // Generate all combinations of 1 or 2 items to move
        auto generate_moves = [&](const std::vector<int>& indexes, bool is_chip) {
            // Single item moves
            for (int idx : indexes) {
                if (elevator < 3) try_move({{idx, is_chip}}, 1);
                if (elevator > 0) try_move({{idx, is_chip}}, -1);
            }
            // Double item moves
            for (size_t i = 0; i < indexes.size(); i++) {
                for (size_t j = i + 1; j < indexes.size(); j++) {
                    if (elevator < 3) try_move({{indexes[i], is_chip}, {indexes[j], is_chip}}, 1);
                    if (elevator > 0) try_move({{indexes[i], is_chip}, {indexes[j], is_chip}}, -1);
                }
            }
        };

        // Move chips and generators
        generate_moves(chip_on_floor, true);
        generate_moves(gen_on_floor, false);

        // Move chip-generator pairs that are together
        for (int i = 0; i < chip_floor_gen_floor.size(); i++) {
            const auto& [chip_floor, gen_floor] = chip_floor_gen_floor[i];
            if (chip_floor == gen_floor && chip_floor == elevator) {
                if (elevator < 3) try_move({{i, true}, {i, false}}, 1);
                if (elevator > 0) try_move({{i, true}, {i, false}}, -1);
            }
        }

        return next_states;
    }


    bool is_goal() const {
        return std::all_of(chip_floor_gen_floor.begin(), chip_floor_gen_floor.end(), [](const auto& pair) {
            return pair.first == 3 && pair.second == 3;
        });
    }
};

bool operator==(const State& lhs, const State& rhs) {
    auto sorted_lhs = lhs.chip_floor_gen_floor;
    auto sorted_rhs = rhs.chip_floor_gen_floor;
    std::sort(sorted_lhs.begin(), sorted_lhs.end());
    std::sort(sorted_rhs.begin(), sorted_rhs.end());
    return lhs.elevator == rhs.elevator && sorted_lhs == sorted_rhs;
}

struct StateHash {
    std::size_t operator()(const State& state) const {
        std::size_t hash = std::hash<int>()(state.elevator);
        auto sorted = state.chip_floor_gen_floor;
        std::sort(sorted.begin(), sorted.end());
        for (const auto& [chip, gen] : sorted) {
            hash ^= std::hash<int>()(chip) + 0x9e3779b9 + (hash<<6) + (hash>>2);
            hash ^= std::hash<int>()(gen)  + 0x9e3779b9 + (hash<<6) + (hash>>2);
        }
        return hash;
    }
};


int bfs(State initial_state){

    std::unordered_set<State, StateHash> visited;
    std::queue<std::pair<State, int>> q;
    q.push({initial_state, 0});

    while (!q.empty()) {
        auto [current_state, steps] = q.front();
        q.pop();

        if (current_state.is_goal()) {
            return steps;
        }

        if (visited.count(current_state) > 0) {
            continue;
        }
        visited.insert(current_state);

        for (const auto& next_state : current_state.get_next_states()) {
            if (visited.count(next_state) == 0) {
                q.push({next_state, steps + 1});
            }
        }
    }

    return -1;
}



int reconstruct_path(const std::unordered_map<State, State, StateHash>& start_parents,
                    const std::unordered_map<State, State, StateHash>& end_parents,
                    const State& meeting_point) {
    std::vector<State> path;

    // Reconstruct path from start to meeting point
    for (State curr = meeting_point; start_parents.count(curr) > 0; curr = start_parents.at(curr)) {
        path.push_back(curr);
    }
    std::reverse(path.begin(), path.end());

    // Reconstruct path from end to meeting point
    for (State curr = meeting_point; end_parents.count(curr) > 0; curr = end_parents.at(curr)) {
        path.push_back(curr);
    }

    return path.size();
}

// Search for the shortest path using bidirectional BFS
int bi_bfs(State initial_state){
    State end_state{ 3, std::vector<std::pair<int, int>>(initial_state.chip_floor_gen_floor.size(), {3, 3}) };

    std::unordered_set<State, StateHash> vs, ve;
    std::queue<std::pair<State, int>> qs, qe;
    qs.push({initial_state, 0});
    qe.push({end_state, 1});
    vs.insert(initial_state);
    ve.insert(end_state);

    std::unordered_map<State, int, StateHash> state_steps;
    state_steps[initial_state] = 0;
    state_steps[end_state] = 1;

    while(!qs.empty() && !qe.empty()) {
        if(qs.size() <= qe.size()){
            auto [cur_state, steps] = qs.front();
            qs.pop();

            for (const auto& neighbour : cur_state.get_next_states()) {
                if (vs.count(neighbour)) continue;
                if(ve.count(neighbour)) return steps + state_steps[neighbour];
                vs.insert(neighbour);
                qs.push({neighbour, steps + 1});
                state_steps[neighbour] = steps + 1;
            }
        } else {
            auto [cur_state, steps] = qe.front();
            qe.pop();

            for (const auto& neighbour : cur_state.get_next_states()) {
                if (ve.count(neighbour)) continue;
                if(vs.count(neighbour)) return steps + state_steps[neighbour];
                ve.insert(neighbour);
                qe.push({neighbour, steps + 1});
                state_steps[neighbour] = steps + 1;
            }
        }
    }

    return -1;
}

int main() {
    Timer::ScopedTimer t("Day 11");

    std::string linetxt;
    int p1 = 0, p2 = 0;

    // name -> id = name_to_id.left(name)
    // id -> name = name_to_id.right(id)
    boost::bimap<std::string, int> name_to_id;
    int id_counter = 0;
    int floor = 0;
    State initial_state{ 0, {} };
    while(std::getline(std::cin, linetxt)){
        
        if(auto match = ctre::search<"The [a-zA-Z]+ floor contains nothing relevant.">(linetxt)){
            // Do nothing
        }else {
            for (auto match : ctre::search_all<"([a-zA-Z]+)-compatible microchip|([a-zA-Z]+) generator">(linetxt)) {
                std::string name;
                if (auto chip = match.get<1>(); chip) {
                    name = chip.to_string();
                    if(in_bimap(name_to_id, name)){
                        initial_state.chip_floor_gen_floor[get_from_bimap(name_to_id, name)].first = floor;
                    }else{
                        initial_state.chip_floor_gen_floor.emplace_back(std::make_pair(floor, -1));
                    }
                    
                } else if (auto gen = match.get<2>(); gen) {
                    name = gen.to_string();
                    if(in_bimap(name_to_id, name)){
                        initial_state.chip_floor_gen_floor[get_from_bimap(name_to_id, name)].second = floor;
                    }else{
                        initial_state.chip_floor_gen_floor.emplace_back(std::make_pair(-1, floor));
                    }
                }
                if(name_to_id.left.find(name) == name_to_id.left.end()) {
                    name_to_id.insert({name, id_counter++});
                }
            }
        }
        floor++;
    }

    //initial_state.name_to_id = &name_to_id;
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

    p1 = bi_bfs(initial_state);
    std::cout<<"Part 1: "<< p1 << "\n";


    name_to_id.insert({"elerium", id_counter++});
    name_to_id.insert({"dilithium", id_counter++});
    initial_state.elevator = 0;
    initial_state.chip_floor_gen_floor.push_back({0, 0});
    initial_state.chip_floor_gen_floor.push_back({0, 0});

    p2 = bi_bfs(initial_state);
    std::cout<<"Part 2: "<< p2 << "\n";
}