#include "api.h"
#include <cmath>
#include <iostream>

int main() {
    std::cout << "Program 1: linked library\n" << "1 A B: area; 2 x: translation; EOF: exit\n";

    int command;
    while (std::cin >> command) {
        if (command == 1) {
            float A, B;
            if (!(std::cin >> A >> B)) {
                std::cerr << "Input error: expected two numbers\n";
                return 1;
            }
            if (!std::isfinite(A) || !std::isfinite(B) || A < 0 || B < 0) {
                std::cerr << "Input error: sides must be finite and nonnegative\n";
                return 1;
            }
            std::cout << Square(A, B) << '\n';
        } 
        else if (command == 2) {
            long x;
            if (!(std::cin >> x)) {
                std::cerr << "Input error: expected an integer in the long range\n";
                return 1;
            }
            std::cout << translation(x) << '\n';
        } 
        else std::cerr << "Unknown command\n";
    }
    if (!std::cin.eof()) {
        std::cerr << "Input error: expected a command number\n";
        return 1;
    }
    return 0;
}
