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


/***
* @brief découpe [minimum ; maximum] en classes de même largeur et compte les valeurs de chaque classe ;
*        le maximum va dans la dernière classe ; si toutes les valeurs sont égales, tout va dans la première
* @param v : valeurs
* @param n : nombre de valeurs (au moins 1)
* @param nb_classes : nombre de classes (au moins 1)
* @param p_min : reçoit le minimum des valeurs
* @param p_largeur : reçoit la largeur d'une classe
* @return un tableau dynamique des nb_classes effectifs, à libérer avec free
*/
int *computeHistogram(double v[], int n, int nb_classes, double *p_min, double *p_largeur);

/***
* @brief affiche une ligne par classe : ses bornes, son effectif et une barre de # ;
*        la classe la plus remplie a 50 #, les autres une barre proportionnelle arrondie
* @param effectifs : effectif de chaque classe
* @param nb_classes : nombre de classes
* @param min : borne gauche de la première classe
* @param largeur : largeur d'une classe
*/
void displayHistogram(int effectifs[], int nb_classes, double min, double largeur);
/*** * @brief compte récursivement les ra... par Philéas ELOI
Philéas ELOI
16:17
/***
* @brief compte récursivement les rameaux du sous-arbre de r
* @param r : rameau de départ (le pied pour tout l'arbre)
* @return le nombre de rameaux, r compris
*/
int countBranches(t_rameau *r);

/***
* @brief calcule récursivement la hauteur : la plus grande génération des rameaux du sous-arbre
* @param r : rameau de départ
* @return la génération maximale
*/
int computeHeight(t_rameau *r);

/***
* @brief calcule récursivement la cime : la plus grande altitude des extrémités du sous-arbre
* @param r : rameau de départ
* @return l'altitude maximale
*/
float computeTop(t_rameau *r);

/***
* @brief calcule récursivement la plus petite position x des extrémités du sous-arbre
* @param r : rameau de départ
* @return la position x minimale
*/
float computeXMin(t_rameau *r);

/***
* @brief calcule récursivement la plus grande position x des extrémités du sous-arbre
* @param r : rameau de départ
* @return la position x maximale
*/
float computeXMax(t_rameau *r);

/***
* @brief compte récursivement les bourgeons (rameaux sans fils) du sous-arbre
* @param r : rameau de départ
* @return le nombre de bourgeons
*/
int countBuds(t_rameau *r);

/***
* @brief libère récursivement tous les rameaux du sous-arbre : d'abord les fils, puis le tableau des fils, puis r
* @param r : rameau de départ
*/
void freeBranches(t_rameau *r);

/***
* @brief crée un arbre de la forêt réduit à son pied, avec ses mesures initiales
* @param id : numéro de l'arbre dans la forêt
* @param x : position horizontale du pied
* @param y : altitude du pied
* @param seed : graine du générateur aléatoire
* @return l'arbre de la forêt
*/
t_arbre_foret createForestTree(int id, float x, float y, int seed);

/***
* @brief libère la file des bourgeons puis tous les rameaux de l'arbre
* @param a : l'arbre de la forêt
*/
void freeForestTree(t_arbre_foret *a);

/***
* @brief fait pousser l'arbre d'une saison, tous ses bourgeons recevant la même lumière
* @param a : l'arbre de la forêt
* @param lumiere : lumière, entre 0 et 1
*/
void growSeason(t_arbre_foret *a, double lumiere);

#endif //FORET_ARBRE_FORET_H
