#include <stdio.h>
#include "learning.h"

int main() {
    Neurone xor;
    initialize(xor);
    for (int i = 0; i < 10000; i++)
    {
        propagation(xor);
        retro(xor);
    }
    printf("%f + %f = %f", xor.x, xor.y, xor.z);
    return 0;
}