/* main_soutenance.c : programme exécuté et édité en direct pendant la soutenance (cible CMake soutenance)
   chaque démo est une fonction ; dans main(), on change les valeurs données par l'enseignant,
   ou on met en commentaire (//) les lignes qu'on ne veut pas lancer */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "arbres_utils.h"
#include "dessin.h"
#include "croissance.h"
#include "croissance_step.h"
#include "sauver_arbre.h"
#include "arbre_foret.h"
#include "foret.h"
#include "sauver_foret.h"
#include "campagne.h"
#include "statistiques.h"

/***
 * @brief affiche les mesures récursives d'un arbre (partie 2, étape 1)
 * @param pied : le pied de l'arbre
 */
static void afficherMesures(t_rameau *pied)
{
    printf("  rameaux %d, hauteur %d, cime %.3f, xmin %.3f, xmax %.3f, extension %.3f, bourgeons %d\n",
           countBranches(pied), computeHeight(pied), computeTop(pied), computeXMin(pied), computeXMax(pied),
           computeXMax(pied) - computeXMin(pied), countBuds(pied));
}

/***
 * @brief partie 1 : fait pousser un arbre avec growTree et avec growTreeStep, le sauve, le relit et le dessine
 * @param graine : graine du générateur aléatoire
 * @param saisons : nombre de saisons
 */
static void demoArbre(int graine, int saisons)
{
    t_arbre a;
    t_arbre relu;
    t_arbre_step s;
    int i;

    printf("=== Partie 1 : arbre de graine %d, %d saisons\n", graine, saisons);

    a = createTree(0, 1, graine);
    growTree(&a, saisons);

    s = createTreeStep(0, 1, graine);
    for (i = 0; i < saisons; i++)
    {
        growTreeStep(&s);
    }
    clearBuds(&s);

    printf("  growTree : %d rameaux ; growTreeStep : %d rameaux\n", a.nb_rameaux, s.arbre.nb_rameaux);

    saveTree(a, "output/soutenance_arbre.txt");
    relu = readTree("output/soutenance_arbre.txt");
    printf("  sauvegarde : output/soutenance_arbre.txt ; relu par readTree : %d rameaux\n", relu.nb_rameaux);

    drawSvg(a.pied, "output/soutenance_arbre.svg");
    animateSvg(a.pied, "output/soutenance_arbre_animation.svg", 0.6);
    printf("  dessins : output/soutenance_arbre.svg et output/soutenance_arbre_animation.svg\n\n");

    freeBranches(a.pied);
    freeBranches(s.arbre.pied);
    freeBranches(relu.pied);
}

/***
 * @brief partie 2, étape 1 : lit un arbre dans un fichier et affiche ses mesures récursives
 * @param nom_fichier : fichier écrit au format de saveTree (par exemple data/arbre_moyen.txt)
 */
static void demoMesures(const char *nom_fichier)
{
    t_arbre a = readTree(nom_fichier);

    printf("=== Partie 2 : mesures récursives de %s\n", nom_fichier);
    afficherMesures(a.pied);
    printf("\n");
    freeBranches(a.pied);
}

/***
 * @brief partie 2, étape 3 : fait pousser un arbre seul avec une lumière fixe ;
 *        les mesures tenues à jour doivent être égales aux mesures récursives
 * @param graine : graine du générateur aléatoire
 * @param saisons : nombre de saisons
 * @param lumiere : lumière, entre 0 et 1
 */
static void demoLumiere(int graine, int saisons, double lumiere)
{
    t_arbre_foret a;
    int i;

    printf("=== Partie 2 : arbre de graine %d, %d saisons, lumière %.2f\n", graine, saisons, lumiere);

    a = createForestTree(0, 0, 1, graine);
    for (i = 0; i < saisons; i++)
    {
        growSeason(&a, lumiere);
    }

    printf("  tenues à jour : rameaux %d, hauteur %d, cime %.3f, extension %.3f, bourgeons %d\n",
           a.step.arbre.nb_rameaux, a.hauteur, a.cime, a.xmax - a.xmin, a.nb_bourgeons);
    printf("  récursives    :");
    afficherMesures(a.step.arbre.pied);
    printf("\n");

    freeForestTree(&a);
}

/***
 * @brief partie 2, étapes 4 à 6 : plante et simule une forêt, puis la sauve pour le Visualiseur
 * @param K : nombre d'arbres
 * @param E : écartement
 * @param A : part de lumière retenue par une extrémité
 * @param S : nombre de saisons
 * @param graine : graine de la forêt
 * @param identique : 1 pour des pieds tous à l'altitude 1, 0 pour des altitudes au hasard
 */
