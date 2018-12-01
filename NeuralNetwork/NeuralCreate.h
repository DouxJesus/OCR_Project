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

typedef struct Network{
	Neural* graph;
	int graphlen;
	int* layers;
	int laylenght;
}Network;


Neural CreateNeural(Neural n, double val);
Neural CreateCompleteNeural(double val, Tuple* predes, Tuple* sucess, int plen, int slen);
Neural AddPredessesseur(Neural n, Tuple predes);
Neural AddSuccesseur(Neural n, Tuple sucess);
void FreeNeural(Neural n);
Tuple CreateTuple(int pos, double cost);
Network CreateNetwork(int* layers, int laylenght);
void SaveNetwork(Network network);
Network LoadNetwork();
void FreeNetwork(Network network);
#endif
