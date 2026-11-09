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
        printf("Impossible de creer le fichier\n");
        return;
    }
    fprintf(f, "%.2f %.2f\n", a.pied->x, a.pied->y);
    t_queue q;
    t_rameau *r;

    q = createEmptyQueue();
    enqueue(&q, a.pied);
    while (isEmptyQueue(q)==0)
    {
        r = dequeue(&q);

        for (int i = 0; i < r->nb_fils; i++)
        {
            t_rameau *fils = r->p_fils[i];
            fprintf(f, "%d %d %d %.2f %.2f %d %d\n",
                    fils->num, fils->saison, r->num,
                    fils->dir, fils->longueur, fils->gen, fils->etat);
            enqueue(&q, fils);
        }
    }
    fclose(f);
}

