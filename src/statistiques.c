/* statistiques.c : partie 3, étape 2 — lire une colonne d'un fichier CSV, la résumer, son histogramme (code fourni, à compléter) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "statistiques.h"

#define LONGUEUR_LIGNE 1024

/* ---------------------------------------------------------------- code fourni */

/***
 * @brief lit la prochaine ligne du fichier qui n'est pas un commentaire (lignes commençant par '#'), sans son retour à la ligne
 * @param f : fichier ouvert en lecture
 * @param ligne : reçoit la ligne (LONGUEUR_LIGNE caractères au plus)
 * @return 1 si une ligne a été lue, 0 à la fin du fichier
 */
static int readDataLine(FILE *f, char ligne[])
{
    int lu = fgets(ligne, LONGUEUR_LIGNE, f) != NULL;

    while (lu && ligne[0] == '#')
    {
        lu = fgets(ligne, LONGUEUR_LIGNE, f) != NULL;
    }
    if (lu)
    {
        ligne[strcspn(ligne, "\r\n")] = '\0';
    }

    return lu;
}

/***
 * @brief renvoie le champ numéro num (à partir de 0) d'une ligne dont les champs sont séparés par ';' ; la ligne est modifiée
 * @return le champ, ou NULL si la ligne a moins de num + 1 champs
 */
static char *fieldOf(char ligne[], int num)
{
    char *champ = strtok(ligne, ";");

    for (int i = 0; i < num && champ != NULL; i++)
    {
        champ = strtok(NULL, ";");
    }

    return champ;
}

double *readColumn(const char *nom_fichier, const char *colonne, int *p_n)
{
    FILE *f = fopen(nom_fichier, "rt");
    char ligne[LONGUEUR_LIGNE];
    double *v = NULL;
    int capacite = 1024;
    int num = -1;

    *p_n = 0;
    if (f == NULL)
    {
        perror(nom_fichier);
    }
    else
    {
        // l'en-tête : numéro de la colonne
        if (readDataLine(f, ligne))
        {
            char *champ = strtok(ligne, ";");

            for (int i = 0; champ != NULL && num == -1; i++)
            {
                if (strcmp(champ, colonne) == 0)
                {
                    num = i;
                }
                champ = strtok(NULL, ";");
            }
        }
        if (num == -1)
        {
            fprintf(stderr, "%s : pas de colonne %s\n", nom_fichier, colonne);
        }
        else
        {
            // les données : tableau agrandi (doublé) quand il est plein
            v = malloc(capacite * sizeof(double));
            while (v != NULL && readDataLine(f, ligne))
            {
                char *champ = fieldOf(ligne, num);

                if (champ != NULL)
                {
                    if (*p_n == capacite)
                    {
                        capacite *= 2;
                        v = realloc(v, capacite * sizeof(double));
                    }
                    if (v != NULL)
                    {
                        v[(*p_n)++] = atof(champ);
                    }
                }
            }
            if (v == NULL)
            {
                printf("CRITICAL ERROR : cannot allocate values\n");
                exit(-1);
            }
        }
        fclose(f);
    }

    return v;
}

/***
 * @brief comparaison de deux réels pour qsort : négatif, nul ou positif selon que *a est plus petit, égal ou plus grand que *b
 */
static int compareValues(const void *a, const void *b)
{
    double x = *(const double *) a;
    double y = *(const double *) b;

    return (x > y) - (x < y);
}

void sortValues(double v[], int n)
{
    qsort(v, n, sizeof(double), compareValues);
}

double computeMean(double v[], int n)
{
    double somme = 0;

    for (int i = 0; i < n; i++)
    {
        somme += v[i];
    }

    return somme / n;
}

double computeStdDev(double v[], int n)
{
    double m = computeMean(v, n);
    double somme = 0;

    for (int i = 0; i < n; i++)
    {
        somme += (v[i] - m) * (v[i] - m);
    }

    return sqrt(somme / n);
}

double computeSkewness(double v[], int n)
{
    double m = computeMean(v, n);
    double s = computeStdDev(v, n);
    double somme = 0;
    double res = 0;

    if (s > 0)
    {
        for (int i = 0; i < n; i++)
        {
            somme += (v[i] - m) * (v[i] - m) * (v[i] - m);
        }
        res = somme / (n * s * s * s);
    }

    return res;
}

double computeMedian(double trie[], int n)
{
    double res;

    if (n % 2 == 1)
    {
        res = trie[n / 2];
    }
    else
    {
        res = (trie[n / 2 - 1] + trie[n / 2]) / 2;
    }

    return res;
}

double computeQuartile(double trie[], int n, int q)
{
    int rang = (q * n + 3) / 4;     // ceil(q n / 4), en entiers

    return trie[rang - 1];
}

