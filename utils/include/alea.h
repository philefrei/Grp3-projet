/**
 * @file alea.h
 * @brief Générateur de nombres pseudo-aléatoires reproductible (module fourni, ne pas modifier).
 *
 * Le générateur est déterministe : avec la même graine, il produit toujours
 * la même suite de nombres, sur toutes les machines. Cela permet de rejouer
 * exactement une même pousse d'arbre.
 *
 * Sans appel à randomInit(), une graine par défaut est utilisée.
 */
#ifndef ALEA_H
#define ALEA_H

#include <stdint.h>

/**
 * @brief Choisit la graine du générateur.
 *
 * Deux exécutions utilisant la même graine produisent la même suite de nombres.
 *
 * @param graine valeur initiale du générateur
 */
void randomInit(uint64_t graine);

/**
 * @brief Tire un nombre à virgule au hasard.
 * @return un réel uniformément réparti dans [0 ; 1[
 */
double randomReal(void);

/**
 * @brief Tire un nombre à virgule au hasard entre deux bornes.
 * @param a borne inférieure (incluse)
 * @param b borne supérieure (exclue)
 * @return un réel uniformément réparti dans [a ; b[
 */
double randomBetween(double a, double b);

/**
 * @brief Tire un entier au hasard.
 * @param n nombre de valeurs possibles (doit être strictement positif)
 * @return un entier uniformément réparti dans {0, 1, ..., n-1}
 */
int randomInt(int n);

#endif
