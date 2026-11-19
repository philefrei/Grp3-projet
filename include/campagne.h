/* campagne.h : partie 3, étape 1 — une campagne de simulations : R forêts, mesures écrites dans deux fichiers CSV (fichier fourni vide, à compléter) */

#ifndef FORET_CAMPAGNE_H
#define FORET_CAMPAGNE_H

#include "foret.h"

// pas HMAX ni P_max : ce sont déjà des champs de t_foret
#define H_MAX 160
#define P_MAX 20000
#define LONGUEUR_NOM 256


/*
 
@brief les réglages d'une campagne de simulations*/
typedef struct s_campagne
{
    int K, E, S;                // nb d'arbres, écartement, nb de saisons
    double A;                   // part de lumière retenue par une extrémité
    int R;                      // nb de forêts à simuler
    int graine;                 // la forêt r est plantée avec la graine graine + r
    int identique;              // 1 : pieds tous à l'altitude 1
    char sortie[LONGUEUR_NOM];  // début du nom des deux fichiers CSV
} t_campagne;

/*
@brief crée une campagne avec les réglages par défaut : K 50, E 3, A 0,25, S 120, R 1, graine 1,
plantation au hasard, sortie "output/campagne"
@return la campagne*/
t_campagne createCampaign(void);

/***
 
@brief simule les R forêts une par une et écrit leurs mesures dans <sortie>_arbres.csv (une ligne par arbre)
et <sortie>_forets.csv (une ligne par forêt) ; affiche une ligne par forêt et la durée du calcul
@param c : la campagne
@return 1, ou 0 si les réglages sont invalides ou si un fichier n'a pas pu être créé*/
int runCampaign(t_campagne c);

#endif //FORET_CAMPAGNE_H
