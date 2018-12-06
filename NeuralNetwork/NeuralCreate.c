#include <stdio.h>
#include <stdlib.h>
#include "NeuralCreate.h"

Neural CreateNeural(Neural n, double val)
{
	n.val = val;
    n.error = 0;
	n.predes = 0;
	n.sucess = 0;
	n.plenght = 0;
	n.slenght = 0;

	return n;
}

Neural CreateCompleteNeural(double val, Tuple* predes, Tuple* sucess, int plen, int slen)
{
	Neural n;
	n.val = val;;
    n.error = 0;
	n.predes = predes;
	n.sucess = sucess;
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
    	n.predes[n.plenght] = predes;
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
    	n.slenght = 1;
	}
	else
	{
		if(!(n.sucess = realloc(n.sucess, (n.slenght + 1) * sizeof(Tuple))))
    	{
        	exit(-1);
    	}
    	n.sucess[n.slenght] = sucess;
    	n.slenght += 1;
	}

	return n;
}

Tuple CreateTuple(int pos, double cost)
{
	Tuple t;
	t.pos = pos;
	t.cost = cost;

	return t;
}

double frand_a_b(double a, double b){

    return ( rand()/(double)RAND_MAX ) * (b-a) + a;

   }

Network CreateNetwork(int* layers, int laylenght)
{
//Chaque ligne de Neural**: une couche de neurones
//chaque col : un sommet de la couche (chaque couche ne fait pas forcément la même taille)
	Network network;
	network.laylenght = laylenght;
	network.layers = layers;
	network.graphlen = 0;

	int index = 0;
    int index_success = 0;
    int index_predes = 0;

	for (int i = 0; i < laylenght; ++i)
	{
		int count = layers[i];
        index_success += count;
		if (network.graphlen != 0)
		{
			if(!(network.graph = realloc(network.graph, (network.graphlen + count) * sizeof(Neural))))
    		{
        		exit(-1);
    		}
    		network.graphlen += count;
		}
		else
		{
			//premiere couche
			if(!(network.graph = calloc(count, sizeof(Neural))))
    		{
        		exit(-1);
    		} 
    		network.graphlen = count;
		}


		for (int j = 0; j <  count; ++j)
		{
			double randbl = frand_a_b(0, 1);
			Neural n;
			n = CreateNeural(n, randbl);


			if(j != 0 && i != 0)
			{
				// sinon c'est le biais
				int countpredes = layers[i - 1];
				for (int l = 0; l < countpredes; ++l)
				{
					randbl = frand_a_b(-10, 10);
					Tuple t = CreateTuple(index_predes + l, randbl);
					n = AddPredessesseur(n, t);
				}
			}

			if(i < laylenght - 1)
			{
				int countsucess = layers[i + 1];
				for (int k = 0; k < countsucess; ++k)
				{
					randbl = frand_a_b(-10, 10);
					Tuple t = CreateTuple(index_success + k, randbl);
					n = AddSuccesseur(n, t);
				}
			}
			network.graph[index + j] = n;
		}

        if (i != 0)
        {
            index_predes += layers[i - 1];
        }
		index += count;

	}

	return network;
}

void FreeNetwork(Network network)
{
    for (int i = 0; i < network.graphlen; ++i)
    {
        Neural n = network.graph[i];
        free(n.predes);
        free(n.sucess);
    }
    free(network.graph);
    free(network.layers);
}


