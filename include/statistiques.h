/* statistiques.h : partie 3, étape 2 — lire une colonne d'un fichier CSV, la résumer, son histogramme (code fourni, à compléter)
   ce module ne sait rien des forêts : il sert pour n'importe quel fichier CSV */

#ifndef FORET_STATISTIQUES_H
#define FORET_STATISTIQUES_H

/* ---------------------------------------------------------------- code fourni */

/***
 * @brief lit toutes les valeurs d'une colonne d'un fichier CSV (séparateur ';', lignes commençant par '#' ignorées,
 *        première ligne restante = en-tête)
 * @param nom_fichier : fichier à lire
 * @param colonne : nom de la colonne, tel qu'il est écrit dans l'en-tête
 * @param p_n : reçoit le nombre de valeurs lues
 * @return un tableau dynamique des *p_n valeurs, à libérer avec free ; NULL si le fichier ou la colonne n'existe pas
 */
double *readColumn(const char *nom_fichier, const char *colonne, int *p_n);

/***
 * @brief trie un tableau de valeurs dans l'ordre croissant, avec qsort
 * @param v : valeurs
 * @param n : nombre de valeurs
 */
void sortValues(double v[], int n);

/***
 * @brief moyenne de n valeurs
 * @param v : valeurs
 * @param n : nombre de valeurs
 * @return la moyenne
 */
double computeMean(double v[], int n);

/***
 * @brief écart-type de n valeurs : racine carrée de la moyenne des carrés des écarts à la moyenne
 * @param v : valeurs
 * @param n : nombre de valeurs
 * @return l'écart-type
 */
double computeStdDev(double v[], int n);

/***
 * @brief indicateur d'asymétrie de n valeurs : moyenne des cubes des écarts à la moyenne, divisée par s^3 ; 0 si s = 0
 * @param v : valeurs
 * @param n : nombre de valeurs
 * @return l'indicateur d'asymétrie
 */
double computeSkewness(double v[], int n);

/***
 * @brief médiane de n valeurs déjà triées
 * @param trie : valeurs triées dans l'ordre croissant
 * @param n : nombre de valeurs
 * @return la médiane
 */
double computeMedian(double trie[], int n);

/***
 * @brief premier (q = 1) ou troisième (q = 3) quartile de n valeurs déjà triées : la valeur de rang ceil(q n / 4),
 *        les rangs étant numérotés à partir de 1
 * @param trie : valeurs triées dans l'ordre croissant
 * @param n : nombre de valeurs
 * @param q : 1 ou 3
 * @return le quartile
 */
double computeQuartile(double trie[], int n, int q);

/***
 * @brief affiche le résumé de n valeurs : nombre, moyenne, écart-type, minimum, maximum, médiane, quartiles, asymétrie ;
 *        le tableau v n'est pas modifié
 * @param v : valeurs
 * @param n : nombre de valeurs
 */
void displaySummary(double v[], int n);

/***
 * @brief dessine un histogramme dans un fichier SVG, à ouvrir avec un navigateur : une barre par classe,
 *        les effectifs sur l'axe vertical, les bornes des classes sous l'axe horizontal
 * @param effectifs : effectif de chaque classe
 * @param nb_classes : nombre de classes
 * @param min : borne gauche de la première classe
 * @param largeur : largeur d'une classe
 * @param titre : titre écrit au-dessus de l'histogramme
 * @param nom_fichier : fichier SVG à écrire
 */
void histoToSVG(int effectifs[], int nb_classes, double min, double largeur, const char *titre, const char *nom_fichier);

/* ---------------------------------------------------------------- à compléter : vos prototypes, à partir d'ici */

#endif //FORET_STATISTIQUES_H
