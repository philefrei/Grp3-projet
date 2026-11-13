/* foret.c : partie 2, étapes 4 à 6 — la forêt, ses grilles d'occupation et de lumière, sa pousse (code fourni, à compléter) */

#include <math.h>
#include "foret.h"
#include <stdlib.h>
#include "alea.h"
#include "croissance_lumiere.h"

/* ---------------------------------------------------------------- code fourni */

int columnOf(t_foret *f, float x)
{
    // en C, -1 % 12 vaut -1 : on ramène le reste entre 0 et C - 1
    int c = (int) floorf(x) % f->C;

    if (c < 0)
    {
        c += f->C;
    }

    return c;
}

int rowOf(t_foret *f, float y)
{
    int r = (int) floorf(y);

    if (r >= f->HMAX)
    {
        r = f->HMAX - 1;
    }
    else if (r < 0)
    {
        r = 0;      // ne se produit pas (un fils est toujours plus haut que son parent) : simple précaution
    }

    return r;
}

/* ---------------------------------------------------------------- à compléter : vos fonctions, à partir d'ici */

t_foret createForest(int K, int E, int HMAX, double A, int P_max)
{
    t_foret f;
    int r;
    int c;
    f.K = K;
    f.E = E;
    f.C = K * E;
    f.HMAX = HMAX;
    f.A = A;
    f.P_max = P_max;
    f.saison = 0;
    f.arbres = malloc(K * sizeof(t_arbre_foret));
    f.occupation = malloc(HMAX * sizeof(int *));
    f.lumiere = malloc(HMAX * sizeof(double *));
    for (r = 0; r < HMAX; r++)
    {
        f.occupation[r] = malloc(f.C * sizeof(int));
        f.lumiere[r] = malloc(f.C * sizeof(double));
        for (c = 0; c < f.C; c++)
        {
            f.occupation[r][c] = 0;
            f.lumiere[r][c] = 1;
        }
    }
    return f;
}
void addToGrid(t_foret *f, t_rameau *r)
{
    f->occupation[rowOf(f, r->y)][columnOf(f, r->x)]++;
}
void addTreeToGrid(t_foret *f, t_rameau *r)
{
    int i;
    addToGrid(f, r);
    for (i = 0; i < r->nb_fils; i++)
    {
        addTreeToGrid(f, r->p_fils[i]);
    }
}
/* ---------------- etape 5 : la lumiere */
void computeLight(t_foret *f)
{
    int r;
    int c;
    int k;
    double L;
    for (c = 0; c < f->C; c++)
    {
        L = 1;
        for (r = f->HMAX - 1; r >= 0; r--)
        {
            f->lumiere[r][c] = L;
            for (k = 0; k < f->occupation[r][c]; k++)
            {
                L = L * (1 - f->A);
            }
        }
    }
}
double getLight(t_foret *f, t_rameau *r)
{
    return f->lumiere[rowOf(f, r->y)][columnOf(f, r->x)];
}