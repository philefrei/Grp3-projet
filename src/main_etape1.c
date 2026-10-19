//
// Created by flasque on 14/10/2026.
//
/* main_etape1.c : étape 1, prise en main de l'environnement
   lire un arbre, l'afficher, le dessiner et animer sa pousse */

#include <stdio.h>
#include "arbre.h"
#include "arbres_utils.h"
#include "dessin.h"

int main(void)
{
    t_arbre a;

    // lecture de l'arbre décrit dans un fichier texte
    a = readTree("data/exemple1.txt");

    // affichage console de tous ses rameaux
    displayTree(a);

    // dessin de profil, à ouvrir dans un navigateur web
    drawSvg(a.pied, "output/exemple1.svg");

    // animation de la pousse, saison par saison (0.5 s par saison)
    animateSvg(a.pied, "output/exemple1_animation.svg", 0.5);

    // trois vues interactives : arbres informatiques et arbre de rameaux
    drawHtml(a.pied, "output/exemple1.html");

    return 0;
}