void displaySummary(double v[], int n)
{
    double *trie = malloc(n * sizeof(double));

    if (trie == NULL)
    {
        printf("CRITICAL ERROR : cannot allocate values\n");
        exit(-1);
    }
    memcpy(trie, v, n * sizeof(double));
    sortValues(trie, n);

    printf("nombre de valeurs : %d\n", n);
    printf("moyenne           : %.4f\n", computeMean(v, n));
    printf("ecart-type        : %.4f\n", computeStdDev(v, n));
    printf("minimum           : %.4f\n", trie[0]);
    printf("maximum           : %.4f\n", trie[n - 1]);
    printf("mediane           : %.4f\n", computeMedian(trie, n));
    printf("quartiles         : %.4f et %.4f\n", computeQuartile(trie, n, 1), computeQuartile(trie, n, 3));
    printf("asymetrie         : %.4f\n", computeSkewness(v, n));

    free(trie);
}

/* dimensions du dessin SVG de histoToSVG, en pixels */
#define SVG_LARGEUR 900
#define SVG_HAUTEUR 520
#define SVG_MARGE_GAUCHE 80
#define SVG_MARGE_DROITE 30
#define SVG_MARGE_HAUT 60
#define SVG_MARGE_BAS 80

void histoToSVG(int effectifs[], int nb_classes, double min, double largeur, const char *titre, const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "wt");
    int plus_grand = 1;
    double l_dessin = SVG_LARGEUR - SVG_MARGE_GAUCHE - SVG_MARGE_DROITE;      // largeur de la zone des barres
    double h_dessin = SVG_HAUTEUR - SVG_MARGE_HAUT - SVG_MARGE_BAS;          // hauteur de la zone des barres
    double l_barre = l_dessin / nb_classes;
    double y_axe = SVG_HAUTEUR - SVG_MARGE_BAS;                               // ordonnée de l'axe horizontal
    int pas = (nb_classes + 9) / 10;        // une borne écrite toutes les pas classes : 10 bornes au plus

    if (f == NULL)
    {
        perror(nom_fichier);
    }
    else
    {
        for (int i = 0; i < nb_classes; i++)
        {
            if (effectifs[i] > plus_grand)
            {
                plus_grand = effectifs[i];
            }
        }
        fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" "
                   "font-family=\"sans-serif\" font-size=\"13\">\n", SVG_LARGEUR, SVG_HAUTEUR, SVG_LARGEUR, SVG_HAUTEUR);
        fprintf(f, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");
        fprintf(f, "<text x=\"%d\" y=\"32\" text-anchor=\"middle\" font-size=\"18\" font-weight=\"bold\">%s</text>\n",
                SVG_LARGEUR / 2, titre);

        // axe vertical : 5 graduations, de 0 au plus grand effectif
        for (int k = 0; k <= 5; k++)
        {
            double y = y_axe - h_dessin * k / 5;

            fprintf(f, "<line x1=\"%d\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"#DDDDDD\"/>\n",
                    SVG_MARGE_GAUCHE, y, SVG_MARGE_GAUCHE + l_dessin, y);
            fprintf(f, "<text x=\"%d\" y=\"%.1f\" text-anchor=\"end\">%.0f</text>\n",
                    SVG_MARGE_GAUCHE - 8, y + 4, (double) plus_grand * k / 5);
        }

        // une barre par classe, de hauteur proportionnelle à son effectif
        for (int i = 0; i < nb_classes; i++)
        {
            double h = h_dessin * effectifs[i] / plus_grand;

            fprintf(f, "<rect x=\"%.1f\" y=\"%.1f\" width=\"%.1f\" height=\"%.1f\" fill=\"#4C8C5A\" stroke=\"white\"/>\n",
                    SVG_MARGE_GAUCHE + i * l_barre, y_axe - h, l_barre, h);
        }

        // axe horizontal et bornes des classes
        fprintf(f, "<line x1=\"%d\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"black\"/>\n",
                SVG_MARGE_GAUCHE, y_axe, SVG_MARGE_GAUCHE + l_dessin, y_axe);
        for (int i = 0; i <= nb_classes; i++)
        {
            if (i % pas == 0 || i == nb_classes)
            {
                double x = SVG_MARGE_GAUCHE + i * l_barre;

                fprintf(f, "<line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\" stroke=\"black\"/>\n", x, y_axe, x, y_axe + 6);
                fprintf(f, "<text x=\"%.1f\" y=\"%.1f\" text-anchor=\"middle\">%.2f</text>\n", x, y_axe + 22, min + i * largeur);
            }
        }
        fprintf(f, "<text x=\"%d\" y=\"%d\" text-anchor=\"middle\" fill=\"#555555\">%d classes de largeur %.3f</text>\n",
                SVG_LARGEUR / 2, SVG_HAUTEUR - 20, nb_classes, largeur);
        fprintf(f, "</svg>\n");
        fclose(f);
    }
}

/* ---------------------------------------------------------------- à compléter : vos fonctions, à partir d'ici */
