#pragma once
// ============================================================
//  Queue ADT  –  Circular array-based (FIFO)
//  CMP G218 Data Structures  |  Lecture 4
//  Used in Pac-Man : BFS ghost pathfinding toward Pac-Man
// ============================================================

#define QUEUE_MAX 512

struct QPos { int x, y; };

struct Queue {
    int  front, rear, count;
    QPos data[QUEUE_MAX];
};

// 'inline' is required because this header is included by multiple .cpp files.
inline void  Queue_Init    (Queue* q) { q->front = 0; q->rear = -1; q->count = 0; }
inline bool  Queue_IsEmpty (const Queue* q) { return q->count == 0; }
inline bool  Queue_IsFull  (const Queue* q) { return q->count == QUEUE_MAX; }
inline int   Queue_Size    (const Queue* q) { return q->count; }

inline bool  Queue_Enqueue(Queue* q, QPos p) {
    if (Queue_IsFull(q)) return false;
    q->rear = (q->rear + 1) % QUEUE_MAX;
    q->data[q->rear] = p;
    q->count++;
    return true;
}

inline bool  Queue_Dequeue(Queue* q, QPos* p) {
    if (Queue_IsEmpty(q)) return false;
    *p = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_MAX;
    q->count--;
    return true;
}
