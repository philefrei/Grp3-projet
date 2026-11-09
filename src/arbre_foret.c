/* arbre_foret.c : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière (fichier fourni vide, à compléter) */
#include <stdlib.h>
#include "arbre_foret.h"
#include "croissance.h"


float clampDirection(float);


int countBranches(t_rameau *r)
{
    int i;
    int total;

    total = 1;    // le rameau r lui-meme

    for (i = 0; i < r->nb_fils; i++)
    {
        total = total + countBranches(r->p_fils[i]);
    }

    return total;
}

int computeHeight(t_rameau *r)
{
    int i;
    int res;
    int h;

    res = r->gen;

    for (i = 0; i < r->nb_fils; i++)
    {
        h = computeHeight(r->p_fils[i]);
        if (h > res)
        {
            res = h;
        }
    }

    return res;
}

float computeTop(t_rameau *r)
{
    int i;
    float res;
    float y;

    res = r->y;

    for (i = 0; i < r->nb_fils; i++)
    {
        y = computeTop(r->p_fils[i]);
        if (y > res)
        {
            res = y;
        }
    }

    return res;
}

float computeXMin(t_rameau *r)
{
    int i;
    float res;
    float x;

    res = r->x;

    for (i = 0; i < r->nb_fils; i++)
    {
        x = computeXMin(r->p_fils[i]);
        if (x < res)
        {
            res = x;
        }
    }

    return res;
}

float computeXMax(t_rameau *r)
{
    int i;
    float res;
    float x;

    res = r->x;

    for (i = 0; i < r->nb_fils; i++)
    {
        x = computeXMax(r->p_fils[i]);
        if (x > res)
        {
            res = x;
        }
    }

    return res;
}

int countBuds(t_rameau *r)
{
    int i;
    int total;
    // un rameau sans fils est un bourgeon
    if (r->nb_fils == 0)
    {
        return 1;
    }

    total = 0;
    for (i = 0; i < r->nb_fils; i++)
    {
        total = total + countBuds(r->p_fils[i]);
    }

    return total;
}

void freeBranches(t_rameau *r)
{
    int i;

    // d'abord les fils : apres free(r), on ne peut plus lire r->p_fils
    for (i = 0; i < r->nb_fils; i++)
    {
        freeBranches(r->p_fils[i]);
    }

    free(r->p_fils);    // le tableau des fils (free(NULL) ne fait rien)
    free(r);            // puis le rameau
}



t_arbre_foret createForestTree(int id, float x, float y, int seed)
{
    t_arbre_foret a;

    // createTreeStep cree le pied, la file avec le pied et la saison 0
    a.step = createTreeStep(x, y, seed);

    // un arbre reduit a son pied
    a.id = id;
    a.hauteur = 0;
    a.cime = y;
    a.xmin = x;
    a.xmax = x;
    a.nb_bourgeons = 1;
    a.tronque = 0;

    return a;
}

void freeForestTree(t_arbre_foret *a)
{
    clearBuds(&a->step);                 // les cellules de la file
    freeBranches(a->step.arbre.pied);    // tous les rameaux
    a->step.arbre.pied = NULL;
}

#include "arbre_foret.h"
