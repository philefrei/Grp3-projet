/* arbres_utils.c : module fourni (lecture et affichage d'un arbre) */
#include <stdio.h>
#include <stdlib.h>
#include "arbres_utils.h"

/* une ligne du fichier : un rameau autre que le pied */
typedef struct
{
    int num, saison, parent, gen, etat;
    float dir, longueur;
} t_ligne;

/* lit la ligne suivante du fichier ; renvoie 1 si elle a pu être lue, 0 sinon */
static int readLine(FILE *f, t_ligne *l)
{
    return fscanf(f, "%d %d %d %f %f %d %d",
                  &l->num, &l->saison, &l->parent, &l->dir, &l->longueur, &l->gen, &l->etat) == 7;
}

t_arbre readTree(const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "rt");    // lecture seule, texte
    float x, y;
    t_arbre a;
    if (f == NULL)
    {
        perror("Impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }
    if (fscanf(f, "%f %f", &x, &y) != 2)
    {
        fprintf(stderr, "Impossible de lire la position du pied\n");
        exit(EXIT_FAILURE);
    }
    // pied construit sans createTree : lire un fichier ne doit pas réinitialiser le hasard
    a.pied = createBranch(x, y, 0.0f, y, 0, 0, 0);
    a.nb_rameaux = 1;
    a.seed = 0;    // graine inconnue : elle n'est pas enregistrée dans le fichier
    // file des rameaux déjà lus, dans l'ordre du parcours en largeur :
    // les cases tete à fin - 1 contiennent les rameaux qui peuvent encore être parents
    int capacite = 16, tete = 0, fin = 0;
    t_rameau **file = malloc(capacite * sizeof(t_rameau *));
    if (file == NULL)
    {
        perror("readTree");
        exit(EXIT_FAILURE);
    }
    file[fin++] = a.pied;
    t_ligne l;
    int lu = readLine(f, &l);
    while (lu)
    {
        // en BFS, les fils d'un même rameau sont sur des lignes consécutives :
        // on lit tout le groupe pour connaître le nombre de fils avant d'allouer p_fils
        int parent = l.parent, nb = 0;
        t_ligne groupe[NB_FILS_MAX];
        while (lu && l.parent == parent)
        {
            if (nb == NB_FILS_MAX)
            {
                fprintf(stderr, "%s : le rameau %d a plus de %d fils\n", nom_fichier, parent, NB_FILS_MAX);
                exit(EXIT_FAILURE);
            }
            groupe[nb++] = l;
            lu = readLine(f, &l);
        }
        // les fils d'un rameau suivent ceux des rameaux placés avant lui dans la file :
        // les rameaux passés en tête de file n'auront plus de fils
        while (tete < fin && file[tete]->num != parent)
        {
            tete++;
        }
        if (tete == fin || file[tete]->nb_fils > 0)
        {
            fprintf(stderr, "%s : parent %d du rameau %d introuvable (fichier pas en BFS ?)\n",
                    nom_fichier, parent, groupe[0].num);
            exit(EXIT_FAILURE);
        }
        t_rameau *p = file[tete];
        p->p_fils = malloc(nb * sizeof(t_rameau *));
        if (p->p_fils == NULL)
        {
            perror("readTree");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < nb; i++)
        {
            t_rameau *r = growBranch(&a, p, groupe[i].saison, groupe[i].dir, groupe[i].longueur);
            // le numéro, la génération et l'état sont ceux du fichier
            r->num = groupe[i].num;
            r->gen = groupe[i].gen;
            r->etat = groupe[i].etat == BOIS ? BOIS : BOURGEON;
            p->p_fils[i] = r;
            p->nb_fils++;
            // le nouveau rameau entre dans la file (agrandie si besoin)
            if (fin == capacite)
            {
                capacite *= 2;
                t_rameau **nouveau = realloc(file, capacite * sizeof(t_rameau *));
                if (nouveau == NULL)
                {
                    perror("readTree");
                    exit(EXIT_FAILURE);
                }
                file = nouveau;
            }
            file[fin++] = r;
        }
    }
    free(file);
    fclose(f);
    return a;
}

/* range l'adresse de r et celles de tous ses descendants dans tab, à l'indice de leur numéro (fonction récursive) */
static void storeByNumber(t_rameau *r, t_rameau **tab, int n)
{
    if (r->num >= 0 && r->num < n)
    {
        tab[r->num] = r;
    }
    for (int i = 0; i < r->nb_fils; i++)
    {
        storeByNumber(r->p_fils[i], tab, n);
    }
}

void displayTree(t_arbre a)
{
    t_rameau **tab = calloc(a.nb_rameaux, sizeof(t_rameau *));
    if (tab == NULL)
    {
        perror("displayTree");
        exit(EXIT_FAILURE);
    }
    storeByNumber(a.pied, tab, a.nb_rameaux);
    printf("Arbre : %d rameaux\n", a.nb_rameaux);
    for (int i = 0; i < a.nb_rameaux; i++)
    {
        t_rameau *r = tab[i];
        if (r == NULL)
        {
            printf("Rameau %d absent\n", i);
            continue;
        }
        printf("Rameau %d (saison %d, generation %d, %s) (%.2f ; %.2f)",
               r->num, r->saison, r->gen, r->etat == BOIS ? "bois" : "bourgeon", r->x, r->y);
        if (r->nb_fils > 0)
        {
            printf(" fils");
            for (int j = 0; j < r->nb_fils; j++)
            {
                printf(" -> %d", r->p_fils[j]->num);
            }
        }
        printf("\n");
    }
    free(tab);
}
