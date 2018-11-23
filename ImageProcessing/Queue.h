#ifndef OCR_QUEUE_H
#define OCR_QUEUE_H
#include <stdlib.h>
#include <stdio.h>
#include <RLSA.h>

/* a link in the queue, holds the info and point to the next Node*/
typedef struct {
    ;
} Data;

typedef struct Node_t {
    Data data;
    struct Node_t *prev;
} Node;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
Node *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

#endif //Queue.h
