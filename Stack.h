#pragma once
// ============================================================
//  Stack ADT  –  Array-based implementation
//
//  Used in Pac-Man : move history (undo trail)
// ============================================================

#define STACK_MAX 512

struct Position { int x, y; };

struct Stack {
    int      top;
    Position data[STACK_MAX];
};

// 'inline' is required because this header is included by multiple
// .cpp files. Without it the linker sees the same function defined
// more than once and refuses to build (multiple definition error).
inline void  Stack_Init    (Stack* s)             { s->top = -1; }
inline bool  Stack_IsEmpty (const Stack* s)       { return s->top == -1; }
inline bool  Stack_IsFull  (const Stack* s)       { return s->top == STACK_MAX - 1; }
inline int   Stack_Size    (const Stack* s)       { return s->top + 1; }

inline bool  Stack_Push(Stack* s, Position p) {
    if (Stack_IsFull(s)) return false;
    s->data[++s->top] = p;
    return true;
}

inline bool  Stack_Pop(Stack* s, Position* p) {
    if (Stack_IsEmpty(s)) return false;
    *p = s->data[s->top--];
    return true;
}

inline bool  Stack_Peek(const Stack* s, Position* p) {
    if (Stack_IsEmpty(s)) return false;
    *p = s->data[s->top];
    return true;
}
