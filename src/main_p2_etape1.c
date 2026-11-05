/* main_p2_etape1.c : partie 2, tests de l'étape 1 (cible CMake p2_etape_1) — code fourni
   mesures récursives des arbres de la partie 1, lus avec readTree ; arbre créé par createForestTree */

#include <stdio.h>
#include "arbre_foret.h"
#include "arbres_utils.h"

/***
 * @brief lit un arbre, affiche ses mesures récursives, puis le libère
 * @param nom_fichier : fichier écrit au format de saveTree
 */
static void measureFile(const char *nom_fichier)
{
    t_arbre a = readTree(nom_fichier);

    printf("%-22s : rameaux %5d (nb_rameaux %5d), hauteur %2d, cime %.3f, extension %.3f, bourgeons %4d\n",
           nom_fichier, countBranches(a.pied), a.nb_rameaux, computeHeight(a.pied), computeTop(a.pied),
           computeXMax(a.pied) - computeXMin(a.pied), countBuds(a.pied));
    // option computeExtension : à vous d'ajouter son test ici, elle doit donner la même extension
    freeBranches(a.pied);

    return;
}

int main(void)
{
    t_arbre_foret a;

    printf("attendu : exemple1 10 4 4,893 1,638 5 ; petit 10 3 4,135 2,675 4 ; moyen 38 8 8,311 9,280 10 ; grand 5518 30 30,926 46,720 1432\n");
    measureFile("data/exemple1.txt");
    measureFile("data/arbre_petit.txt");
    measureFile("data/arbre_moyen.txt");
    measureFile("data/arbre_grand.txt");

    // un arbre créé : ses mesures sont celles que calculent les fonctions récursives
    a = createForestTree(0, 0, 1, 1);
    printf("createForestTree(0, 0, 1, 1) : tenues à jour  %d rameau, hauteur %d, cime %.0f, extension %.0f, %d bourgeon\n",
           a.step.arbre.nb_rameaux, a.hauteur, a.cime, a.xmax - a.xmin, a.nb_bourgeons);
    printf("                               récursives     %d rameau, hauteur %d, cime %.0f, extension %.0f, %d bourgeon\n",
           countBranches(a.step.arbre.pied), computeHeight(a.step.arbre.pied), computeTop(a.step.arbre.pied),
           computeXMax(a.step.arbre.pied) - computeXMin(a.step.arbre.pied), countBuds(a.step.arbre.pied));
    freeForestTree(&a);

    return 0;
}
