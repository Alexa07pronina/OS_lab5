#include "api.h"
#include <climits>

float LAB5_CALL Square(float A, float B) {
    return A * B;
}

char* LAB5_CALL translation(long x) {
    static char buffer[sizeof(long) * CHAR_BIT + 2];
    int position = sizeof(buffer) - 1;
    buffer[position] = '\0';

    unsigned long number = static_cast<unsigned long>(x);
    if (x < 0) number = 0 - number;

    do {
        buffer[--position] = '0' + number % 2;
        number /= 2;
    } 
    while (number != 0);

    if (x < 0) buffer[--position] = '-';
    return buffer + position;
}
