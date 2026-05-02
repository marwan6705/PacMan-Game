#pragma once

#define QUEUE_MAX 512

struct QPos { int x, y; }; //store position

struct Queue {             //decleration of the queue
    int  front, rear, count;
    QPos data[QUEUE_MAX];
};

// 'inline' is required because this header is included by multiple .cpp files
inline void  Queue_Init    (Queue* q) { q->front = 0; q->rear = -1; q->count = 0; }
inline bool  Queue_IsEmpty (const Queue* q) { return q->count == 0; }
inline bool  Queue_IsFull  (const Queue* q) { return q->count == QUEUE_MAX; }
inline int   Queue_Size    (const Queue* q) { return q->count; }

inline bool  Queue_Enqueue(Queue* q, QPos p) {  
    if (Queue_IsFull(q)) return false;            //1-check if the queue is full or not
    q->rear = (q->rear + 1) % QUEUE_MAX;   //2-increase the rear by one and if the rear was at the last place it will circulate to the first place
    q->data[q->rear] = p;                   //3-put the added element in the rear
    q->count++;                             //4-increase the count by one
    return true;
}

inline bool  Queue_Dequeue(Queue* q, QPos* p) {   
    if (Queue_IsEmpty(q)) return false;            //1-check if the queue is empty or not
    *p = q->data[q->front];                     //2-return the first element not a copy to the user before deleting it
    q->front = (q->front + 1) % QUEUE_MAX;      //3-move the front to the next by using circular methode 
    q->count--;                                 //4-decrease the count by one 
    return true;
}
