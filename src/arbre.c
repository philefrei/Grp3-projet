/* arbre.c : module fourni (structures et construction d'un arbre) */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "arbre.h"
#include "alea.h"

t_rameau *createBranch(float x, float y, float dir, float longueur, int saison, int gen, int num)
{
    t_rameau *r = malloc(sizeof(t_rameau));
    if (r == NULL)
    {
        perror("createBranch");
        exit(EXIT_FAILURE);
    }
    r->num = num;
    r->longueur = longueur;
    r->x = x;
    r->y = y;
    r->dir = dir;
    r->saison = saison;
    r->gen = gen;
    r->etat = BOURGEON;
    r->nb_fils = 0;
    r->p_fils = NULL;
    return r;
}

t_arbre createTree(float x, float y, int seed)
{
    t_arbre a;
    a.pied = (t_rameau *) malloc(sizeof(t_rameau));
    *(a.pied) = (t_rameau){0, y, x, y, 0.0f, 0, 0, BOURGEON, 0, NULL};
    a.nb_rameaux = 1;
    a.seed = seed;
    randomInit((uint64_t) seed);
    return a;
}

t_rameau *growBranch(t_arbre *a, t_rameau *parent, int saison, float dir, float longueur)
{
    // extrémité du nouveau rameau : on part de l'extrémité du parent
    float x = parent->x + longueur * (float) sin(dir * PI / 180);
    float y = parent->y + longueur * (float) cos(dir * PI / 180);
    t_rameau *r = createBranch(x, y, dir, longueur, saison, parent->gen + 1, a->nb_rameaux);
    parent->etat = BOIS;
    a->nb_rameaux++;
    return r;
}
