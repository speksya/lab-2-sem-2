#ifndef MAIN_H
#define MAIN_H

struct String {
    char* data;
    char Mark;
};

struct Node {
    String data;
    Node* next;
};

struct LinkedList {
    Node* head;
};

#endif
