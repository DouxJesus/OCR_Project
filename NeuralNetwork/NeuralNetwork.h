#ifndef OCR_Neural_H
#define OCR_Neural_H

typedef struct Neural
{
    int val;
    int* predes:
    int plenght;
    int* sucess;
    int slenght;
} Neural;

Neural CreateNeural(int val);
Neural CreateCompleteNeural(int val, int* predes, int* sucess, int plen, int slen);
Neural AddPredessesseur(Neural n, int predes);
Neural AddSuccesseur(Neural n, int sucess);
void FreeNeural(Neural n);

Neural** initialize(int** values)
float sigmoid(float x)
Neural** propagation(Neural** network, char[] returnvalue)
Neural** retro(Neural** network, int target)
Neural** train(Neural** network, char[] path)
#endif