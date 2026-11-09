//
// Created by flasque on 12/10/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "croissance.h"
#include "queue.h"

// prototypes des fonctions locales

float clampDirection(float);

// code des fonctions locales

float clampDirection(float dir)
{
    float res = dir;

    if (res < -80)
    {
        res = -80;
    }
    else if (res > 80)
    {
        res = 80;
    }

    return res;
}

// code des fonctions exportées

int canGrow(float probabilite)
{
    return (randomReal() < probabilite / 100.0);
}

int howManyBranches(float p1, float p2)
{
    double u = randomReal();

    if (u < p1 / 100.0)
        return 1;
    if (u < (p1 + p2) / 100.0)
        return 2;
    return 3;
}

void growBud(t_arbre *a, t_rameau *b, int n, int saison)
{
        // 1. on réserve un tableau de n cases pour ranger les fils
        b->p_fils = malloc(n * sizeof(t_rameau *));
        if (b->p_fils == NULL)
        {
            perror("growBud");
            exit(EXIT_FAILURE);
        }
        b->nb_fils = n;

        // 2. on crée les fils un par un
        for (int i = 0; i < n; i++)
        {
            float dir;
            float longueur;

            if (i == 0)
            {
                // 1er fils : il prolonge le parent
                dir = b->dir + randomBetween(-10, 10);
                longueur = randomBetween(0.7, 1.3);
            }
            else
            {
                // à gauche ou à droite, de 30° à 60°
                int cote = randomInt(2);
                float angle = randomBetween(30, 60);
                if (cote == 0)
                    dir = b->dir - angle;              // gauche baisse
                else
                    dir = b->dir + angle;              // droite augmente
                longueur = randomBetween(0.7, 1.3);
            }

            // 3. on bloque la direction entre -80° et +80°
            dir = clampDirection(dir);

            // 4. on crée le rameau et on le range dans la case i
            b->p_fils[i] = growBranch(a, b, saison, dir, longueur);
        }
    }


void growTree(t_arbre *a, int nbSaisons)
{
    // au départ, F contient seulement le pied
    t_queue F = createEmptyQueue();
    enqueue(&F, a->pied);

    for (int saison = 1; saison <= nbSaisons; saison++)
    {
        // début de saison : G est vide
        t_queue G = createEmptyQueue();

        // on vide F, un bourgeon à la fois
        while (!isEmptyQueue(F))
        {
            t_rameau *b = dequeue(&F);           // on sort un bourgeon de F

            if (canGrow(95))                      // il pousse (95 %
            {
                int n = howManyBranches(70, 25);  // combien de fils ?
                growBud(a, b, n, saison);
                for (int i = 0; i < n; i++)
                    enqueue(&G, b->p_fils[i]);    // ses fils iront à la saison suivante
            }
            else                                  // pousse pas
            {
                enqueue(&G, b);                   //  reste bourgeon
            }
        }

        F = G;
    }

    while (!isEmptyQueue(F))
        dequeue(&F);
}