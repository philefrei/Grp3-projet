/* main_p2_etape6.c : partie 2, tests de l'étape 6 (cible CMake p2_etape_6) — code fourni
   invariants de la forêt, arbre seul sans ombre, reproductibilité, forêt de validation, sauvegarde par saveForest */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "foret.h"
#include "sauver_foret.h"
#include "arbres_utils.h"

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

/***
 * @brief compare deux arbres : même structure, mêmes extrémités exactement
 * @param a, b : racines des deux sous-arbres
 * @return 1 si les deux arbres sont identiques, 0 sinon
 */
static int sameTrees(t_rameau *a, t_rameau *b)
{
    int res = a->num == b->num && a->nb_fils == b->nb_fils && a->x == b->x && a->y == b->y;

    for (int i = 0; i < a->nb_fils && res; i++)
    {
        res = sameTrees(a->p_fils[i], b->p_fils[i]);
    }

    return res;
}

/***
 * @brief compte les écarts entre les mesures tenues à jour et les mesures récursives, et entre l'occupation tenue à jour
 *        et celle d'une nouvelle grille remplie avec addTreeToGrid
 * @param f : la forêt
 * @return le nombre de problèmes trouvés
 */
static int checkForest(t_foret *f)
{
    int problemes = 0;
    long total = 0, occupation = 0;
    t_foret v = createForest(f->K, f->E, f->HMAX, f->A, f->P_max);
    t_rameau *p;

    for (int k = 0; k < f->K; k++)
    {
        p = f->arbres[k].step.arbre.pied;
        if (f->arbres[k].step.arbre.nb_rameaux != countBranches(p) || f->arbres[k].hauteur != computeHeight(p)
            || f->arbres[k].cime != computeTop(p) || f->arbres[k].xmin != computeXMin(p)
            || f->arbres[k].xmax != computeXMax(p) || f->arbres[k].nb_bourgeons != countBuds(p))
        {
            problemes++;
        }
        total += f->arbres[k].step.arbre.nb_rameaux;
        addTreeToGrid(&v, p);
    }
    for (int r = 0; r < f->HMAX; r++)
    {
        for (int c = 0; c < f->C; c++)
        {
            occupation += f->occupation[r][c];
            if (v.occupation[r][c] != f->occupation[r][c])
            {
                problemes++;
            }
        }
    }
    if (occupation != total)
    {
        problemes++;
    }
    // v n'a pas d'arbres plantés : ses tableaux seulement
    free(v.arbres);
    for (int r = 0; r < v.HMAX; r++)
    {
        free(v.occupation[r]);
        free(v.lumiere[r]);
    }
    free(v.occupation);
    free(v.lumiere);

    return problemes;
}

/***
 * @brief compare deux fichiers texte, octet par octet
 * @param nom1, nom2 : les deux fichiers
 * @return 1 s'ils sont identiques, 0 sinon (ou si l'un ne s'ouvre pas)
 */
static int sameFiles(const char *nom1, const char *nom2)
{
    FILE *f1 = fopen(nom1, "rb");
    FILE *f2 = fopen(nom2, "rb");
    int res = f1 != NULL && f2 != NULL;
    int c1, c2;
    int fin = 0;

    while (res && !fin)
    {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        // les fins de ligne peuvent différer selon le système : on ignore les '\r'
        while (c1 == '\r')
        {
            c1 = fgetc(f1);
        }
        while (c2 == '\r')
        {
            c2 = fgetc(f2);
        }
        if (c1 != c2)
        {
            res = 0;
        }
        else if (c1 == EOF)
        {
            fin = 1;
        }
    }
    if (f1 != NULL)
    {
        fclose(f1);
    }
    if (f2 != NULL)
    {
        fclose(f2);
    }

    return res;
}

int main(void)
{
    t_foret f, g;
    t_arbre ref;
    t_rameau *a, *b;
    int ok, problemes = 0, differences = 0;
    long total = 0;

    // 1. un seul arbre, A = 0, plantation identique, graine 3, 8 saisons : arbre_moyen.txt décalé de 5
    ref = readTree("data/arbre_moyen.txt");
    f = createForest(1, 10, 160, 0.0, 20000);
    plantForest(&f, 3, 1);
    simulateForest(&f, 8);
    ok = f.arbres[0].step.arbre.nb_rameaux == ref.nb_rameaux;
    for (int n = 0; n < ref.nb_rameaux && ok; n++)
    {
        a = findBranch(f.arbres[0].step.arbre.pied, n);
        b = findBranch(ref.pied, n);
        ok = a != NULL && b != NULL && a->nb_fils == b->nb_fils && fabsf(a->y - b->y) < 0.05f && fabsf(a->x - b->x - 5) < 0.05f;
    }
    printf("K = 1, A = 0, identique, graine 3, 8 saisons : %s arbre_moyen.txt décalé de 5 (attendu : identique à)\n",
           ok ? "identique à" : "DIFFÉRENT de");
    freeForest(&f);
    freeBranches(ref.pied);

    // 2. invariants et reproductibilité, graines 1 à 10
    for (int graine = 1; graine <= 10; graine++)
    {
        f = createForest(24, 3, 160, 0.25, 20000);
        plantForest(&f, graine, 0);
        simulateForest(&f, 32);
        problemes += checkForest(&f);
        g = createForest(24, 3, 160, 0.25, 20000);
        plantForest(&g, graine, 0);
        simulateForest(&g, 32);
        for (int k = 0; k < f.K; k++)
        {
            if (!sameTrees(f.arbres[k].step.arbre.pied, g.arbres[k].step.arbre.pied))
            {
                differences++;
            }
        }
        freeForest(&f);
        freeForest(&g);
    }
    printf("10 forêts : %d problème(s) de mesures ou d'occupation, %d arbre(s) différent(s) d'une simulation à l'autre (attendu : 0 et 0)\n",
           problemes, differences);

    // 3. la forêt de validation, puis sa sauvegarde
    f = createForest(24, 3, 160, 0.25, 20000);
    plantForest(&f, 1, 0);
    simulateForest(&f, 32);
    for (int k = 0; k < f.K; k++)
    {
        total += f.arbres[k].step.arbre.nb_rameaux;
    }
    printf("K = 24, E = 3, A = 0,25, graine 1, 32 saisons : %ld rameaux\n", total);
    printf("  arbre 0 : %d rameaux, hauteur %d, cime %.3f, extension %.3f, %d bourgeons\n", f.arbres[0].step.arbre.nb_rameaux,
           f.arbres[0].hauteur, f.arbres[0].cime, f.arbres[0].xmax - f.arbres[0].xmin, f.arbres[0].nb_bourgeons);
    printf("  arbre 7 : %d rameaux, hauteur %d, extension %.3f\n", f.arbres[7].step.arbre.nb_rameaux, f.arbres[7].hauteur,
           f.arbres[7].xmax - f.arbres[7].xmin);
    printf("attendu : 6809 ; arbre 0 : 352, 32, 33,009, 17,479, 87 ; arbre 7 : 17, 8, 3,941\n");
    saveForest(&f, "output/foret.txt");
    printf("output/foret.txt %s data/foret_exemple.txt (attendu : identique à)\n",
           sameFiles("output/foret.txt", "data/foret_exemple.txt") ? "identique à" : "DIFFÉRENT de");
    freeForest(&f);

    return 0;
}
