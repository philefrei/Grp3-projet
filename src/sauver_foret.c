/* sauver_foret.c : module fourni (partie 2) — sauvegarde d'une forêt dans un fichier texte */
#include <stdio.h>
#include "queue.h"
#include "sauver_foret.h"

/***
 * @brief Écrit un arbre au format de saveTree (partie 1) : le pied, puis ses rameaux par un parcours en largeur.
 * @param f fichier ouvert en écriture
 * @param pied pied de l'arbre
 */
static void writeTree(FILE *f, t_rameau *pied)
{
    t_queue q = createEmptyQueue();
    t_rameau *r;
    t_rameau *fils;

    fprintf(f, "%.2f %.2f\n", pied->x, pied->y);
    enqueue(&q, pied);
    while (!isEmptyQueue(q))
    {
        r = dequeue(&q);
        // un rameau ne connaît pas son père : on écrit chaque fils au moment où on traite son père r
        for (int i = 0; i < r->nb_fils; i++)
        {
            fils = r->p_fils[i];
            fprintf(f, "%d %d %d %.2f %.2f %d %d\n",
                    fils->num, fils->saison, r->num, fils->dir, fils->longueur, fils->gen, fils->etat);
            enqueue(&q, fils);
        }
    }

    return;
}

void saveForest(t_foret *f, const char *nom_fichier)
{
    FILE *fic = fopen(nom_fichier, "wt");
    t_arbre_foret *a;

    if (fic == NULL)
    {
        perror("Impossible de creer le fichier");
    }
    else
    {
        fprintf(fic, "FORET %d %d %d %d %.3f %d %d\n", f->K, f->E, f->C, f->HMAX, f->A, f->P_max, f->saison);
        for (int k = 0; k < f->K; k++)
        {
            a = &f->arbres[k];
            fprintf(fic, "ARBRE %d %d %d\n", a->id, a->tronque, a->step.arbre.nb_rameaux);
            writeTree(fic, a->step.arbre.pied);
        }
        fclose(fic);
    }

    return;
}
