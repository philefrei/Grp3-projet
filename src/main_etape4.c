/* main_etape4.c : étape 4, sauvez les arbres !
   écrire un arbre dans un fichier avec saveTree, le comparer aux fichiers fournis, puis le relire avec readTree */

#include <stdio.h>
#include "arbre.h"
#include "arbres_utils.h"
#include "croissance.h"
#include "sauver_arbre.h"

/* renvoie 1 si les deux fichiers texte ont le même contenu (fins de ligne Windows ou Unix ignorées), 0 sinon */
static int sameText(const char *nom1, const char *nom2)
{
    FILE *f1 = fopen(nom1, "rb");
    FILE *f2 = fopen(nom2, "rb");
    int c1 = 0, c2 = 0;

    if (f1 != NULL && f2 != NULL)
    {
        do
        {
            do { c1 = fgetc(f1); } while (c1 == '\r');
            do { c2 = fgetc(f2); } while (c2 == '\r');
        } while (c1 == c2 && c1 != EOF);
    }
    if (f1 != NULL) fclose(f1);
    if (f2 != NULL) fclose(f2);
    return f1 != NULL && f2 != NULL && c1 == c2;
}

int main(void)
{
    t_arbre a, b;

    // 1. graine 5, 3 saisons : le fichier écrit doit être identique à data/arbre_petit.txt
    a = createTree(0, 1, 5);
    growTree(&a, 3);
    saveTree(a, "output/etape4_petit.txt");
    printf("graine 5, 3 saisons : identique a data/arbre_petit.txt ? %s\n",
           sameText("output/etape4_petit.txt", "data/arbre_petit.txt") ? "oui" : "NON");

    // 2. le fichier écrit doit pouvoir être relu par readTree, et redonner le même arbre
    b = readTree("output/etape4_petit.txt");
    printf("arbre sauve :\n");
    displayTree(a);
    printf("arbre relu :\n");
    displayTree(b);
    printf("\n");

    // 3. graine 3, 8 saisons et graine 4, 30 saisons
    a = createTree(0, 1, 3);
    growTree(&a, 8);
    saveTree(a, "output/etape4_moyen.txt");
    printf("graine 3, 8 saisons : identique a data/arbre_moyen.txt ? %s\n",
           sameText("output/etape4_moyen.txt", "data/arbre_moyen.txt") ? "oui" : "NON");
    a = createTree(0, 1, 4);
    growTree(&a, 30);
    saveTree(a, "output/etape4_grand.txt");
    printf("graine 4, 30 saisons : identique a data/arbre_grand.txt ? %s\n",
           sameText("output/etape4_grand.txt", "data/arbre_grand.txt") ? "oui" : "NON");

    return 0;
}
