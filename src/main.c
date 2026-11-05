/* main.c : votre programme principal */

#include <stdio.h>
#include "arbre.h"
#include "dessin.h"
#include "arbres_utils.h"
#include "alea.h"
#include "croissance.h"
#include "croissance_step.h"
#include "sauver_arbre.h"

int main(void)
{
    t_arbre a = createTree(0, 1, 3);
    growTree(&a, 8);
    saveTree(a, "output/etape3_growTree.txt");

    t_arbre_step s = createTreeStep(0, 1, 3);
    for (int i = 0; i < 8; i++)
        growTreeStep(&s);
    clearBuds(&s);
    saveTree(s.arbre, "output/etape3_step.txt");

    printf("growTree : %d rameaux\n", a.nb_rameaux);
    printf("growTreeStep : %d rameaux\n", s.arbre.nb_rameaux);

    return 0;
}
