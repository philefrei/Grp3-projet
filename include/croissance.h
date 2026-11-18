//
// Created by flasque on 12/10/2026.
//

#ifndef FICHIERS_FOURNIS_CROISSANCE_H
#define FICHIERS_FOURNIS_CROISSANCE_H

#include "alea.h"
#include "arbre.h"

/***
 * @brief tire au hasard si un bourgeon pousse ou non
 * @param float : probabilité de pousse, en % (ex : 95 en pleine lumière)
 * @return 1 si le bourgeon pousse, 0 sinon
 */
int canGrow(float);

/***
 * @brief tire au hasard le nombre de rameaux que donne un bourgeon qui pousse, avec un seul tirage
 * @param p1 : probabilité, en %, de donner 1 rameau
 * @param p2 : probabilité, en %, de donner 2 rameaux (3 rameaux sinon)
 * @return 1, 2 ou 3
 */
int howManyBranches(float p1, float p2);

/***
 * @brief fait naître n rameaux à l'extrémité du bourgeon b : le premier prolonge b (±10°),
 *        les suivants partent à gauche ou à droite (30° à 60°) ; longueurs tirées entre 0,7 et 1,3
 * @param a : l'arbre (son nombre de rameaux est mis à jour)
 * @param b : le bourgeon qui pousse ; il devient du bois
 * @param n : nombre de rameaux à créer (1 à 3)
 * @param saison : saison de naissance des nouveaux rameaux
 */
void growBud(t_arbre *a, t_rameau *b, int n, int saison);

/***
 * @brief fait pousser l'arbre en pleine lumière pendant nbSaisons saisons, avec deux files :
 *        les bourgeons de la saison en cours, et ceux de la saison suivante
 * @param a : l'arbre, créé par createTree
 * @param nbSaisons : nombre de saisons
 */
void growTree(t_arbre *a, int nbSaisons);

#endif //FICHIERS_FOURNIS_CROISSANCE_H