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
    int i;
    int cote;
    float angle;
    float dir;
    float longueur;

    // on cree le tableau des fils : n cases
    b->p_fils = malloc(n * sizeof(t_rameau*));

    for (i = 0; i < n; i++)
    {
        if (i == 0)
        {
            // 1er rameau : il prolonge le parent
            dir = b->dir + randomBetween(-10, 10);
            longueur = randomBetween(0.7, 1.3);
        }
        else
        {
            // rameaux suivants : a gauche ou a droite
            cote = randomInt(2);
            angle = randomBetween(30, 60);

            if (cote == 0)
            {
                dir = b->dir - angle;
            }
            else
            {
                dir = b->dir + angle;
            }

            longueur = randomBetween(0.7, 1.3);
        }

        // on ramene la direction entre -80 et 80 avec clampDirection (deja donné)
        dir = clampDirection(dir);
        b->p_fils[i] = growBranch(a, b, saison, dir, longueur);
    }
    b->nb_fils = n;
}


void growTree(t_arbre *a, int nbSaisons){
    if (a == NULL || a->pied == NULL) {
        return;
    }
    // F saison courante G saison suivante
    t_queue F = createEmptyQueue();
    t_queue G = createEmptyQueue();
    //racine de l'arbre
    enqueue(&F, a->pied);
    for (int i = 0; i < nbSaisons; i++) {
        G = createEmptyQueue(); // on remet G à 0 à chaque nouvelle saison
        while (!isEmptyQueue(F)) {
            t_rameau *b = dequeue(&F); // dépilement
            if (canGrow(95)) { // test pousse bourgeon
                int n;
                n = howManyBranches(70, 25);
                growBud(a, b, n, i + 1);
                for (int j = 0; j < n ; j++) {
                    enqueue(&G, b->p_fils[j]);
                }

            } else enqueue(&G,b); // erreur de la pousse du bourgeon
        }
        F = G;
    }
    while (!isEmptyQueue(F)) {
        dequeue(&F);
    }
}