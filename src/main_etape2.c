//
// Created by flasque on 15/10/2026.
//
/* main_etape2.c : étape 2, la pousse d'un arbre en pleine lumière
   tester les fonctions de croissance, une par une, dans l'ordre du sujet */

#include <stdio.h>
#include "arbre.h"
#include "arbres_utils.h"
#include "dessin.h"
#include "alea.h"
#include "queue.h"
#include "croissance.h"

int main(void)
{
    int compte[4] = {0, 0, 0, 0};
    t_arbre a, b, c;
    t_queue q;

    // 1. howManyBranches : 1 000 000 de tirages (graine par défaut du générateur)
    for (int i = 0; i < 1000000; i++)
    {
        compte[howManyBranches(70, 25)]++;
    }
    printf("howManyBranches(70, 25) : 1 -> %d fois, 2 -> %d fois, 3 -> %d fois\n", compte[1], compte[2], compte[3]);
    printf("attendu, a 2000 pres   : 1 -> 700000 fois, 2 -> 250000 fois, 3 -> 50000 fois\n\n");

    // 2. growBud : le pied d'un arbre donne 3 rameaux à la saison 1
    a = createTree(0, 1, 1);
    growBud(&a, a.pied, 3, 1);
    displayTree(a);
    printf("\n");

    // 3. la file : on enfile les 3 fils du pied, puis on les défile
    q = createEmptyQueue();
    for (int i = 0; i < a.pied->nb_fils; i++)
    {
        enqueue(&q, a.pied->p_fils[i]);
    }
    displayQueue(q);
    if (!isEmptyQueue(q))
    {
        printf("defile : rameau %d\n", dequeue(&q)->num);
        displayQueue(q);
    }
    while (!isEmptyQueue(q))
    {
        dequeue(&q);
    }
    displayQueue(q);
    printf("\n");

    // 4. growTree : avec la graine 5 et 3 saisons, on doit retrouver arbre_petit.txt
    b = createTree(0, 1, 5);
    growTree(&b, 3);
    printf("growTree, graine 5, 3 saisons :\n");
    displayTree(b);
    printf("arbre_petit.txt :\n");
    displayTree(readTree("data/arbre_petit.txt"));
    printf("\n");

    // 5. dessin et animation : avec la graine 3 et 8 saisons, à comparer avec data/animation_8_saisons.svg
    c = createTree(0, 1, 3);
    growTree(&c, 8);
    printf("growTree, graine 3, 8 saisons : %d rameaux (arbre_moyen.txt : 38 rameaux)\n", c.nb_rameaux);
    drawSvg(c.pied, "output/etape2_moyen.svg");
    animateSvg(c.pied, "output/etape2_moyen_animation.svg", 0.6);

    return 0;
}
