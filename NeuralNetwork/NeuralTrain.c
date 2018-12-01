#include "NeuralCreate.h"
#include "math.h"

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

void initializeTargets(char *target[])
{
    for(int i = 0; i < 52; i++)
    {
        char translate[52];
        for (int k = 0; k < 52; k++)
        {
            if (k == i)
            {
                translate[k] = 1;
            }
            else
            {
                translate[k] = 0;
            }
        }
        target[i] = translate;
    }
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

void propagation(Network network)
{
    for (int i = network.layers[0]; i < network.graphlen; i++)
    {
        network.graph[i].val = sigmoid(sum(network.graph[i], network));
    }
}

void retro(Network network, char target, double step)
{
    for(int i = 1; i < network.laylenght; i++)
    {
        int nbCurrent = network.layers[i];
        int nbPrev = network.layers[i - 1];
        for (int  k = 0; k < nbCurrent; k++)
        {
            for(int j = 0; j < nbPrev; j++)
            {
                int indSrc = j;
                if (i > 1)
                {
                    indSrc += network.layers[i - 2];
                }
                double cost = newCost(network, network.graph[indSrc], network.graph[k + i - 1], step, target, i == (network.laylenght - 1));
                network.graph[k].predes[j].cost = cost;
            }
        }
    }
}

void main()
{
    return 0;
}

/*void train(Network network)
{
    char target[52];
    Neural f = network.graph[1];
    f.predes
    initialzeTargets(target);
}*/
