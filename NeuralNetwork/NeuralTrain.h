#ifndef OCR_Neural_Train_H
#define OCR_Neural_Train_H

float sigmoid(float x);
void initialzeResult(char target[]);
void initializeTargets(char *target[]);
double error(Network network, Neural neural, double target, int isOut);
double newCost(Network network, Neural source, Neural dest, double step, double target, int isOut);
Neural* propagation(Neural* network, char[] returnvalue);
Neural* retro(Neural* network, int target);
//Neural* train(Neural* network, char[] path);
Neural indexToNeural(Network network, int couche ,int unit);
double sum(Neural neural, Network network);


#endif