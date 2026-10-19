//
// Created by flasque on 12/10/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "croissance.h"
#include "queue.h"

// prototypes des fonctions locales

float clampDirection(float);

// code des fonctions locales

float clampDirection(float dir)
{
    float res = dir;

    if (res < -80)
    {
        res = -80;
    }
    else if (res > 80)
    {
        res = 80;
    }

    return res;
}

// code des fonctions exportées

int canGrow(float probabilite)
{
    return (randomReal() < probabilite / 100.0);
}
