#include <stdlib.h>
#include "Queue.h"



queue* createQueue() {
    queue* q = malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}


// Function to create a new linked list node.
qNode* newNNode(PCB* d) {
    qNode* temp = (qNode*)malloc(sizeof(qNode));
    temp->data = d;
    temp->next = NULL;
    return temp;
}


// The function to add a key k to q
void enQueue(queue* q,PCB* d) { 
    // Create a new LL node
    qNode* temp = newNNode(d);
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}


// Return the value at head
PCB* peekN(queue* q) {
    return q->front->data;
}


// Function to remove a key from given queue q
void deQueue(queue* q) { 
    // If queue is empty, return.
    if (q->front == NULL)
        return;
    // Store previous front and move front one node ahead
    qNode* temp = q->front;
    q->front = q->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);    
}



// Function to check is list is empty
bool isEmptyN(queue* q) {
    return q->front == NULL;
}


void PrintQueue(queue* q) {
    qNode* temp = q->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data->id);
        temp = temp->next;
    }
    printf("NULL\n");
}



PCB* TraverseQueue(queue* q, MemoryNode* root, SchedulingAlgo algo, bool(*func)(PCB*, SchedulingAlgo,int))
{
    qNode* temp = q->front;
    qNode* prev = NULL;

    while (temp != NULL)
    {
        if (func(temp->data, algo, 1))
        {
            if (prev == NULL)
                q->front = temp->next;
            else
                prev->next = temp->next;

            if (temp == q->rear)
            {
                if (prev == NULL)
                    q->rear = NULL;
                else
                    q->rear = prev;
            }

            PCB* result = temp->data;
            free(temp); // if allocated with malloc
            return result;
        }
        prev = temp;
        temp = temp->next;
    }
    return NULL;
}