static void demoForet(int K, int E, double A, int S, int graine, int identique)
{
    t_foret f;
    long total = 0;
    long occupation = 0;
    int k, r, c;

    printf("=== Partie 2 : forêt K = %d, E = %d, A = %.2f, %d saisons, graine %d, identique %d\n",
           K, E, A, S, graine, identique);

    f = createForest(K, E, H_MAX, A, P_MAX);
    plantForest(&f, graine, identique);
    simulateForest(&f, S);

    for (k = 0; k < f.K; k++)
    {
        total += f.arbres[k].step.arbre.nb_rameaux;
    }
    for (r = 0; r < f.HMAX; r++)
    {
        for (c = 0; c < f.C; c++)
        {
            occupation += f.occupation[r][c];
        }
    }
    printf("  %ld rameaux ; somme des compteurs d'occupation : %ld (doit être égale)\n", total, occupation);
    printf("  arbre 0 : %d rameaux, hauteur %d, cime %.3f, extension %.3f, %d bourgeons\n",
           f.arbres[0].step.arbre.nb_rameaux, f.arbres[0].hauteur, f.arbres[0].cime,
           f.arbres[0].xmax - f.arbres[0].xmin, f.arbres[0].nb_bourgeons);

    saveForest(&f, "output/soutenance_foret.txt");
    printf("  sauvegarde : output/soutenance_foret.txt (à ouvrir dans le Visualiseur)\n\n");

    freeForest(&f);
}

/***
 * @brief partie 3, étape 1 : lance une campagne de R forêts et écrit les deux fichiers CSV
 * @param K, E, A, S : réglages de chaque forêt
 * @param R : nombre de forêts
 * @param graine : la forêt r est plantée avec la graine graine + r
 * @param identique : 1 pour une plantation identique
 * @param sortie : début du nom des deux fichiers (par exemple "output/soutenance")
 */
static void demoCampagne(int K, int E, double A, int S, int R, int graine, int identique, const char *sortie)
{
    t_campagne c = createCampaign();

    printf("=== Partie 3 : campagne\n");
    c.K = K;
    c.E = E;
    c.A = A;
    c.S = S;
    c.R = R;
    c.graine = graine;
    c.identique = identique;
    strncpy(c.sortie, sortie, LONGUEUR_NOM - 1);
    c.sortie[LONGUEUR_NOM - 1] = '\0';

    if (!runCampaign(c))
    {
        printf("  la campagne n'a pas pu écrire ses fichiers\n");
    }
    printf("\n");
}

/***
 * @brief partie 3, étapes 2 et 3 : résume une colonne d'un fichier CSV et dessine son histogramme
 * @param nom_fichier : fichier CSV (par exemple "output/soutenance_arbres.csv")
 * @param colonne : nom de la colonne (cime, extension, rameaux, hauteur, cime_moy, extension_moy…)
 * @param nb_classes : nombre de classes
 * @param nom_svg : fichier SVG à écrire
 */
static void demoAnalyse(const char *nom_fichier, const char *colonne, int nb_classes, const char *nom_svg)
{
    double *v;
    int n;
    int *effectifs;
    double min, largeur;

    printf("=== Partie 3 : analyse de %s, colonne %s, %d classes\n", nom_fichier, colonne, nb_classes);
    v = readColumn(nom_fichier, colonne, &n);
    if (v == NULL || n == 0 || nb_classes <= 0)
    {
        printf("  analyse impossible (fichier, colonne ou nombre de classes)\n\n");
        free(v);
        return;
    }

    displaySummary(v, n);
    effectifs = computeHistogram(v, n, nb_classes, &min, &largeur);
    displayHistogram(effectifs, nb_classes, min, largeur);
    histoToSVG(effectifs, nb_classes, min, largeur, colonne, nom_svg);
    printf("  dessin : %s\n\n", nom_svg);

    free(effectifs);
    free(v);
}

int main(void)
{
    /* ---------- À MODIFIER EN DIRECT : les valeurs données par l'enseignant ---------- */

    /* partie 1 : graine, nombre de saisons */
    demoArbre(3, 8);

    /* partie 2 : un arbre de data/ (exemple1, arbre_petit, arbre_moyen, arbre_grand) ou un arbre sauvé */
    demoMesures("data/arbre_moyen.txt");

    /* partie 2 : graine, nombre de saisons, lumière entre 0 et 1 */
    demoLumiere(1, 20, 0.5);

    /* partie 2 : K, E, A, saisons, graine, identique (0 ou 1) */
    demoForet(24, 3, 0.25, 32, 1, 0);

    /* partie 3 : K, E, A, saisons, R, graine, identique, début du nom des fichiers */
    demoCampagne(24, 3, 0.25, 32, 3, 1, 0, "output/soutenance");

    /* partie 3 : fichier CSV, colonne, nombre de classes, fichier SVG */
    demoAnalyse("output/soutenance_arbres.csv", "cime", 10, "output/soutenance_cime.svg");

    return 0;
}