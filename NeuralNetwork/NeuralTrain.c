#include "NeuralCreate.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

void initialzeResult(char target[])
{
    for(char i = 'a'; i <= 'z'; i++)
    {
        target[i - 'a'] = i;
    }
    for(char k = 'A'; k <= 'Z'; k++)
    {
        target[k - 'A'] = k;
    }
}

double** initializeTargets()
{
    double **target;
    target = malloc(52 * sizeof(double*));
    if (!target)
    {
        exit(1);
    }
    for(int i = 0; i < 52; i++)
    {
        target[i] = malloc(52 * sizeof(double));
        if(!target[i])
            exit(1);
        for (int k = 0; k < 52; k++)
        {
            if (k == i)
            {
                target[i][k] = 1;
            }
            else
            {
                target[i][k] = 0;
            }
        }
    }
    return target;
}



float sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

Neural indexToNeural(Network network, int couche ,int unit)
{
    int index = 0;
    for (int i = 0; i < couche; i++)
    {
        index += network.layers[i];
    }
    index += unit;
    Neural current = network.graph[index];
    return current;
}

double sum(Neural neural, Network network)
{
    double res = 0;
    for (int i = 0; i < neural.plenght; i++)
    {
        res += neural.predes[i].cost * network.graph[neural.predes[i].pos].val;
    }
    return res;
}

double error(Network network, Neural neural, double target, int isOut)
{
    double error;
    if (isOut == 1)
    {
        error = (target - neural.val)* neural.val * ( 1- neural.val);
    }
    else
    {
        error = neural.val * (1 - neural.val);
        for (int i = 0; i < neural.slenght; i++)
        {
            error += neural.sucess[i].cost * network.graph[neural.sucess[i].pos].val;
        }
    }
    return error;
}

double newCost(Network network, Neural source, Neural dest, double step, double target, int isOut)
{
    return step * error(network, dest, target, isOut) * source.val;
}

void propagation(Network network, double* inputs)
{
    for(int j = 0; j < network.layers[0]; j++)
    {
        network.graph[j].val = inputs[j];
    }
    for (int i = network.layers[0]; i < network.graphlen; i++)
    {
        network.graph[i].val = sigmoid(sum(network.graph[i], network));
    }
}

void retro(Network network, double* target, double step)
{
    int pastNode = 0;
    printf("LAYER LENGTH = %i\n", network.laylenght);
    printf("########################################################\n");
    for(int i = 1; i < network.laylenght; i++)
    {
        int nbCurrent = network.layers[i];
        printf("COURANT = %i\n", nbCurrent);
        int nbPrev = network.layers[i - 1];
        printf("PREVIOUS = %i\n", nbPrev);
        for (int  k = 0; k < nbCurrent; k++)
        {
            printf("k = %i ; NB PREDES = %i\n", k + pastNode, network.graph[pastNode + k].plenght);
            for(int m = 0; m < network.graph[pastNode + k].plenght; m++)
            {
                printf("     pred %i = %d\n", m, network.graph[pastNode + k].predes[m].pos);
            }
            printf("     *********************\n");
            for(int j = 0; j < nbPrev; j++)
            {
                int indSrc = pastNode + j - network.layers[i - 1];
                printf("     source = %i\n", indSrc);
                double cost = newCost(network, network.graph[indSrc], network.graph[k + pastNode], step, target[k], i == (network.laylenght - 1));

                /*for(int l = 0; l < network.graph[k].plenght; l++)
                {
                    printf("%f\n", network.graph[k].predes[l]);
                }*/
                //network.graph[k].predes[j].cost = cost;
            }
        }
        pastNode += network.layers[i];
        printf("########################################################\n");
    }
}

int main()
{
    int* layers;
    if(!(layers = malloc(4 * sizeof(int))))
    {
        exit(-1);
    }
    layers[0] = 4;
    layers[1] = 5;
    layers[2] = 3;
    layers[3] = 5;

    int laylen = 4;
    Network net = CreateNetwork(layers, laylen);
    double** inputs;
    inputs = malloc(52 * sizeof(double*));
    if (!inputs)
    {
        exit(1);
    }
    for(int l = 0; l < 52; l++)
    {
        inputs[l] = malloc(net.layers[0] * sizeof(double));
        if(!inputs[l])
            exit(1);
        for (int j = 0; j < 4; j++)
        {
            if (j == (l % 4))
            {
                inputs[l][j] = 1;
            }
            else
            {
                inputs[l][j] = 0;
            }
        }
    }
    double** target = initializeTargets();
    printf("%d\n", net.laylenght);
    for(int k = 0; k < 3; k++)
    {
        for(int j = 0; j < 52; j++)
        {
            propagation(net, inputs[j]);

            retro(net, target[j], 0.1);
        }
    }
    SaveNetwork(net);
    /*for(int i = 0; i < 52; i++)
    {
        for(int j = 0; j < 52; j++)
        {
            printf("%x", (int) target[i][j]);
        }
        printf("\n");
    }*/
    free(inputs);
    free(target);
    free(layers);
    return 0;
}

/*void train(Network network)
{
    char target[52];
    Neural f = network.graph[1];
    f.predes
    initialzeTargets(target);
}*/
