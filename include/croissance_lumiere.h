/* croissance_lumiere.h : partie 2, étape 2 — le nombre de nouveaux rameaux selon la lumière (code fourni, à compléter) */

#ifndef FORET_CROISSANCE_LUMIERE_H
#define FORET_CROISSANCE_LUMIERE_H

/* ---------------------------------------------------------------- code fourni */

/***
 * @brief tire au hasard le numéro d'une tranche dans une table de pourcentages
 * @param pourcentages : pourcentages des tranches (en %, leur somme vaut 100)
 * @param n : nombre de tranches
 * @return le numéro de la tranche obtenue, de 0 à n - 1
 */
int drawFromTable(float pourcentages[], int n);

/***
 * @brief tire au hasard si un bourgeon qui reçoit la lumière lumiere pousse, selon la table de pousse
 * @param lumiere : lumière reçue par le bourgeon, entre 0 et 1
 * @return 1 si le bourgeon pousse, 0 sinon
 */
int canGrowInLight(double lumiere);

/* ---------------------------------------------------------------- à compléter : vos prototypes, à partir d'ici */

#endif //FORET_CROISSANCE_LUMIERE_H
