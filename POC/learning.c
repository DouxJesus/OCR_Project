//
// Created by nora on 19/10/18.
//

#include "learning.h"

void initialize(Neurone n)
{
    n.x = 1;
    n.y = 0;
    n.target = 1;

    n.i = 1;
    n.j = 1;
    n.i_k = (float) rand();
    n.j_z = (float) rand();
    n.k_z = (float) rand();
    n.x_k = (float) rand();
    n.x_z = (float) rand();
    n.y_k = (float) rand();
    n.y_z = (float) rand();
}

float sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

void propagation(Neurone n)
{
    n.k = sigmoid(n.x * n.x_k + n.i * n.i_k + n.y * n.y_k);
    n.z = sigmoid(n.x * n.x_z + n.j * n.j_z + n.y * n.y_z);
}

void retro(Neurone n)
{
    float errorZ = (n.t - n.z) * n.z * (1 - n.z);
    float errorK = n.k * (1 - n.k) * errorZ * n.k_z;

    float step = 0.1;

    n.x_z = step * n.x * n.z;
    n.y_z = step * n.y * n.z;
    n.k_z = step * n.k * n.z;
    n.j_z = step * n.j * n.z;

    n.x_k = step * n.x * n.k;
    n.y_k = step * n.y * n.k;
    n.i_k = step * n.i * n.k;
}