/**
 * @file dessin.h
 * @brief Dessin d'un arbre de profil, au format SVG, fixe ou animé (module fourni).
 *
 * L'échelle est choisie automatiquement pour que l'arbre tienne dans l'image.
 * Les fichiers produits s'ouvrent avec un navigateur web.
 */
#ifndef DESSIN_H
#define DESSIN_H

#include "arbre.h"

/**
 * @brief Écrit le dessin de profil d'un arbre dans un fichier SVG.
 *
 * @param pied        pied de l'arbre à dessiner
 * @param nom_fichier nom du fichier à écrire (par exemple "arbre.svg")
 */
void drawSvg(t_rameau *pied, const char *nom_fichier);

/**
 * @brief Écrit une animation de la pousse de l'arbre, saison par saison, dans un fichier SVG.
 *
 * Ouvrir le fichier avec un navigateur web, et recharger la page (F5) pour revoir l'animation.
 *
 * @param pied         pied de l'arbre à animer
 * @param nom_fichier  nom du fichier à écrire (par exemple "pousse.svg")
 * @param duree_saison durée d'une saison dans l'animation, en secondes (par exemple 0.5)
 */
void animateSvg(t_rameau *pied, const char *nom_fichier, double duree_saison);

/**
 * @brief Écrit une page HTML interactive qui montre l'arbre de trois façons.
 *
 * La page contient l'arbre informatique (racine en haut), l'arbre informatique (racine en bas)
 * et l'arbre de rameaux, de profil. Survoler un nœud ou un rameau le met en évidence dans les trois
 * vues et affiche ses informations (parent, fils, saison, génération, direction, longueur, extrémité).
 * Ouvrir le fichier avec un navigateur web.
 *
 * Seuls les rameaux des 15 premières saisons sont dessinés : au-delà, les dessins sont illisibles.
 *
 * @param pied        pied de l'arbre à dessiner
 * @param nom_fichier nom du fichier à écrire (par exemple "arbre.html")
 */
void drawHtml(t_rameau *pied, const char *nom_fichier);

#endif
