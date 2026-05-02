#pragma once

// Linear List ADT - Array-based implementation
// Used in Pac-Man : sorted leaderboard (top 10 scores)

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

// inline is required because this header is included by multiple .cpp files.

// Declaration : initialise the list to empty
inline void  List_Init    (List* L)              { L->length = 0; }

// Declaration : return true if the list holds no entries
inline bool  List_IsEmpty (const List* L)        { return L->length == 0; }

// Declaration : return true if the list has reached LIST_MAX capacity
inline bool  List_IsFull  (const List* L)        { return L->length == LIST_MAX; }

// Declaration : insert entry e at index i, shifting elements right
inline bool  List_Insert(List* L, int i, ScoreEntry e) {
    if (List_IsFull(L) || i < 0 || i > L->length) return false;
    for (int j = L->length; j > i; j--)
        L->items[j] = L->items[j - 1];
    L->items[i] = e;
    L->length++;
    return true;
}

// Declaration : remove entry at index i, shifting elements left
inline bool  List_Remove(List* L, int i) {
    if (List_IsEmpty(L) || i < 0 || i >= L->length) return false;
    for (int j = i; j < L->length - 1; j++)
        L->items[j] = L->items[j + 1];
    L->length--;
    return true;
}

// Declaration : copy entry at index i into e
inline bool  List_Get(const List* L, int i, ScoreEntry* e) {
    if (i < 0 || i >= L->length) return false;
    *e = L->items[i];
    return true;
}

// Declaration : insert a score entry keeping the list sorted descending
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