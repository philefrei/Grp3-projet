//
// Created by flasque on 14/10/2026.
//

#ifndef FICHIERS_FOURNIS_CROISSANCE_STEP_H
#define FICHIERS_FOURNIS_CROISSANCE_STEP_H

#include "arbre.h"
#include "queue.h"

typedef struct s_arbre_step
{
    t_arbre arbre;
    t_queue bourgeons;
    int saison;
} t_arbre_step;

t_arbre_step createTreeStep(float x, float y, int seed);
void growTreeStep(t_arbre_step *a);
void clearBuds(t_arbre_step *a);

#endif //FICHIERS_FOURNIS_CROISSANCE_STEP_H