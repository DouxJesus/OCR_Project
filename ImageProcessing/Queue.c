#include "Queue.h"

Queue *ConstructQueue(int limit) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void DestructQueue(Queue *q) {
    Node *node;
    while (!isEmpty(q)) {
        node = Dequeue(q);
        free(node);
    }
    free(q);
}

int Enqueue(Queue *q, Node *item) {
    /* Bad parameter */
    if ((q == NULL) || (item == NULL)) {
        return 0;
    }
    // if(q->limit != 0)
    if (q->size >= q->limit) {
        return 0;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (q->size == 0) {
        q->head = item;
        q->tail = item;

    } else {
        /*adding item to the end of the queue*/
        q->tail->prev = item;
        q->tail = item;
    }
    q->size++;
    return 1;
}

Node * Dequeue(Queue *q) {
    /*the queue is empty or bad param*/
    Node *item;
    if (isEmpty(q))
        return NULL;
    item = q->head;
    q->head = (q->head)->prev;
    q->size--;
    return item;
}

int isEmpty(Queue* q) {
    if (q == NULL) {
        return 0;
    }
    if (q->size == 0) {
        return 1;
    } else {
        return 0;
    }
}

// int main() {
//     int i;
//     Queue *pQ = ConstructQueue(7);
//     Node *pN;

//     for (i = 0; i < 9; i++) {
//         pN = (Node*) malloc(sizeof (Node));
//         pN->data.info = 100 + i;
//         Enqueue(pQ, pN);
//     }

//     while (!isEmpty(pQ)) {
//         pN = Dequeue(pQ);
//         printf("\nDequeued: %d", pN->data);
//         free(pN);
//     }
//     DestructQueue(pQ);
//     return (EXIT_SUCCESS);
// }