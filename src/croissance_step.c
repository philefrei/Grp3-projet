//
// Created by flasque on 14/10/2026.
//
#include <stdio.h>
#include <stdlib.h>

#include "croissance_step.h"
#include "croissance.h"

t_arbre_step createTreeStep(float x, float y, int seed)
{
    t_arbre_step a;

    a.arbre = createTree(x, y, seed);
    a.bourgeons = createEmptyQueue();
    enqueue(&a.bourgeons, a.arbre.pied);
    a.saison = 0;

    return a;
}

void growTreeStep(t_arbre_step *a)
{
    t_queue G = createEmptyQueue();
    int saison = a->saison + 1;

    while (!isEmptyQueue(a->bourgeons))
    {
        t_rameau *b = dequeue(&a->bourgeons);

        if (canGrow(95))
        {
            int n = howManyBranches(70, 25);
            growBud(&a->arbre, b, n, saison);
            for (int i = 0; i < n; i++)
                enqueue(&G, b->p_fils[i]);
        }
        else
        {
            enqueue(&G, b);
        }
    }

    a->bourgeons = G;
    a->saison = saison;
}

void clearBuds(t_arbre_step *a)
{
    while (!isEmptyQueue(a->bourgeons))
        dequeue(&a->bourgeons);
}
