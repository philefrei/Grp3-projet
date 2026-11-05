/* main_p2_etape2.c : partie 2, tests de l'étape 2 (cible CMake p2_etape_2) — code fourni
   1 000 000 de tirages par lumière pour canGrowInLight, puis pour howManyBranchesInLight ;
   en pleine lumière, mêmes tirages que canGrow(95) puis howManyBranches(70, 25) */

#include <stdio.h>
#include "croissance_lumiere.h"
#include "croissance.h"
#include "alea.h"

#define NB_TIRAGES 1000000
#define NB_COMPARAISONS 1000

int main(void)
{
    double L[4] = {1, 0.5, 0.2, 0.1};
    int pousse;
    int compte[4];
    int a[NB_COMPARAISONS];
    int b[NB_COMPARAISONS];
    int differences = 0;

    // test : on initialise le générateur soi-même (dans la simulation, seule plantForest le fait)
    randomInit(1);

    // 1. le bourgeon pousse-t-il ?
    printf("canGrowInLight, nombre de 1 sur %d tirages :\n", NB_TIRAGES);
    for (int j = 0; j < 4; j++)
    {
        pousse = 0;
        for (int i = 0; i < NB_TIRAGES; i++)
        {
            pousse += canGrowInLight(L[j]);
        }
        printf("  L = %.1f : %7d\n", L[j], pousse);
    }
    printf("attendu, à environ 2 000 près : L = 1 : 950 000 ; L = 0,5 : 700 000 ; L = 0,2 : 400 000 ; L = 0,1 : 0\n");

    // 2. combien de rameaux, pour un bourgeon qui pousse ? (pas de L = 0,1 : un bourgeon en dormance ne pousse jamais)
    printf("howManyBranchesInLight, nombre de 1, 2 et 3 rameaux sur %d tirages :\n", NB_TIRAGES);
    for (int j = 0; j < 3; j++)
    {
        for (int k = 0; k < 4; k++)
        {
            compte[k] = 0;
        }
        for (int i = 0; i < NB_TIRAGES; i++)
        {
            compte[howManyBranchesInLight(L[j])]++;
        }
        printf("  L = %.1f : %7d %7d %7d\n", L[j], compte[1], compte[2], compte[3]);
    }
    printf("attendu, à environ 2 000 près : L = 1 : 700 000 250 000 50 000 ; L = 0,5 : 850 000 150 000 0 ; L = 0,2 : 1 000 000 0 0\n");

    // 3. L = 1 : exactement les tirages de la partie 1, dans le même ordre (0 = le bourgeon ne pousse pas)
    randomInit(1);
    for (int i = 0; i < NB_COMPARAISONS; i++)
    {
        a[i] = 0;
        if (canGrow(95))
        {
            a[i] = howManyBranches(70, 25);
        }
    }
    randomInit(1);
    for (int i = 0; i < NB_COMPARAISONS; i++)
    {
        b[i] = 0;
        if (canGrowInLight(1.0))
        {
            b[i] = howManyBranchesInLight(1.0);
        }
    }
    for (int i = 0; i < NB_COMPARAISONS; i++)
    {
        if (a[i] != b[i])
        {
            differences++;
        }
    }
    printf("L = 1 contre canGrow(95) puis howManyBranches(70, 25) : %d différence(s) sur %d tirages (attendu : 0)\n",
           differences, NB_COMPARAISONS);

    return 0;
}
