//
// Created by nora on 19/10/18.
//

#ifndef RESEAUNEURONE_LEARNING_H
#define RESEAUNEURONE_LEARNING_H


typedef struct
{
    float x;
    float x_k;
    float x_z;

    float y;
    float y_k;
    float y_z;

    float i;
    float i_k;

    float k;
    float k_z;

    float z;

    float j;
    float j_z;

    float target;
}Neurone;

void initialize(Neurone n);
void propagation(Neurone n);
void retro(Neurone n);

#endif //RESEAUNEURONE_LEARNING_H
