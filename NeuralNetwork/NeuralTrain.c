#include "NeuralCreate.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>


//**********************************************************************************
//  INITIALZE
//**********************************************************************************

char *initialzeResult(int numberCharacters)
{
    char *characters = malloc(numberCharacters * sizeof(char));
    for(char i = 'a'; (i - 'a') < numberCharacters && i <= 'z'; i++)
    {
        characters[i - 'a'] = i;
    }
    for(char k = 'A'; (k - 'A' + 'z' - 'a' + 1) < numberCharacters && k <= 'Z'; k++)
    {
        characters[k - 'A' + 'z' - 'a' + 1] = k;
    }
    return  characters;
}

double** initializeTargets(int numberCharacters)
{
    double **target;
    target = malloc(numberCharacters * sizeof(double*));
    if (!target)
    {
        exit(1);
    }
    for(int i = 0; i < numberCharacters; i++)
    {
        target[i] = malloc(numberCharacters * sizeof(double));
        if(!target[i])
            exit(1);
        for (int k = 0; k < numberCharacters; k++)
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

//**********************************************************************************
//  USEFUL FUNCTIONS
//**********************************************************************************

double sigmoid(double x)
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

//**********************************************************************************
//  LEARNING FORMULA
//**********************************************************************************

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
    if (isOut)
    {
        error = (target - neural.val)* neural.val * ( 1- neural.val);
    }
    else
    {
        error = neural.val * (1 - neural.val);
        double sum = 0;
        for (int i = 0; i < neural.slenght; i++)
        {
            sum += neural.sucess[i].cost * network.graph[neural.sucess[i].pos].error;
        }
        error *= sum;
    }
    neural.error = error;
    return error;
}

double newCost(Network network, Neural source, Neural dest, double step, double target, int isOut)
{
    return step * error(network, dest, target, isOut) * source.val;
}

//**********************************************************************************
//  PROPAGATION / RETROPROPAGATION
//**********************************************************************************

void propagation(Network network, double* inputs)
{
    for(int j = 1; j < network.layers[0]; j++)
    {
        network.graph[j].val = inputs[j - 1];
    }
    for (int i = network.layers[0]; i < network.graphlen; i++)
    {
        if(network.graph[i].plenght != 0)
        {
            network.graph[i].val = sigmoid(sum(network.graph[i], network));
        }
    }
}

void retro(Network network, double* target, double step)
{
    int pastNode = network.layers[0];
    for(int i = 1; i < network.laylenght; i++)
    {
        int nbCurrent = network.layers[i];
        for (int  k = 0; k < nbCurrent; k++)
        {
            for(int j = 0; j < network.graph[pastNode + k].plenght; j++)
            {
                double cost = newCost(network, network.graph[network.graph[k + pastNode].predes[j].pos],
                        network.graph[k + pastNode], step, target[k], i == (network.laylenght - 1));
                if (i == (network.laylenght - 1))
                {
                    cost = newCost(network, network.graph[network.graph[k + pastNode].predes[j].pos],
                                   network.graph[k + pastNode], step, target[k], i == (network.laylenght - 1));
                }
                else
                {
                    cost = newCost(network, network.graph[network.graph[k + pastNode].predes[j].pos],
                                   network.graph[k + pastNode], step, 0, i == (network.laylenght - 1));
                }
                network.graph[k + pastNode].predes[j].cost += cost;
            }
        }
        pastNode += network.layers[i];
    }
}

//**********************************************************************************
//  RESULT
//**********************************************************************************

char readResult(Network network, int numberCharacters)
{
    int index = 0;
    for(int j = 0; j < network.laylenght - 1; j++)
    {
        index += network.layers[j];
    }
    index++;
    int max = index;
    for(int i = index + 1; i < index + network.layers[network.laylenght - 1]; i++)
    {
        if (network.graph[i].val > network.graph[max].val)
        {
            max = i;
        }
    }
    char *characters = initialzeResult(numberCharacters);
    return characters[max - index];
}


//**********************************************************************************
//  TRAINING FUNCTION
//**********************************************************************************

void train(Network network, double** exercices, double** targets, int numberCharacters)
{
    for(int k = 0; k < 10000; k++)
    {
        printf("####################################\n");
        for(int j = 0; j < numberCharacters; j++)
        {
            propagation(network, exercices[j]);
            printf("Result  = ");
            int index = 0;
            for(int l = 0; l < network.laylenght - 1; l++)
            {
                index += network.layers[l];
            }
            for(int i = index + 1; i < index + network.layers[network.laylenght - 1]; i++)
            {
                printf("Node(%i) = %f ", i, network.graph[i].val);
            }
            printf("\nReturns %c\n", readResult(network, numberCharacters));
            for(int p = 0; p < 4; p++)
            {
                printf("The target must be %i\n ", (int) targets[j][p]);
            }
            retro(network, targets[j], 0.1);
        }
    }
}




//**********************************************************************************
//
//**********************************************************************************

int main()
{
    int NUMBERLAYER = 4;
    int NUMBERCHARACTERS = 4;
    int NUMBERELEMENTS = 9;

    int* layers = malloc(NUMBERLAYER * sizeof(int));
    if(!(layers))
    {
        exit(-1);
    }
    layers[0] = NUMBERELEMENTS + 1;
    layers[1] = 100;
    layers[2] = 100;
    layers[3] = NUMBERCHARACTERS + 1;

    int laylen = NUMBERLAYER;

    Network net = CreateNetwork(layers, laylen);


    double** inputs;
    inputs = malloc(NUMBERCHARACTERS * sizeof(double*));
    if (!inputs)
    {
        exit(1);
    }
    inputs[0] = malloc(net.layers[0] * sizeof(double));
    inputs[0][0] = 1;
    inputs[0][1] = 0;
    inputs[0][2] = 1;
    inputs[0][3] = 0;
    inputs[0][4] = 0;
    inputs[0][5] = 0;
    inputs[0][6] = 1;
    inputs[0][7] = 0;
    inputs[0][8] = 1;
    inputs[1] = malloc(net.layers[0] * sizeof(double));
    inputs[1][0] = 1;
    inputs[1][1] = 0;
    inputs[1][2] = 0;
    inputs[1][3] = 0;
    inputs[1][4] = 1;
    inputs[1][5] = 0;
    inputs[1][6] = 0;
    inputs[1][7] = 0;
    inputs[1][8] = 1;
    inputs[2] = malloc(net.layers[0] * sizeof(double));
    inputs[2][0] = 1;
    inputs[2][1] = 0;
    inputs[2][2] = 0;
    inputs[2][3] = 1;
    inputs[2][4] = 0;
    inputs[2][5] = 0;
    inputs[2][6] = 1;
    inputs[2][7] = 0;
    inputs[2][8] = 0;
    inputs[3] = malloc(net.layers[0] * sizeof(double));
    inputs[3][0] = 0;
    inputs[3][1] = 1;
    inputs[3][2] = 0;
    inputs[3][3] = 1;
    inputs[3][4] = 1;
    inputs[3][5] = 1;
    inputs[3][6] = 0;
    inputs[3][7] = 1;
    inputs[3][8] = 0;



    double** target = initializeTargets(NUMBERCHARACTERS);


    train(net, inputs, target, NUMBERCHARACTERS);

    /*printf("The recognize character isss : %c.\n", readResult(net, NUMBERCHARACTERS));
    propagation(net, inputs[0]);*/

    printf("\n");
    for(int index = 0; index < NUMBERCHARACTERS; index++)
    {
        for(int yo = 0; yo < NUMBERELEMENTS; yo++)
        {
            printf("%i ", (int) inputs[index][yo]);
        }
        printf("\n");

        printf("Result  = ");
        int mar = 0;
        for(int g = 0; g < net.laylenght - 1; g++)
        {
            mar += net.layers[g];
        }
        for(int v = mar + 1; v < mar + net.layers[net.laylenght - 1]; v++)
        {
            printf("Node(%i) = %f ", v, net.graph[v].val);
        }
        printf("\n");
        propagation(net, inputs[index]);
        printf("The recognize character is : %c.\n", readResult(net, NUMBERCHARACTERS));
    }


    SaveNetwork(net);

    free(inputs);
    free(target);
    free(layers);

    return 0;
}

