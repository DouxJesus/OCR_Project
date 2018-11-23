#include <stdio.h>
#include <stdlib.h>

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

double frand_a_b(double a, double b){

    return ( rand()/(double)RAND_MAX ) * (b-a) + a

   }

Neural* CreateNetwork(int* layers, int laylenght)
{
//Chaque ligne de Neural**: une couche de neurones
//chaque col : un sommet de la couche (chaque couche ne fait pas forcément la même taille)
	Neural* network;
	network.laylenght = laylenght;
	network.layers = layers;
	network.graphlen = 0;

	for (int i = 0; i < laylenght; ++i)
	{
		int count = layers[i];
		if (network.graphlen != 0)
		{
			if(!(n.predes = realloc(network.graph, (network.graphlen + count + 1) * sizeof(Tuple))))
    		{
        		exit(-1);
    		}
    		network.graphlen += count + 1;
		}
		else
		{
			//premiere couche
			if(!(network.graph = calloc(count + 1, sizeof(Neural))))
    		{
        		exit(-1);
    		}
    		network.graphlen = count + 1;
		}
		for (int j = 0; j <  count; ++j)
		{
			double randbl = frand_a_b(0, 1);
			Neural n;
			n = CreateNeural(randbl);
			
			if(j != 0)
			{
				int countpredes = layers[i - 1];
				for (int l = 0; l < countpredes; ++l)
				{
					/* code */
				}
			}
			if(j < count - 1)
			{
				int countsucess = layers[i + 1];
				for (int k = 0; k < countsucess; ++k)
				{
					/* code */
				}
			}
		}

	}

	return network;
}