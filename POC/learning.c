//
// Created by nora on 19/10/18.
//
#include <stdio.h>
#include "math.h"
#include <stdlib.h>
#include "learning.h"

Neurone initialize()
{
    Neurone n;

    n.i = 1;
    n.j = 1;
    n.i_k = 0.5 ;
    n.j_z = 0.5 ;
    n.k_z = 0.5 ;
    n.x_k = 0.5 ;
    n.x_z = 0.5 ;
    n.y_k = 0.5 ;
    n.y_z = 0.5 ;
    return n;
}

float sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

Neurone propagation(Neurone n, float x, float y)
{
    n.x = x;
    n.y = y;
    n.k = sigmoid(n.x * n.x_k + n.i * n.i_k + n.y * n.y_k);
    n.z = sigmoid(n.x * n.x_z + n.j * n.j_z + n.y * n.y_z + n.k * n.k_z);

    return n;
}

Neurone retro(Neurone n, float target)
{
    float errorZ = (target - n.z) * (n.z * (1 - n.z));

    float step = 1;

    n.x_z += step * n.x * errorZ;
    n.y_z += step * n.y * errorZ;
    n.k_z += step * n.k * errorZ;
    n.j_z += step * n.j * errorZ;

    float errorK = n.k * (1 - n.k) * errorZ * n.k_z;

    n.x_k += step * n.x * errorK;
    n.y_k += step * n.y * errorK;
    n.i_k += step * n.i * errorK;
    return n;
}

Neurone train(Neurone n)
{

    n = propagation(n, 1, 1);
    n = retro(n, 0);

    n = propagation(n, 0, 1);
    n = retro(n, 1);

    n = propagation(n, 0, 0);
    n = retro(n, 0);

    n = propagation(n, 1, 0);
    n = retro(n, 1);

    return n;
}
