#ifndef OCR_Neural_Train_H
#define OCR_Neural_Train_H

#include "NeuralCreate.h"
#include "math.h"
float sigmoid(float x);
char *initialzeResult();
double** initializeTargets();
Network error(Network network, Neural neural, double target, int isOut);
Network newCost(Network network, Neural source, Neural dest, double step, double target, int isOut);
Network propagation(Network network);
Network retro(Network network, int target);
Network train(Network network, char[] path);
Neural indexToNeural(Network network, int couche ,int unit);
double sum(Neural neural, Network network);


#endif