//linked list for storing jobs
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H
#include <stdio.h>
#include <stdlib.h>

typedef struct Element Element;
struct Element
{
    idconnx id;
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
    int taille;
};

Liste *initialisation();
idconnx getId(Liste *liste, int position);
void insertion(Liste *liste, idconnx id1);
void suppression(Liste *liste);
#endif
