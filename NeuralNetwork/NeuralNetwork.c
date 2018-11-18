#include "NeuralNetwork.h"


Neural CreateNeural(int val)
{
	Neural n;
	n.val = val;
	n.plenght = 0;
	n.slenght = 0;
	return n;
}

Neural CreateCompleteNeural(int val, int* predes, int* sucess, int plen, int slen)
{
	Neural n;
	n.val = val;
	n->predes = predes;
	n->sucess = sucess;
	n.plenght = plen;
	n.slenght = slen;

	return n;
}

Neural AddPredessesseur(Neural n, int predes)
{
	if (n.plenght == 0)
	{
		if(!(n.predes = malloc(sizeof(int))))
    	{
        	exit(-1);
    	}
    	n.predes[0] = predes;
	}
	else
	{
		if(!(n.predes = realloc(n.predes, (n.plenght + 1) * sizeof(int))))
    	{
        	exit(-1);
    	}
    	n.predes[0] = predes;
	}
	n.plenght += 1;

	return n;
}

Neural AddSuccesseur(Neural n, int sucess)
{
	if (n.slenght == 0)
	{
		if(!(n.sucess = malloc(sizeof(int))))
    	{
        	exit(-1);
    	}
    	n.sucess[0] = sucess;
	}
	else
	{
		if(!(n.sucess = realloc(n.sucess, (n.slenght + 1) * sizeof(int))))
    	{
        	exit(-1);
    	}
    	n.sucess[0] = predes;
	}
	n.slenght += 1;

	return n;
}


void FreeNeural(Neural n)
{
	Free(n.predes);
	Free(n.sucess);
	Free(n);
}


Neural** initialize(int height, int colbegin, int colend)
{
//Chaque ligne de Neural**: une couche de neurones
//chaque col : un sommet de la couche (chaque couche ne fait pas forcément la même taille)
	Neural** network;

	return network;
}

float sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

/*Neural** propagation(Neural** network, char[] returnvalue)
{

}

Neural** retro(Neural** network, int target)
{

}

Neural** train(Neural** network, char[] path)
{

}*/