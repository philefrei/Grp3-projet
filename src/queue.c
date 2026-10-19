//
// Created by flasque on 13/10/2026.
//
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

// prototypes des fonctions locales

t_qcell *createQCell(t_rameau *);

// code des fonctions locales

t_qcell *createQCell(t_rameau *p_r)
{
    t_qcell *p_res;

    p_res = (t_qcell *) malloc(sizeof(t_qcell));
    if (p_res == NULL)
    {
        printf("CRITICAL ERROR : cannot allocate queue cell\n");
        exit(-1);
    }

    p_res->data = p_r;
    p_res->next = NULL;

    return p_res;
}

// code des fonctions exportées

t_queue createEmptyQueue(void)
{
    t_queue q;

    q.head = NULL;
    q.tail = NULL;

    return q;
}

int isEmptyQueue(t_queue q)
{
    return (q.head == NULL);
}

void enqueue(t_queue *p_q, t_rameau *p_r)
{
    t_qcell *nouv;
    nouv = createQCell(p_r);

    if (isEmptyQueue(*p_q))
    {
        p_q->head = nouv;
    }
    else
    {
        p_q->tail->next = nouv;
    }
    p_q->tail = nouv;

    return;
}

t_rameau *dequeue(t_queue *p_q)
{
    t_rameau *res;
    t_qcell *temp;

    if (isEmptyQueue(*p_q))
    {
        printf("queue is empty - cannot dequeue\n");
        res = NULL;
    }
    else
    {
        temp = p_q->head;
        res = temp->data;
        p_q->head = temp->next;
        if (p_q->head == NULL)
        {
            p_q->tail = NULL;
        }
        free(temp);
    }

    return res;
}

void displayQueue(t_queue q)
{
    t_qcell *temp = q.head;

    printf("out <- ");
    while (temp != NULL)
    {
        printf("%d ", temp->data->num);
        temp = temp->next;
    }
    printf("<- in\n");

    return;
}
