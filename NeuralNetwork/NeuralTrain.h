#ifndef OCR_Neural_Train_H
#define OCR_Neural_Train_H

float sigmoid(float x);
Neural* propagation(Neural* network, char[] returnvalue);
Neural* retro(Neural* network, int target);
Neural* train(Neural* network, char[] path);


#endif