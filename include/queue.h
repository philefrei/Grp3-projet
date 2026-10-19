//
// Created by flasque on 13/10/2026.
//

#ifndef FICHIERS_FOURNIS_QUEUE_H
#define FICHIERS_FOURNIS_QUEUE_H

#include "arbre.h"

struct s_qcell
{
    t_rameau *data;
    struct s_qcell *next;
};

typedef struct s_qcell t_qcell;

typedef struct s_queue
{
    t_qcell *head;
    t_qcell *tail;
} t_queue;

/***
 * @brief crée une file vide
 * @return une file dont head et tail valent NULL
 */
t_queue createEmptyQueue(void);

/***
 * @brief teste si une file est vide
 * @param t_queue : file à tester
 * @return 1 si la file est vide, 0 sinon
 */
int isEmptyQueue(t_queue);

/***
 * @brief ajoute un rameau en queue de file (côté tail)
 * @param t_queue * : adresse de la file à modifier
 * @param t_rameau * : adresse du rameau à enfiler
 * @return : void
 */
void enqueue(t_queue *, t_rameau *);

/***
 * @brief retire le rameau en tête de file (côté head)
 * @param t_queue * : adresse de la file à modifier
 * @return l'adresse du rameau retiré, NULL si la file est vide
 */
t_rameau *dequeue(t_queue *);

/***
 * @brief affiche les numéros des rameaux de la file, de la sortie (head) vers l'entrée (tail)
 * @param t_queue : file à afficher
 * @return : void
 */
void displayQueue(t_queue);

#endif //FICHIERS_FOURNIS_QUEUE_H
