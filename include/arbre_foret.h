/* arbre_foret.h : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière (fichier fourni vide, à compléter) */

#ifndef FORET_ARBRE_FORET_H
#define FORET_ARBRE_FORET_H

#include "croissance_step.h"

typedef struct s_arbre_foret
{
    t_arbre_step step;    // l'arbre, sa file de bourgeons et sa saison (partie 1)
    int id;               // numero de l'arbre dans la foret
    int hauteur;          // generation maximale de ses rameaux
    float cime;           // altitude maximale des extremites
    float xmin, xmax;     // positions x minimale et maximale des extremites
    int nb_bourgeons;     // nombre de bourgeons
    int tronque;          // 1 si l'arbre a atteint P_max rameaux
} t_arbre_foret;


int countBranches(t_rameau *r);
int computeHeight(t_rameau *r);
float computeTop(t_rameau *r);
float computeXMin(t_rameau *r);
float computeXMax(t_rameau *r);
int countBuds(t_rameau *r);
void freeBranches(t_rameau *r);

t_arbre_foret createForestTree(int id, float x, float y, int seed);
void freeForestTree(t_arbre_foret *a);

/***
 * @brief comme growBud, mais la longueur des rameaux dépend de la lumière : (0,5 + 0,5 L) × u ;
 *        met à jour les mesures de l'arbre (hauteur, cime, xmin, xmax, nombre de bourgeons)
 * @param a : l'arbre de la forêt
 * @param b : le bourgeon qui pousse
 * @param n : nombre de rameaux à créer (1 à 3)
 * @param lumiere : lumière reçue par le bourgeon, entre 0 et 1
 */
void growBudInLight(t_arbre_foret *a, t_rameau *b, int n, double lumiere);
void growSeason(t_arbre_foret *a, double lumiere);

#endif //FORET_ARBRE_FORET_H
