/* campagne.h : partie 3, étape 1 — une campagne de simulations : R forêts, mesures écrites dans deux fichiers CSV (fichier fourni vide, à compléter) */

#ifndef FORET_CAMPAGNE_H
#define FORET_CAMPAGNE_H

#include "foret.h"

// pas HMAX ni P_max : ce sont déjà des champs de t_foret
#define H_MAX 160
#define P_MAX 20000
#define LONGUEUR_NOM 256

typedef struct s_campagne
{
    int K, E, S;
    double A;
    int R;
    int graine;
    int identique;
    char sortie[LONGUEUR_NOM];
} t_campagne;

t_campagne createCampaign(void);

// renvoie 0 si l'un des fichiers n'a pas pu être créé
int runCampaign(t_campagne c);

#endif //FORET_CAMPAGNE_H
