#ifndef LAB5_CONSOLE_H
#define LAB5_CONSOLE_H

#include "api.h"
#include <cmath>
#include <iostream>

inline bool execute_command(int command, SquareFunction square, TranslationFunction translate) {
    if (command == 1) {
        float A, B;
        if (!(std::cin >> A >> B)) {
            std::cerr << "Input error: expected two numbers\n";
            return false;
        }
        if (!std::isfinite(A) || !std::isfinite(B) || A < 0 || B < 0) {
            std::cerr << "Input error: sides must be finite and nonnegative\n";
            return false;
        }
        std::cout << square(A, B) << '\n';
    } 
    else if (command == 2) {
        long x;
        if (!(std::cin >> x)) {
            std::cerr << "Input error: expected an integer in the long range\n";
            return false;
        }
        std::cout << translate(x) << '\n';
    } 
    else std::cerr << "Unknown command\n";
    return true;
}

#endif
