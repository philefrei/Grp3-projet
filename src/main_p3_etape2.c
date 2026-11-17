/* main_p3_etape2.c : partie 3, tests de l'étape 2 (cible CMake p3_etape_2) — code fourni
   histogrammes de petits fichiers, dont les effectifs se calculent à la main, puis leur dessin par histoToSVG */

#include <stdio.h>
#include <stdlib.h>
#include "statistiques.h"

/***
 * @brief lit une colonne, calcule et affiche son histogramme, le dessine dans un fichier SVG, puis affiche les effectifs attendus
 * @param nom_fichier : fichier CSV
 * @param colonne : nom de la colonne
 * @param nb_classes : nombre de classes
 * @param nom_svg : fichier SVG à écrire
 * @param attendu : histogramme attendu
 */
static void testHistogram(const char *nom_fichier, const char *colonne, int nb_classes, const char *nom_svg,
                          const char *attendu)
{
    int n;
    double min, largeur;
    double *v = readColumn(nom_fichier, colonne, &n);
    int *effectifs;

    printf("--- %s, colonne %s, %d classes\n", nom_fichier, colonne, nb_classes);
    if (v != NULL)
    {
        displaySummary(v, n);
        effectifs = computeHistogram(v, n, nb_classes, &min, &largeur);
        printf("minimum %.4f, largeur d'une classe %.4f\n", min, largeur);
        displayHistogram(effectifs, nb_classes, min, largeur);
        histoToSVG(effectifs, nb_classes, min, largeur, nom_fichier, nom_svg);
        printf("dessin : %s\n", nom_svg);
        free(effectifs);
        free(v);
    }
    printf("attendu : %s\n\n", attendu);
}

int main(void)
{
    testHistogram("data/test_analyse.csv", "valeur", 5, "output/test_analyse_5.svg",
                  "minimum 1, largeur 1,8 ; 2 valeurs dans chaque classe");
    testHistogram("data/test_analyse.csv", "valeur", 3, "output/test_analyse_3.svg",
                  "minimum 1, largeur 3 ; effectifs 3, 3 et 4 (la derniere classe contient le maximum 10)");
    testHistogram("data/test_asymetrie.csv", "valeur", 5, "output/test_asymetrie_5.svg",
                  "minimum 1, largeur 1 ; effectifs 4, 0, 0, 0 et 1");
    testHistogram("data/test_histo.csv", "note", 10, "output/test_histo_10.svg",
                  "minimum 3,49, largeur 1,532 ; effectifs 9, 7, 18, 25, 36, 40, 23, 26, 12 et 4");

    return 0;
}
