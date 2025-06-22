#include <stdlib.h>
#include "PQueue.h"




pQueue* createPriorityQueue() {
    pQueue* q = (pQueue*)malloc(sizeof(pQueue));
    q->head = NULL;
    return q;
}


pqNode* newNode(PCB* d, int p) {
    pqNode* temp = (pqNode*)malloc(sizeof(pqNode));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;
    return temp;
}


PCB* peek(pQueue* q) {
    return q->head->data;
}


void pop(pQueue* q) {
    pqNode* temp = q->head;
    q->head = q->head->next;
    free(temp);
}


void push(pQueue* q,PCB* d, int p) {
    pqNode* start = q->head;
    
    
    // Create new Node
    pqNode* temp = newNode(d, p);

    if (start == NULL) {
        q->head = temp;
    } else {
        // Special Case: The head of list has
        // lesser priority than new node. So
        // insert newnode before head node
        // and change head node.
        if (q->head->priority > p)
        {
            // Insert New Node before head
            temp->next = q->head;
            q->head = temp;
        }
        else
        {
            // Traverse the list and find a
            // position to insert new node
            while (start->next != NULL && start->next->priority <= p)
            {
                start = start->next;
            }
            // Either at the ends of the list
            // or at required position
            temp->next = start->next;
            start->next = temp;
        }
    }
}


bool isEmpty(pQueue* q) {
    return q->head == NULL;
}

void updateFrontPriority(pQueue* q,int newP) {
    if(q->head->data)
        q->head->priority = newP;
}

int GetTopPriority(pQueue* q) {
    return q->head->priority;
}
