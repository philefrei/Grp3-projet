//
// Created by flasque on 12/10/2026.
//

#ifndef FICHIERS_FOURNIS_CROISSANCE_H
#define FICHIERS_FOURNIS_CROISSANCE_H

#include "alea.h"
#include "arbre.h"

/***
 * @brief tire au hasard si un bourgeon pousse ou non
 * @param float : probabilité de pousse, en % (ex : 95 en pleine lumière)
 * @return 1 si le bourgeon pousse, 0 sinon
 */
int canGrow(float);

int howManyBranches(float p1, float p2);

void growBud(t_arbre *a, t_rameau *b, int n, int saison);

void growTree(t_arbre *a, int nbSaisons);


#endif //FICHIERS_FOURNIS_CROISSANCE_H
