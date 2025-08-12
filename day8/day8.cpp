#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include <ctre.hpp>

struct GRID{
    int width, height;
    std::vector<std::vector<char>> cells;

    GRID(int w, int h) : width(w), height(h), cells(h, std::vector<char>(w, '.')) {}
    void print() const {
        for (const auto& row : cells) {
            for (char cell : row) {
                std::cout << cell;
            }
            std::cout << "\n";
        }
    }

    void rotate_row(int row, int steps) {
        std::rotate(cells[row].rbegin(), cells[row].rbegin() + steps, cells[row].rend());
    }

    void rotate_column(int col, int steps) {
        std::vector<char> new_col(height);
        for (int y = 0; y < height; ++y) {
            new_col[(y + steps) % height] = cells[y][col];
        }
        for (int y = 0; y < height; ++y) {
            cells[y][col] = new_col[y];
        }
    }

    void rect(int width, int height) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cells[y][x] = '#';
            }
        }
    }

    int sum_pixels(){
        return std::accumulate(cells.begin(), cells.end(), 0, [](int sum, const std::vector<char>& row) {
            return sum + std::count(row.begin(), row.end(), '#');
        });
    }
};



int main() {
    std::string linetxt;
    GRID grid(50, 6);
    while(std::getline(std::cin, linetxt)) {
        if(auto match = ctre::match<"rect (\\d+)x(\\d+)">(linetxt)) {
            int width = match.get<1>().to_number<int>();
            int height = match.get<2>().to_number<int>();
            grid.rect(width, height);
        }else if(auto match = ctre::match<"rotate row y=(\\d+) by (\\d+)">(linetxt)) {
            int row = match.get<1>().to_number<int>();
            int by = match.get<2>().to_number<int>();
            grid.rotate_row(row, by);
        }else if(auto match = ctre::match<"rotate column x=(\\d+) by (\\d+)">(linetxt)) {
            int col = match.get<1>().to_number<int>();
            int by = match.get<2>().to_number<int>();
            grid.rotate_column(col, by);
        }else{
            std::cerr << "Unknown command: " << linetxt << "\n";
        }
    }

    std::cout<<"Part 1: "<<grid.sum_pixels()<<"\n";
    std::cout<<"Part 2: \n";
    grid.print();
}