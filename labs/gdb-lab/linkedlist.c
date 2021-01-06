#include <stdlib.h>
#include <stdio.h>

#include "linkedlist.h"

typedef struct Node {
        int data;
        struct Node* next;
} Node;

LinkedList newNode(int x)
{
        LinkedList xs = malloc(sizeof(*xs));

        xs->data = x;
        xs->next = NULL;

        return xs;
}

LinkedList makeList(int n)
{
        LinkedList xs = newNode(0);
        LinkedList next;

        next = xs;

        for (int i = 1; i < n; i++) {
                next->next = newNode(i);
                next = next->next;
        }
        incrementList(xs);
        return xs;
}

void printList(LinkedList xs)
{
        for (LinkedList next = xs; next != NULL; next = next->next) {
                printf("%d\n", next->data);
        }       
}

void freeList(LinkedList xs)
{
        LinkedList next;

        while (xs != NULL) {
                next = xs->next;
                free(xs);
                xs = next;
        }

}

void incrementList(LinkedList xs) {
        for (LinkedList next = xs; next != NULL; next = next->next) {
                next->data = next->data + 1;;
        }   
}