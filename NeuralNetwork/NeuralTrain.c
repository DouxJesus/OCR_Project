#include "NeuralCreate.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

int NUMBERLAYER = 4;
int NUMBERCHARACTERS = 4;
int NUMBERELEMENTS = 9;


typedef struct TupleTest{
    double newCost;
    double error;
}TupleTest;

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


//Calcul  de la valeur d'activation
double sum(Neural neural, Network network)
{
    double res = 0;
    //printf("%i\n", neural.plenght);
    for (int i = 0; i < neural.plenght; i++)
    {
        //printf("%f - %i\n", neural.predes[i].cost, neural.predes[i].pos);
        res += neural.predes[i].cost * network.graph[neural.predes[i].pos].val; //somme de tt les predes*cout 
    }
    return res;
}

double error(Network network, Neural neural, double target, int isOut)
{
    double error;
    if (isOut)
    {
        error = (target - neural.val)* neural.val * (1 - neural.val);
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

TupleTest newCost(Network network, Neural source, Neural dest, double step, double target, int isOut)
{
    TupleTest t;
    t.error = error(network, dest, target, isOut);
    t.newCost = step * t.error * source.val;
    return t;
}
//**********************************************************************************
//  PROPAGATION / RETROPROPAGATION
//**********************************************************************************

Network propagation(Network network, double* inputs)
{
    for(int j = 1; j < network.layers[0]; j++)
    {
        network.graph[j].val = inputs[j - 1]; //on place les valeurs dans la couche d'entrée
    }
    for (int i = network.layers[0]; i < network.graphlen; i++)
    {
        if(network.graph[i].plenght != 0)
        {
            network.graph[i].val = sigmoid(sum(network.graph[i], network));
        }
    }
    return network;
}

Network retro(Network network, double* target, double step)
{
    int currentLayer = network.laylenght - 1;
    int nbCurrent = network.graphlen - network.layers[currentLayer];
    const int lastLayer = currentLayer;

    for (int i = network.graphlen - 1; i >= 0; i--)
    {
        Neural currentNeural = network.graph[i];
        int count = currentNeural.plenght;
        for (int j= 0; j < count; ++j)
        {
            double cost;
            if(currentLayer == lastLayer)
            {
            //cout 
            //cost = newCost(network, predes du noeud courant,
            //                       noeud courant, step, target courant, dernière couche ? bool);
                int predes_idx = currentNeural.predes[j].pos;
                TupleTest t = newCost(network, network.graph[predes_idx], currentNeural, step, target[i - nbCurrent - 1], 1);
                cost = t.newCost;
                network.graph[i].error = t.error;
            }
            else
            {
                //into a layer
                int predes_idx = currentNeural.predes[j].pos;
                TupleTest t = newCost(network, network.graph[predes_idx], currentNeural, step, 0, 0);
                cost = t.newCost;
                network.graph[i].error = t.error;
            }
            network.graph[i].predes[j].cost = cost;
            //network.graph[predes_idx].sucess[]
            //network.graph[i].sucess[j].cost = cost;
        }
        if(i == nbCurrent)
        {
            //switch layer
            currentLayer--;
            nbCurrent-= network.layers[currentLayer];
        }
    }
    return network;
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
    for(int i = index; i < network.graphlen; i++)
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

Network train(Network network, double** exercices, double** targets, int numberCharacters)
{
    for(int k = 0; k < 100; k++)
    {
        for(int j = 0; j < numberCharacters; j++)
        {
            /*for(int s = 0; s < 4; s++)
            {
                printf("%f ", targets[j][s]);
            }
            printf("\n");*/
            network = propagation(network, exercices[j]);
            network = retro(network, targets[j], 0.1);
            int index = 0;
            for(int j = 0; j < network.laylenght - 1; j++)
            {
                index += network.layers[j];
            }
        }
    }
    return network;
}

char runNeural(Network network, double* input)
{
    propagation(network, input);
    return readResult(network, NUMBERCHARACTERS);
}



//**********************************************************************************
//
//**********************************************************************************

int main()
{

    int* layers = malloc(NUMBERLAYER * sizeof(int));
    if(!(layers))
    {
        exit(-1);
    }
    layers[0] = NUMBERELEMENTS + 1;
    layers[1] = 50;
    layers[2] = 50;
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

    // for (int i = 0; i < net.graphlen; ++i)
    // {
    //     Neural n = net.graph[i];
    //     for (int i = 0; i < n.plenght; ++i)
    //     {
    //         printf("%i - %f \n", n.predes[i].pos, n.predes[i].cost);
    //     }
    //     printf("\n");
    // }

    net = train(net, inputs, target, NUMBERCHARACTERS);

    SaveNetwork(net);
    FreeNetwork(net);
    net = LoadNetwork();
    SaveNetwork(net);

    for(int index = 0; index < NUMBERCHARACTERS; index++)
    {
        printf("\n");
        net = propagation(net, inputs[index]);
        for(int yo = 0; yo < NUMBERELEMENTS; yo++)
        {
            printf("%i ", (int) inputs[index][yo]);
        }
        printf("\n");
        for(int h = 1; h < net.layers[0]; h++)
        {
            printf("%d ", (int) net.graph[h].val);
        }
        printf("\n");
        printf("Result: ");
        int mar = 0;
        for(int g = 0; g < net.laylenght - 1; g++)
        {
            mar += net.layers[g];
        }
        for(int v = mar + 1; v < net.graphlen; v++)
        {
            printf("Node(%i) = %f ", v, net.graph[v].val);
        }
        printf("\n");
        printf("The recognize character is : %c.\n", readResult(net, NUMBERCHARACTERS));
    }

    free(inputs);
    free(target);
    free(layers);

    return 0;
}

