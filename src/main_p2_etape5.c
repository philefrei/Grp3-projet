/* main_p2_etape5.c : partie 2, tests de l'étape 5 (cible CMake p2_etape_5) — code fourni
   colonne de l'exemple construite à la main ; lumière de l'arbre exemple1.txt seul dans une forêt 10 x 8, A = 0,5 */

#include <stdio.h>
#include <stdlib.h>
#include "foret.h"
#include "arbres_utils.h"

/***
 * @brief libère les tableaux d'une forêt dont les arbres n'ont pas été plantés (test seulement)
 * @param f : la forêt
 */
static void freeArrays(t_foret *f)
{
    free(f->arbres);
    for (int r = 0; r < f->HMAX; r++)
    {
        free(f->occupation[r]);
        free(f->lumiere[r]);
    }
    free(f->occupation);
    free(f->lumiere);

    return;
}

/***
 * @brief cherche le rameau de numéro num dans le sous-arbre de r
 * @param r : racine du sous-arbre
 * @param num : numéro cherché
 * @return le rameau, ou NULL s'il n'est pas dans le sous-arbre
 */
static t_rameau *findBranch(t_rameau *r, int num)
{
    t_rameau *res = NULL;

    if (r->num == num)
    {
        res = r;
    }
    for (int i = 0; i < r->nb_fils && res == NULL; i++)
    {
        res = findBranch(r->p_fils[i], num);
    }

    return res;
}

int main(void)
{
    t_foret f = createForest(1, 1, 12, 0.5, 20000);
    t_foret g;
    int bourgeons[5] = {4, 5, 7, 8, 9};

    // une colonne : deux extrémités en rangée 5, une en rangée 9 (occupation écrite à la main)
    f.occupation[5][0] = 2;
    f.occupation[9][0] = 1;
    computeLight(&f);
    printf("colonne à la main, A = 0,5 :");
    for (int r = f.HMAX - 1; r >= 0; r--)
    {
        printf(" %g", f.lumiere[r][0]);
    }
    printf("\nattendu : 1 dans les rangées 9 et au-dessus, 0,5 dans les rangées 5 à 8, 0,125 dans les rangées 0 à 4\n");
    freeArrays(&f);

    // exemple1.txt, forêt de 10 colonnes et 8 rangées, A = 0,5
    g = createForest(1, 10, 8, 0.5, 20000);
    g.arbres[0].step.arbre = readTree("data/exemple1.txt");
    addTreeToGrid(&g, g.arbres[0].step.arbre.pied);
    computeLight(&g);
    for (int r = 4; r >= 0; r--)
    {
        printf("  rangée %d, colonnes 3 4 5 : %g %g %g\n", r, g.lumiere[r][3], g.lumiere[r][4], g.lumiere[r][5]);
    }
    printf("attendu : 4 : 1 1 1 ; 3 : 1 0,5 0,5 ; 2 : 1 0,25 0,5 ; 1 : 1 0,03125 0,25 ; 0 : 0,25 0,015625 0,25\n");
    printf("bourgeons 4, 5, 7, 8, 9 :");
    for (int i = 0; i < 5; i++)
    {
        printf(" %g", getLight(&g, findBranch(g.arbres[0].step.arbre.pied, bourgeons[i])));
    }
    printf("\nattendu : 0,5 0,25 1 1 1\n");
    freeBranches(g.arbres[0].step.arbre.pied);
    freeArrays(&g);

    return 0;
}