void SaveNetwork(Network network)
{
	FILE* file = fopen("network.mytxt", "w");

    if (file != NULL)
    {
    	char str[40];
    	sprintf(str, "%i\n", network.laylenght);
    	fputs(str, file);

    	for (int i = 0; i < network.laylenght; ++i)
    	{
    		//if(i != network.laylenght - 1)
    			sprintf(str, "%i-", network.layers[i]);
    		//else
    		//	sprintf(str, "%i", network.layers[i]);
    		fputs(str, file);
    	}
    	fputc('\n', file);
    	
    	sprintf(str, "%i\n", network.graphlen);
    	fputs(str, file);

    	//ajout neurones
    	for (int i = 0; i < network.graphlen; ++i)
    	{
    		Neural n = network.graph[i];

    		sprintf(str, "%f\n", n.val);
    		fputs(str, file);

    		//sprintf(str, "%i\n%i\n", n.plenght, n.slenght);
    		//fputs(str, file);

		//listes ajout
    		Tuple* listpredess = n.predes;
    		Tuple* listsucess = n.sucess;

    		for (int j = 0; j < n.plenght; ++j)
    		{
    			Tuple t = listpredess[j];
    			//if(j != n.plenght - 1)
    				sprintf(str, "(%i,%f)_", t.pos, t.cost);
    			//else
    			//	sprintf(str, "(%i,%f)", t.pos, t.cost);
    			fputs(str, file);
    		}
            fputs("\n", file);
    		for (int k = 0; k < n.slenght; ++k)
    		{
    			Tuple t = listsucess[k];
    			//if(k != n.slenght - 1)
    				sprintf(str, "(%i,%f)_", t.pos, t.cost);
    			//else
    			//	sprintf(str, "(%i,%f)", t.pos, t.cost);
    			fputs(str, file);
    		}
    		fputs("\n", file);
    	}
    	fputs("\n#", file);
        fclose(file); 
    }

	
}


Network LoadNetwork()
{
	FILE* file = fopen("network.mytxt", "r");
	Network network;
	network.laylenght = 0;
	network.layers = 0;
	network.graphlen = 0;
	network.graph = 0;
    if (file != NULL)
    {
    	 int c = 0;
    	 int i = 0;
    	 char str[40];
    	 //récupérer laylenght
    	 while ((c = fgetc(file)) != '\n')
   		{
   			str[i] = c;
   			i++;
 		}
        str[i] = '\0';
 		sscanf(str, "%i", &network.laylenght);

 		if(!(network.layers = malloc(network.laylenght * sizeof(int))))
    	{
        	exit(-1);
    	}

    	i=0;
 		//récupérer layers
 		while((c = fgetc(file)) != '\n' && i < network.laylenght)
 		 {
 		 	int j = 0;
            str[j] = c;
            j++;
 		 	while ((c = fgetc(file)) != '-')
   			{
   				str[j] = c;
   				j++;
 			}
            str[j] = '\0';
            int val = 0;
 			sscanf(str, "%i", &val);
 			network.layers[i] = val;
 			i++;
 		}

        i = 0;
         while ((c = fgetc(file)) != '\n')
        {
            str[i] = c;
            i++;
        }
        str[i] = '\0';
        sscanf(str, "%i", &network.graphlen);
        network.graph = malloc(network.graphlen * sizeof(Neural));

        if(!network.graph)
        {
            exit(1);
        }

        i=0;
        int j = 0;
         while (j < network.graphlen && (c = fgetc(file)) != '#')
        {
            Neural n;

            i = 0;
            while(c != '\n')
            {
               str[i] = c;
               i++;
               c = fgetc(file);
            }
            str[i] = '\0';
            sscanf(str, "%lf", &n.val);

            n = CreateNeural(n, n.val);

            while((c = fgetc(file)) != '\n')
            {
                i = 0;
                str[i] = c;
                i++;
                while ((c = fgetc(file)) != '_')
                {
                    str[i] = c;
                    i++;
                }
                str[i] = '\0';
                int pos = 0;
                double cost = 0;
            sscanf(str, "(%i,%lf)", &pos, &cost);

            Tuple t = CreateTuple(pos, cost);

            n = AddPredessesseur(n, t);
            }

            while((c = fgetc(file)) != '\n')
            {
                i = 0;
                str[i] = c;
                i++;
                while ((c = fgetc(file)) != '_')
                {
                    str[i] = c;
                    i++;
                }
                str[i] = '\0';
                int pos = 0;
                double cost = 0;
            sscanf(str, "(%i,%le)", &pos, &cost);

            Tuple t = CreateTuple(pos, cost);

            n = AddSuccesseur(n, t);
            }
            network.graph[j] = n;
            j++;
        }

 		 while ((c = fgetc(file)) != EOF)
   		{
 		}
      fclose (file); 
    }

    return network;
}
