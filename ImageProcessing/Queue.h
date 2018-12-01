#ifndef OCR_QUEUE_H
#define OCR_QUEUE_H
#include <stdlib.h>


typedef struct Node {
    struct Rect* data;
    struct Node* prev;
} Node;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue {
    Node *head;
    Node *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, Node *item);
Node *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

#endif //Queue.h
