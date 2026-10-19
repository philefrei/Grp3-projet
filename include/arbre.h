/**
 * @file arbre.h
 * @brief Structures et construction d'un arbre (module fourni).
 *
 * Un arbre est formé de rameaux. Chaque rameau est un segment qui part de
 * l'extrémité de son père et dont on mémorise l'extrémité (x ; y).
 * Un rameau a au plus NB_FILS_MAX fils. Le premier rameau, le pied, part du sol.
 */
#ifndef ARBRE_H
#define ARBRE_H

/** @brief Valeur approchée de pi, pour convertir les degrés en radians. */
#define PI 3.14159265358979

/** @brief États possibles d'un rameau (champ t_rameau::etat). */
typedef enum e_type
{
    BOURGEON = 0,   /**< rameau sans fils, qui peut encore pousser */
    BOIS = 1        /**< rameau qui a donné au moins un fils */
} t_type;

/**
 * @brief Un rameau de l'arbre (nœud de l'arbre).
 */
typedef struct s_rameau
{
    int num;                   /**< numéro du rameau (ordre d'apparition, 0 pour le pied) */

    float longueur;           /**< longueur du rameau */
    float x;                   /**< extrémité du rameau : position horizontale */
    float y;                   /**< extrémité du rameau : altitude */
    float dir;                 /**< direction en degrés (0 = vertical, positif vers la droite) */

    int saison;              /**< saison d'apparition */
    int gen;                 /**< génération (profondeur dans l'arbre, 0 pour le pied) */
    t_type etat;              /**< BOURGEON ou BOIS */

    unsigned char nb_fils;     /**< nombre de fils, de 0 à NB_FILS_MAX */
    struct s_rameau **p_fils;    /**< rameaux nés à son extrémité : tableau dynamique de nb_fils cases (NULL si aucun fils) */
} t_rameau;

/** @brief Nombre maximal de fils d'un rameau. */
#define NB_FILS_MAX 3

/**
 * @brief Un arbre : son pied, son nombre de rameaux et sa graine.
 */
typedef struct
{
    t_rameau *pied;              /**< premier rameau : la racine de l'arbre */
    int nb_rameaux;            /**< nombre de rameaux de l'arbre */
    int seed;                  /**< graine du générateur aléatoire utilisée pour faire pousser l'arbre */
} t_arbre;

/**
 * @brief Crée un rameau (bourgeon, sans fils).
 *
 * Le rameau est alloué dynamiquement ; son tableau de fils est vide (p_fils vaut NULL).
 * Le programme s'arrête si l'allocation échoue.
 *
 * @param x        position horizontale de l'extrémité
 * @param y        altitude de l'extrémité
 * @param dir      direction en degrés (0 = vertical)
 * @param longueur longueur du rameau
 * @param saison   saison d'apparition
 * @param gen      génération (profondeur dans l'arbre)
 * @param num      numéro du rameau
 * @return l'adresse du rameau créé
 */
t_rameau *createBranch(float x, float y, float dir, float longueur, int saison, int gen, int num);

/**
 * @brief Crée un arbre ne contenant que son pied.
 *
 * Le pied est un bourgeon vertical de numéro 0, de génération 0, apparu à la saison 0.
 * Il part du sol, au point (x ; 0) : sa longueur vaut donc y.
 * La graine est mémorisée dans l'arbre, et le générateur aléatoire est initialisé
 * avec elle (randomInit) : faites pousser l'arbre juste après l'avoir créé.
 * Le programme s'arrête si une allocation échoue.
 *
 * @param x    position horizontale de l'extrémité du pied
 * @param y    altitude de l'extrémité du pied
 * @param seed graine du générateur aléatoire
 * @return l'arbre créé
 */
t_arbre createTree(float x, float y, int seed);

/**
 * @brief Crée un nouveau rameau qui pousse à l'extrémité d'un rameau existant.
 *
 * L'extrémité du nouveau rameau est calculée à partir de celle du parent,
 * de la direction et de la longueur. Le nouveau rameau est un bourgeon de génération
 * parent->gen + 1, qui reçoit le numéro a->nb_rameaux (le nombre de rameaux de l'arbre
 * augmente de 1). Le parent passe à l'état BOIS.
 *
 * Le nouveau rameau n'est pas rangé dans le tableau p_fils du parent : c'est à l'appelant
 * d'allouer ce tableau (il connaît le nombre de fils), d'y placer le rameau et de mettre
 * à jour parent->nb_fils.
 *
 * @param a        arbre auquel appartient le parent (modifié)
 * @param parent   rameau à l'extrémité duquel pousse le nouveau rameau
 * @param saison   saison d'apparition du nouveau rameau
 * @param dir      direction du nouveau rameau, en degrés (0 = vertical)
 * @param longueur longueur du nouveau rameau
 * @return l'adresse du nouveau rameau
 */
t_rameau *growBranch(t_arbre *a, t_rameau *parent, int saison, float dir, float longueur);

#endif
