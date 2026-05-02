#pragma once
// ============================================================
//  Linear List ADT  –  Array-based implementation
//  CMP G218 Data Structures  |  Lecture 5
//  Used in Pac-Man : sorted leaderboard (top 10 scores)
// ============================================================

#include <cstring>

#define LIST_MAX 10

struct ScoreEntry {
    char name[20];
    int  score;
    int  level;
};

struct List {
    int        length;
    ScoreEntry items[LIST_MAX];
};

// 'inline' is required because this header is included by multiple .cpp files.
inline void  List_Init    (List* L)              { L->length = 0; }
inline bool  List_IsEmpty (const List* L)        { return L->length == 0; }
inline bool  List_IsFull  (const List* L)        { return L->length == LIST_MAX; }

// Insert at index i – shift elements right (Lecture 5)
inline bool  List_Insert(List* L, int i, ScoreEntry e) {
    if (List_IsFull(L) || i < 0 || i > L->length) return false;
    for (int j = L->length; j > i; j--)
        L->items[j] = L->items[j - 1];
    L->items[i] = e;
    L->length++;
    return true;
}

// Remove at index i – shift elements left (Lecture 5)
inline bool  List_Remove(List* L, int i) {
    if (List_IsEmpty(L) || i < 0 || i >= L->length) return false;
    for (int j = i; j < L->length - 1; j++)
        L->items[j] = L->items[j + 1];
    L->length--;
    return true;
}

inline bool  List_Get(const List* L, int i, ScoreEntry* e) {
    if (i < 0 || i >= L->length) return false;
    *e = L->items[i];
    return true;
}

// Insert score keeping list sorted descending
inline void  List_AddScore(List* L, ScoreEntry e) {
    int pos = L->length;
    for (int i = 0; i < L->length; i++) {
        if (e.score > L->items[i].score) { pos = i; break; }
    }
    if (List_IsFull(L)) {
        if (pos == LIST_MAX) return;   // score not good enough
        L->length--;                   // drop the last entry
    }
    List_Insert(L, pos, e);
}
