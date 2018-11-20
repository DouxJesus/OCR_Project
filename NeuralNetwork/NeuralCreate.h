#ifndef OCR_Neural_Create_H
#define OCR_Neural_Create_H

typedef struct Tuple{
	int pos;
	double cost;
}Tuple;

typedef struct Neural{
	double val;
	Tuple* predes;
	Tuple* sucess;
	int plenght;
    int slenght;
}Neural;


Neural CreateNeural(int val);
Neural CreateCompleteNeural(int val, int* predes, int* sucess, int plen, int slen);
Neural AddPredessesseur(Neural n, int predes);
Neural AddSuccesseur(Neural n, int sucess);
void FreeNeural(Neural n);


#endif
