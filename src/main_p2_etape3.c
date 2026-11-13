/* main_p2_etape3.c : partie 2, tests de l'étape 3 (cible CMake p2_etape_3) — code fourni
   en pleine lumière, growSeason redonne les arbres de la partie 1 ; le test automatique des mesures tenues à jour est à écrire */

#include <stdio.h>
#include <math.h>
#include "arbre_foret.h"
#include "arbres_utils.h"

/***
 * @brief compare deux arbres : même structure, mêmes numéros, mêmes extrémités à tol près
 * @param a, b : racines des deux sous-arbres
 * @param tol : écart maximal accepté sur x et y (les fichiers arrondissent à 0,01)
 * @return 1 si les deux arbres sont identiques, 0 sinon
 */
static int sameTrees(t_rameau *a, t_rameau *b, float tol)
{
    int res = a->num == b->num && a->saison == b->saison && a->gen == b->gen && a->nb_fils == b->nb_fils
              && fabsf(a->x - b->x) <= tol && fabsf(a->y - b->y) <= tol;

    for (int i = 0; i < a->nb_fils && res; i++)
    {
        res = sameTrees(a->p_fils[i], b->p_fils[i], tol);
    }

    return res;
}

int main(void)
{
    const char *noms[3] = {"data/arbre_petit.txt", "data/arbre_moyen.txt", "data/arbre_grand.txt"};
    int graines[3] = {5, 3, 4};
    int saisons[3] = {3, 8, 30};
    double L[2] = {1.0, 0.5};
    t_arbre ref;
    t_arbre_foret a;

    // 1. pleine lumière : les arbres de la partie 1
    for (int j = 0; j < 3; j++)
    {
        ref = readTree(noms[j]);
        a = createForestTree(0, 0, 1, graines[j]);
        for (int s = 0; s < saisons[j]; s++)
        {
            growSeason(&a, 1.0);
        }
        printf("%-21s graine %d, %2d saisons : %4d rameaux, cime %.3f : %s\n", noms[j], graines[j], saisons[j],
               a.step.arbre.nb_rameaux, a.cime, sameTrees(ref.pied, a.step.arbre.pied, 0.05f) ? "identique" : "DIFFERENT");
        freeForestTree(&a);
        freeBranches(ref.pied);
    }
    printf("attendu : identique pour les trois ; arbre_grand 5518 rameaux ; cime d'arbre_petit 4,130 (4,135 pour l'arbre lu)\n");

    // 2. À ÉCRIRE : le test automatique de la validation de l'étape 3
    //    pour 100 graines, 20 saisons, avec L = 1 puis L = 0,5 : après chaque saison, les mesures tenues à jour
    //    sont exactement celles des fonctions récursives, la hauteur ne dépasse pas le numéro de la saison
    //    et elle augmente d'au plus 1 ; affichez le nombre de problèmes trouvés (attendu : 0)
    for (int j = 0; j < 2; j++)
    {
        int problemes = 0;

        for (int graine = 1; graine <= 100; graine++)
        {
            a = createForestTree(0, 0, 1, graine);

            for (int s = 1; s <= 20; s++)
            {
                int hauteur_avant = a.hauteur;
                t_rameau *pied;

                growSeason(&a, L[j]);
                pied = a.step.arbre.pied;

                // les mesures tenues à jour sont exactement celles des fonctions récursives
                if (a.step.arbre.nb_rameaux != countBranches(pied)) problemes++;
                if (a.hauteur != computeHeight(pied)) problemes++;
                if (a.cime != computeTop(pied)) problemes++;
                if (a.xmin != computeXMin(pied)) problemes++;
                if (a.xmax != computeXMax(pied)) problemes++;
                if (a.nb_bourgeons != countBuds(pied)) problemes++;

                // la hauteur ne dépasse pas le numéro de la saison, et augmente d'au plus 1
                if (a.hauteur > s) problemes++;
                if (a.hauteur > hauteur_avant + 1) problemes++;
            }
            freeForestTree(&a);
        }
        printf("test automatique, L = %.1f, 100 graines, 20 saisons : %d probleme(s) (attendu : 0)\n", L[j], problemes);
    }

    // 3. graine 1, 20 saisons
    for (int j = 0; j < 2; j++)
    {
        a = createForestTree(0, 0, 1, 1);
        for (int s = 0; s < 20; s++)
        {
            growSeason(&a, L[j]);
        }
        printf("graine 1, 20 saisons, L = %.1f : %d rameaux, hauteur %d, cime %.3f, extension %.3f, %d bourgeons\n",
               L[j], a.step.arbre.nb_rameaux, a.hauteur, a.cime, a.xmax - a.xmin, a.nb_bourgeons);
        freeForestTree(&a);
    }
    printf("attendu : L = 1 : 493, 20, 19,869, 31,761, 117 ; L = 0,5 : 62, 19, 14,509, 8,358, 10\n");

    return 0;
}
