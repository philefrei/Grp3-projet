/* main_p3_etape1.c : partie 3, étape 1 — le programme de campagne, réglé par la ligne de commande (cible CMake p3_etape_1) (fichier fourni vide, à compléter)
   exemple : p3_etape_1 --K 50 --E 3 --A 0.25 --S 120 --R 40 --graine 101 --sortie output/C1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "campagne.h"

int main(int argc, char *argv[])
{
    t_campagne c = createCampaign();    // valeurs par défaut
    int ok = 1;
    int i;

    for (i = 1; i < argc && ok; i++)
    {
        if (strcmp(argv[i], "--K") == 0 && i + 1 < argc)
        {
            c.K = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--E") == 0 && i + 1 < argc)
        {
            c.E = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--A") == 0 && i + 1 < argc)
        {
            c.A = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "--S") == 0 && i + 1 < argc)
        {
            c.S = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--R") == 0 && i + 1 < argc)
        {
            c.R = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--graine") == 0 && i + 1 < argc)
        {
            c.graine = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--identique") == 0)
        {
            c.identique = 1;            // ce paramètre n'a pas de valeur
        }
        else if (strcmp(argv[i], "--sortie") == 0 && i + 1 < argc)
        {
            strncpy(c.sortie, argv[++i], LONGUEUR_NOM - 1);
            c.sortie[LONGUEUR_NOM - 1] = '\0';
        }
        else
        {
            printf("Parametre inconnu ou sans valeur : %s\n", argv[i]);
            ok = 0;
        }
    }

    if (ok)
    {
        ok = runCampaign(c);
    }

    return ok ? 0 : 1;
}