/* campagne.c : partie 3, étape 1 — une campagne de simulations : R forêts, mesures écrites dans deux fichiers CSV (fichier fourni vide, à compléter) */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "campagne.h"

t_campagne createCampaign(void)
{
    t_campagne c;

    c.K = 50;
    c.E = 3;
    c.A = 0.25;
    c.S = 120;
    c.R = 1;
    c.graine = 1;
    c.identique = 0;
    strcpy(c.sortie, "output/campagne");

    return c;
}

static void writeSettings(FILE *fic, t_campagne c)
{
    fprintf(fic, "# K=%d E=%d A=%.3f S=%d R=%d graine=%d identique=%d HMAX=%d P_max=%d\n",
            c.K, c.E, c.A, c.S, c.R, c.graine, c.identique, H_MAX, P_MAX);
}

static int checkCampaign(t_campagne c)
{
    int ok = 1;

    if (c.K <= 0 || c.E <= 0 || c.S < 0 || c.R <= 0 || c.A < 0 || c.A > 1)
    {
        printf("Reglages invalides : il faut K > 0, E > 0, S >= 0, R > 0 et 0 <= A <= 1\n");
        ok = 0;
    }

    return ok;
}

int runCampaign(t_campagne c)
{
    char nom_arbres[LONGUEUR_NOM + 16];
    char nom_forets[LONGUEUR_NOM + 16];
    FILE *fic_arbres = NULL;
    FILE *fic_forets = NULL;
    t_foret f;
    t_arbre_foret *a;
    clock_t debut;
    long rameaux;
    double somme_cime, somme_extension;
    int ok;
    int r, k;

    ok = checkCampaign(c);

    if (ok)
    {
        sprintf(nom_arbres, "%s_arbres.csv", c.sortie);
        sprintf(nom_forets, "%s_forets.csv", c.sortie);
        fic_arbres = fopen(nom_arbres, "w");
        fic_forets = fopen(nom_forets, "w");

        if (fic_arbres == NULL || fic_forets == NULL)
        {
            printf("Impossible de creer %s\n", fic_arbres == NULL ? nom_arbres : nom_forets);
            if (fic_arbres != NULL)
            {
                fclose(fic_arbres);
            }
            if (fic_forets != NULL)
            {
                fclose(fic_forets);
            }
            ok = 0;
        }
    }

    if (ok)
    {
        writeSettings(fic_arbres, c);
        fprintf(fic_arbres, "foret;arbre;rameaux;hauteur;cime;extension;tronque\n");
        writeSettings(fic_forets, c);
        fprintf(fic_forets, "foret;rameaux;cime_moy;extension_moy\n");

        debut = clock();

        for (r = 0; r < c.R; r++)
        {
            f = createForest(c.K, c.E, H_MAX, c.A, P_MAX);
            plantForest(&f, c.graine + r, c.identique);
            simulateForest(&f, c.S);

            rameaux = 0;
            somme_cime = 0;
            somme_extension = 0;
            for (k = 0; k < f.K; k++)
            {
                a = &f.arbres[k];
                fprintf(fic_arbres, "%d;%d;%d;%d;%.3f;%.3f;%d\n", r, a->id, a->step.arbre.nb_rameaux, a->hauteur,
                        a->cime, a->xmax - a->xmin, a->tronque);
                rameaux += a->step.arbre.nb_rameaux;
                somme_cime += a->cime;
                somme_extension += a->xmax - a->xmin;
            }
            fprintf(fic_forets, "%d;%ld;%.4f;%.4f\n", r, rameaux, somme_cime / f.K, somme_extension / f.K);

            printf("foret %d (graine %d) : %ld rameaux, cime moyenne %.4f, extension moyenne %.4f\n",
                   r, c.graine + r, rameaux, somme_cime / f.K, somme_extension / f.K);

            freeForest(&f);
        }

        printf("%d foret(s) simulee(s) en %.2f s\n", c.R, (double) (clock() - debut) / CLOCKS_PER_SEC);
        printf("fichiers ecrits : %s et %s\n", nom_arbres, nom_forets);

        fclose(fic_arbres);
        fclose(fic_forets);
    }

    return ok;
}
