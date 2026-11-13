#include <stdlib.h>
#include "arbre_foret.h"
#include "croissance.h"
#include "croissance_lumiere.h"


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

void growBudInLight(t_arbre_foret *a, t_rameau *b, int n, double lumiere)
{
    int i;
    int cote;
    float angle;
    float dir;
    double u;
    float longueur;
    t_rameau *fils;

    // on cree le tableau des fils : n cases
    b->p_fils = malloc(n * sizeof(t_rameau *));

    for (i = 0; i < n; i++)
    {
        if (i == 0)
        {
            // 1er rameau : il prolonge le parent
            dir = b->dir + randomBetween(-10, 10);
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
        }

        // longueur : plus courte a l'ombre
        u = randomBetween(0.7, 1.3);
        longueur = (0.5 + 0.5 * lumiere) * u;

        dir = clampDirection(dir);
        fils = growBranch(&a->step.arbre, b, a->step.saison, dir, longueur);
        b->p_fils[i] = fils;

        // mise a jour des mesures avec le nouveau rameau
        if (fils->gen > a->hauteur)
        {
            a->hauteur = fils->gen;
        }
        if (fils->y > a->cime)
        {
            a->cime = fils->y;
        }
        if (fils->x < a->xmin)
        {
            a->xmin = fils->x;
        }
        if (fils->x > a->xmax)
        {
            a->xmax = fils->x;
        }
    }
    b->nb_fils = n;

    // b devient du bois (-1 bourgeon) et ses n fils sont des bourgeons (+n)
    a->nb_bourgeons = a->nb_bourgeons + n - 1;
}


void growSeason(t_arbre_foret *a, double lumiere)
{
    t_queue G;
    t_rameau *b;
    int n;
    int i;

    G = createEmptyQueue();

    // nouvelle saison : les nouveaux rameaux portent ce numero
    a->step.saison = a->step.saison + 1;

    while (!isEmptyQueue(a->step.bourgeons))
    {
        b = dequeue(&a->step.bourgeons);

        if (canGrowInLight(lumiere))
        {
            n = howManyBranchesInLight(lumiere);
            growBudInLight(a, b, n, lumiere);

            for (i = 0; i < n; i++)
            {
                enqueue(&G, b->p_fils[i]);
            }
        }
        else
        {
            enqueue(&G, b);    // il reste bourgeon
        }
    }
    a->step.bourgeons = G;
}
