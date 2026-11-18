//
// Created by flasque on 14/10/2026.
//

#ifndef FICHIERS_FOURNIS_CROISSANCE_STEP_H
#define FICHIERS_FOURNIS_CROISSANCE_STEP_H

#include "arbre.h"
#include "queue.h"

/***
 * @brief arbre qui pousse une saison à la fois : on garde sa file de bourgeons entre deux saisons
 */
typedef struct s_arbre_step
{
    t_arbre arbre;        // l'arbre
    t_queue bourgeons;    // les bourgeons qui pousseront à la prochaine saison
    int saison;           // numéro de la dernière saison jouée (0 au départ)
} t_arbre_step;

/***
 * @brief crée un arbre réduit à son pied, seul bourgeon de la file, à la saison 0
 * @param x : position horizontale du pied
 * @param y : altitude du pied
 * @param seed : graine du générateur aléatoire
 * @return l'arbre pas à pas
 */
t_arbre_step createTreeStep(float x, float y, int seed);

/***
 * @brief fait pousser l'arbre d'une seule saison ; n appels donnent le même arbre que growTree(n)
 * @param a : l'arbre pas à pas (file de bourgeons et saison mises à jour)
 */
void growTreeStep(t_arbre_step *a);

/***
 * @brief vide la file des bourgeons pour libérer ses cellules (les rameaux restent dans l'arbre)
 * @param a : l'arbre pas à pas
 */
void clearBuds(t_arbre_step *a);

#endif //FICHIERS_FOURNIS_CROISSANCE_STEP_H
