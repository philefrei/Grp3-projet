/**
 * @file sauver_foret.h
 * @brief Sauvegarde d'une forêt dans un fichier texte (module fourni, partie 2).
 *
 * Format du fichier :
 *   FORET K E C HMAX A P_max saison
 * puis, pour chaque arbre, de l'arbre 0 à l'arbre K - 1 :
 *   ARBRE id tronque nb_rameaux
 * suivi de l'arbre au format de saveTree (partie 1) : l'extrémité du pied « x y », puis une ligne
 * « num saison parent dir longueur gen etat » par rameau, dans l'ordre d'un parcours en largeur.
 * Les positions x ne sont pas ramenées entre 0 et C : la forêt fait le tour au moment du dessin.
 * Le fichier s'ouvre avec le visualiseur du site du projet, qui en tire une image PNG et un GIF animé.
 */
#ifndef SAUVER_FORET_H
#define SAUVER_FORET_H

#include "foret.h"

/***
 * @brief Écrit la forêt f dans le fichier texte nom_fichier.
 * @param f la forêt à sauvegarder
 * @param nom_fichier nom du fichier à écrire (par exemple "output/foret.txt")
 */
void saveForest(t_foret *f, const char *nom_fichier);

#endif
