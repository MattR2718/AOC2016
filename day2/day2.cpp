#include <iostream>
#include <string>
#include <ranges>
#include <numeric>
#include <vector>

#include <timer.h>

#if __has_include(<mdspan>)
  #include <mdspan>
  using std::mdspan;
#elif __has_include(<experimental/mdspan>)
  #include <experimental/mdspan>
  using std::experimental::mdspan;
#else
  #error "mdspan not available; please install Kokkos mdspan"
#endif


#include <ctre.hpp>

int main() {

    Timer::ScopedTimer t("Day 2");

    std::string linetxt;
    std::string p1{""};
    std::string p2{""};

    std::vector<std::string> keypad_nums {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    mdspan keypad{keypad_nums.data(), std::extents<size_t, 3, 3>{3, 3}};
    size_t x{1}, y{1};

    std::vector<std::string> keypad_nums_2 {".", ".", "1", ".", ".", 
                                          ".", "2", "3", "4", ".",
                                          "5", "6", "7", "8", "9",
                                          ".", "A", "B", "C", ".",
                                          ".", ".", "D", ".", "."
                                        };

    mdspan keypad_2{keypad_nums_2.data(), std::extents<size_t, 5, 5>{5, 5}};
    size_t x_2{0}, y_2{2};

    while(std::getline(std::cin, linetxt)){
        for(const char c : linetxt){
            switch(c) {
                case 'U':{ if (y > 0) --y; if(y_2 > 0 && keypad_2.at(y_2 - 1, x_2) != ".") --y_2; break;}
                case 'D':{ if (y < 2) ++y; if(y_2 < 4 && keypad_2.at(y_2 + 1, x_2) != ".") ++y_2; break;}
                case 'L':{ if (x > 0) --x; if(x_2 > 0 && keypad_2.at(y_2, x_2 - 1) != ".") --x_2; break;}
                case 'R':{ if (x < 2) ++x; if(x_2 < 4 && keypad_2.at(y_2, x_2 + 1) != ".") ++x_2; break;}
            }
        }
        p1 += keypad.at(y, x);
        p2 += keypad_2.at(y_2, x_2);
    }

    std::cout << "Part 1: " << p1 << "\n";
    std::cout << "Part 2: " << p2 << "\n";
}