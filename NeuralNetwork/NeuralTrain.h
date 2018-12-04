#ifndef OCR_Neural_Train_H
#define OCR_Neural_Train_H

#include "NeuralCreate.h"
#include "math.h"
float sigmoid(float x);
void initialzeResult(char target[]);
double** initializeTargets();
double error(Network network, Neural neural, double target, int isOut);
double newCost(Network network, Neural source, Neural dest, double step, double target, int isOut);
Neural* propagation(Network network);
Neural* retro(Network network, int target);
//Neural* train(Network network, char[] path);
Neural indexToNeural(Network network, int couche ,int unit);
double sum(Neural neural, Network network);


#endif