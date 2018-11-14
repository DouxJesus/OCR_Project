#include <stdio.h>
#include "learning.h"
#include <stdlib.h>

int main() {
    //INITIALISATION
    Neurone xor = initialize();

    //TRAINING
    for (int i = 0; i < 42000; i++)
    {
        xor = train(xor);
    }

    //PRINT RESULTS
    xor = propagation(xor, 0, 0);
    printf("%f + %f = %f\n", xor.x, xor.y, xor.z);
    xor = propagation(xor, 0, 1);
    printf("%f + %f = %f\n", xor.x, xor.y, xor.z);
    xor = propagation(xor, 1, 0);
    printf("%f + %f = %f\n", xor.x, xor.y, xor.z);
    xor = propagation(xor, 1, 1);
    printf("%f + %f = %f\n", xor.x, xor.y, xor.z);

    return 0;
}
