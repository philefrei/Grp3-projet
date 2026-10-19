//
// Created by flasque on 14/10/2026.
//

#include "sauver_arbre.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// étape 4 : sauvegarde de l'arbre a dans le fichier texte nom_fichier
// la sauvegarde doit être effectuée par un PARCOURS EN LARGEUR de l'arbre (file de rameaux, voir queue.h) :
// c'est l'ordre que readTree attend pour relire le fichier
void saveTree(t_arbre a, char *nom_fichier)
{
    // ouverture du fichier en écriture, en mode texte (fourni, à ne pas modifier)
    FILE *f = fopen(nom_fichier, "wt");
    if (f == NULL)
    {
        perror("Impossible de creer le fichier");
        return;
    }

    // À ÉCRIRE : le parcours en largeur de l'arbre, qui écrit une ligne par rameau
    // première ligne, l'extrémité (x ; y) du pied :
    //     fprintf(f, "%.2f %.2f\n", ..., ...);
    // puis une ligne par rameau autre que le pied :
    //     fprintf(f, "%d %d %d %.2f %.2f %d %d\n", num, saison, numéro du père, dir, longueur, gen, etat);
    // un rameau ne connaît pas son père : écrivez la ligne de chaque fils au moment où vous traitez son père
    // saveTree ne modifie pas l'arbre et ne le libère pas : il sert encore après la sauvegarde
    // à la fin du parcours, la file est vide et dequeue a déjà libéré ses cellules : rien d'autre à libérer

    // fermeture du fichier (fourni, à ne pas modifier)
    fclose(f);
}
