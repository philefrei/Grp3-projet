/**
 * @file arbres_utils.h
 * @brief Lecture d'un arbre dans un fichier texte et affichage console (module fourni).
 */
#ifndef ARBRES_UTILS_H
#define ARBRES_UTILS_H

#include "arbre.h"

/**
 * @brief Construit l'arbre décrit dans un fichier texte, écrit en parcours en largeur (BFS).
 *
 * Format du fichier (valeurs séparées par des espaces) :
 * - première ligne : `x y`, l'extrémité du pied (le pied a le numéro 0) ;
 * - chaque ligne suivante décrit un rameau autre que le pied :
 *   `numero saison parent direction longueur generation etat`
 *   (etat : 0 pour BOURGEON, 1 pour BOIS).
 *
 * Les rameaux doivent être écrits dans l'ordre d'un parcours en largeur depuis le pied :
 * le parent d'un rameau est donc toujours décrit avant lui, et les fils d'un même rameau
 * sont sur des lignes consécutives, dans l'ordre de son tableau p_fils.
 * Les numéros des rameaux sont conservés ; ils doivent aller de 0 à nb_rameaux - 1.
 * Les extrémités (x ; y) sont recalculées à partir de la direction et de la longueur.
 *
 * Le tableau p_fils de chaque rameau est alloué à la taille exacte de son nombre de fils.
 *
 * Le programme s'arrête si le fichier ne peut pas être ouvert, si la première ligne est
 * invalide, si le parent d'un rameau n'est pas trouvé ou si les fils d'un rameau ne sont pas
 * sur des lignes consécutives (fichier qui n'est pas en BFS), ou si un rameau a plus de
 * NB_FILS_MAX fils.
 *
 * @param nom_fichier nom du fichier à lire
 * @return l'arbre construit
 */
t_arbre readTree(const char *nom_fichier);

/**
 * @brief Affiche tous les rameaux d'un arbre, par numéro.
 *
 * Pour chaque rameau : numéro, saison, génération, état, extrémité et numéros des fils.
 *
 * @param a arbre à afficher
 */
void displayTree(t_arbre a);

#endif
