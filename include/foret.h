/* foret.h : partie 2, étapes 4 à 6 — la forêt, ses grilles d'occupation et de lumière, sa pousse (code fourni, à compléter) */

#ifndef FORET_FORET_H
#define FORET_FORET_H

/* ---------------------------------------------------------------- à compléter : la structure t_foret, ici, avant le code fourni */
#include "arbre_foret.h"

/***
 * @brief la forêt : K arbres sur une ligne qui fait le tour, et deux grilles HMAX x C
 */
typedef struct s_foret
{
    int K, E, C, HMAX;      // nb d'arbres, écartement, nb de colonnes (K x E), nb de rangées
    double A;               // part de lumière retenue par chaque extrémité de rameau
    int P_max;              // nb de rameaux à partir duquel un arbre est tronqué
    t_arbre_foret *arbres;  // tableau dynamique des K arbres
    int **occupation;       // occupation[r][c] : nb d'extrémités dans la case
    double **lumiere;       // lumiere[r][c] : lumière reçue dans la case
    int saison;             // nb de saisons jouées
} t_foret;
/* ---------------------------------------------------------------- code fourni (ne compile qu'une fois t_foret écrite) */

/***
 * @brief colonne de la case qui contient l'abscisse x (la forêt fait le tour : modulo C)
 * @param f : la forêt
 * @param x : position, éventuellement négative ou au moins égale à C
 * @return la colonne, entre 0 et C - 1
 */
int columnOf(t_foret *f, float x);

/***
 * @brief rangée de la case qui contient l'altitude y (au moins HMAX : dernière rangée)
 * @param f : la forêt
 * @param y : altitude
 * @return la rangée, entre 0 et HMAX - 1
 */
int rowOf(t_foret *f, float y);

/* ---------------------------------------------------------------- à compléter : vos prototypes, à partir d'ici */

/***
 * @brief crée une forêt vide : grilles d'occupation à 0 et de lumière à 1
 * @param K : nombre d'arbres
 * @param E : écartement entre deux arbres
 * @param HMAX : nombre de rangées des grilles
 * @param A : part de lumière retenue par une extrémité
 * @param P_max : nombre de rameaux à partir duquel un arbre est tronqué
 * @return la forêt
 */
t_foret createForest(int K, int E, int HMAX, double A, int P_max);

/***
 * @brief ajoute l'extrémité du rameau r dans la case de la grille d'occupation qui la contient
 * @param f : la forêt
 * @param r : le rameau
 */
void addToGrid(t_foret *f, t_rameau *r);

/***
 * @brief ajoute récursivement à la grille les extrémités de tous les rameaux du sous-arbre de r
 * @param f : la forêt
 * @param r : rameau de départ
 */
void addTreeToGrid(t_foret *f, t_rameau *r);

/***
 * @brief calcule la lumière de chaque case, colonne par colonne, de haut en bas :
 *        chaque extrémité d'une case multiplie par (1 - A) la lumière des cases du dessous
 * @param f : la forêt
 */
void computeLight(t_foret *f);

/***
 * @brief lumière reçue par l'extrémité du rameau r
 * @param f : la forêt
 * @param r : le rameau
 * @return la lumière de sa case, entre 0 et 1
 */
double getLight(t_foret *f, t_rameau *r);

/***
 * @brief plante les K arbres en x = i E + E/2 ; altitude du pied 1, ou tirée entre 0,5 et 1,5
 * @param f : la forêt
 * @param seed : graine de la forêt (randomInit une seule fois)
 * @param identique : 1 pour des pieds tous à l'altitude 1, 0 pour des altitudes au hasard
 */
void plantForest(t_foret *f, int seed, int identique);

/***
 * @brief fait pousser un arbre d'une saison, chaque bourgeon avec la lumière de sa case ;
 *        les nouveaux rameaux sont ajoutés à la grille ; l'arbre est tronqué à P_max rameaux
 * @param f : la forêt
 * @param a : l'arbre
 */
void growSeasonInForest(t_foret *f, t_arbre_foret *a);

/***
 * @brief une saison de la forêt : calcul de la lumière, puis pousse des arbres 0 à K - 1
 * @param f : la forêt
 */
void growForestSeason(t_foret *f);

/***
 * @brief fait pousser la forêt pendant S saisons
 * @param f : la forêt
 * @param S : nombre de saisons
 */
void simulateForest(t_foret *f, int S);

/***
 * @brief libère tous les arbres, le tableau des arbres et les deux grilles
 * @param f : la forêt
 */
void freeForest(t_foret *f);

#endif //FORET_FORET_H