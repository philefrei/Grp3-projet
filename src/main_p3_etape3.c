#include <stdio.h>
#include <stdlib.h>
#include "statistiques.h"

int main(int argc, char *argv[])
{
    const char *nom_fichier;
    const char *colonne;
    const char *nom_svg = "output/histogramme.svg";
    int nb_classes = 20;
    double *v;
    int n;
    int *effectifs;
    double min, largeur;

    if (argc < 3)
    {
        printf("usage : %s fichier colonne [nombre_de_classes [fichier_svg]]\n", argv[0]);
        printf("exemple : %s output/C1_arbres.csv cime 24 output/C1_cime.svg\n", argv[0]);
        return 1;
    }

    nom_fichier = argv[1];
    colonne = argv[2];
    if (argc >= 4)
    {
        nb_classes = atoi(argv[3]);
    }
    if (argc >= 5)
    {
        nom_svg = argv[4];
    }
    if (nb_classes <= 0)
    {
        printf("nombre de classes invalide : %s\n", argv[3]);
        return 1;
    }

    v = readColumn(nom_fichier, colonne, &n);
    if (v == NULL)
    {
        return 1;                   // readColumn a déjà affiché un message
    }
    if (n == 0)
    {
        printf("aucune valeur dans la colonne %s\n", colonne);
        free(v);
        return 1;
    }

    printf("--- %s, colonne %s, %d classes\n", nom_fichier, colonne, nb_classes);
    displaySummary(v, n);
    effectifs = computeHistogram(v, n, nb_classes, &min, &largeur);
    printf("minimum %.4f, largeur d'une classe %.4f\n", min, largeur);
    displayHistogram(effectifs, nb_classes, min, largeur);
    histoToSVG(effectifs, nb_classes, min, largeur, colonne, nom_svg);
    printf("dessin : %s\n", nom_svg);

    free(effectifs);
    free(v);
    return 0;
}
