float sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

void propagation(Network network, char[] returnvalue)
{
    return None;
}

void retro(Network network, int target)
{
    return None;
}

void train(Network network, char[] path)
{
    return None;
}

Neural indexToNeural(Network network, int couche ,int unit)
{
    int index = 0;
    for (int i = 0; i < couche; i++)
    {
        index += network.layers[i];
    }
    index += unit;
    Neural current = network.graph[index];
    return curent;
}

double sum(Neural neural)
{
    return 0;
}