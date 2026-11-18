//
// Created by flasque on 14/10/2026.
//

#ifndef FORET_SAUVER_ARBRE_H
#define FORET_SAUVER_ARBRE_H

#include "arbre.h"

/***
 * @brief écrit l'arbre dans un fichier texte par un parcours en largeur : la position du pied,
 *        puis une ligne par rameau (numéro, saison, numéro du parent, direction, longueur, génération, état) ;
 *        le fichier se relit avec readTree
 * @param a : l'arbre à sauvegarder
 * @param nom_fichier : nom du fichier à créer
 */
void saveTree(t_arbre a, char *nom_fichier);

#endif //FORET_SAUVER_ARBRE_H
