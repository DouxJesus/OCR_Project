#include <stdio.h>

Neural CreateNeural(double val)
{
	Neural n;
	n.val = val;;
	n.plenght = 0;
	n.slenght = 0;
	return n;
}

Neural CreateCompleteNeural(double val, Tuple* predes, Tuple* sucess, int plen, int slen)
{
	Neural n;
	n.val = val;;
	n->predes = predes;
	n->sucess = sucess;
	n.plenght = plen;
	n.slenght = slen;

	return n;
}

Neural AddPredessesseur(Neural n, Tuple predes)
{
	if (n.plenght == 0)
	{
		if(!(n.predes = malloc(sizeof(Tuple))))
    	{
        	exit(-1);
    	}
    	n.predes[0] = predes;
	}
	else
	{
		if(!(n.predes = realloc(n.predes, (n.plenght + 1) * sizeof(Tuple))))
    	{
        	exit(-1);
    	}
    	n.predes[plenght] = predes;
	}
	n.plenght += 1;

	return n;
}

Neural AddSuccesseur(Neural n, Tuple sucess)
{
	if (n.slenght == 0)
	{
		if(!(n.sucess = malloc(sizeof(Tuple))))
    	{
        	exit(-1);
    	}
    	n.sucess[0] = sucess;
	}
	else
	{
		if(!(n.sucess = realloc(n.sucess, (n.slenght + 1) * sizeof(Tuple))))
    	{
        	exit(-1);
    	}
    	n.sucess[slenght] = sucess;
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

Tuple CreateTuple(int pos, double cost)
{
	Tuple t;
	t.pos = pos;
	t.cost = cost;

	return t;
}

Neural* CreateNetwork(int height, int colbegin, int colend)
{
//Chaque ligne de Neural**: une couche de neurones
//chaque col : un sommet de la couche (chaque couche ne fait pas forcément la même taille)
	Neural* network;

	return network;
}