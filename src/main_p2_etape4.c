/* main_p2_etape4.c : partie 2, tests de l'étape 4 (cible CMake p2_etape_4) — code fourni
   colonnes et rangées ; occupation de l'arbre exemple1.txt seul dans une forêt de 10 colonnes et 8 rangées */

#include <stdio.h>
#include <stdlib.h>
#include "foret.h"
#include "arbres_utils.h"

/***
 * @brief affiche la grille d'occupation, la rangée la plus haute en premier
 * @param f : la forêt
 */
static void displayOccupation(t_foret *f)
{
    for (int r = f->HMAX - 1; r >= 0; r--)
    {
        printf("  rangée %d :", r);
        for (int c = 0; c < f->C; c++)
        {
            printf(" %d", f->occupation[r][c]);
        }
        printf("\n");
    }

    return;
}

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

int main(void)
{
    t_foret f = createForest(3, 4, 160, 0.25, 20000);
    t_foret g;
    int somme = 0;

    // la forêt de l'exemple : 12 colonnes
    printf("columnOf 6.4 : %d, 11.99 : %d, 12.3 : %d, -0.5 : %d ; rowOf 2.7 : %d, 163.2 : %d\n",
           columnOf(&f, 6.4f), columnOf(&f, 11.99f), columnOf(&f, 12.3f), columnOf(&f, -0.5f), rowOf(&f, 2.7f), rowOf(&f, 163.2f));
    printf("attendu :      6           11           0           11            2              159\n");
    freeArrays(&f);

    // exemple1.txt dans une forêt de 1 arbre, 10 colonnes, 8 rangées (seul le champ step.arbre est rempli)
    g = createForest(1, 10, 8, 0.5, 20000);
    g.arbres[0].step.arbre = readTree("data/exemple1.txt");
    addTreeToGrid(&g, g.arbres[0].step.arbre.pied);
    displayOccupation(&g);
    for (int r = 0; r < g.HMAX; r++)
    {
        for (int c = 0; c < g.C; c++)
        {
            somme += g.occupation[r][c];
        }
    }
    printf("somme des compteurs : %d (attendu : 10) ; attendu, colonnes 3 4 5 : rangée 4 : 0 1 1 ; 3 : 0 1 0 ; 2 : 0 3 1 ; 1 : 2 1 0\n", somme);
    freeBranches(g.arbres[0].step.arbre.pied);
    freeArrays(&g);

    // À ÉCRIRE : deux ou trois autres cas construits à la main, dont un rameau d'altitude supérieure à HMAX
    //    (il est compté dans la dernière rangée)

    return 0;
}
