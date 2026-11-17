# cmake/partie3.cmake : cibles de la partie 3 du projet (les statistiques), incluses par CMakeLists.txt
# les chemins sont relatifs au dossier du projet ; aucun fichier des parties 1 et 2 n'est modifié

# fichiers des parties 1 et 2 utilisés par la campagne
set(PARTIES_1_2
        src/arbre.c include/arbre.h
        src/queue.c include/queue.h
        src/croissance.c include/croissance.h
        src/croissance_step.c include/croissance_step.h
        src/arbre_foret.c include/arbre_foret.h
        src/croissance_lumiere.c include/croissance_lumiere.h
        src/foret.c include/foret.h
        utils/src/alea.c utils/include/alea.h)

# le module de statistiques : il ne sait rien des forêts (lecture, résumé et dessin fournis, histogramme à écrire)
set(STATISTIQUES
        src/statistiques.c include/statistiques.h)

# étape 1 : le programme de campagne (ligne de commande)
add_executable(p3_etape_1 src/main_p3_etape1.c src/campagne.c include/campagne.h ${PARTIES_1_2})

# étape 2 : l'histogramme (lecture, résumé et dessin SVG fournis dans statistiques.c)
add_executable(p3_etape_2 src/main_p3_etape2.c ${STATISTIQUES})

# étape 3 : l'outil d'analyse (ligne de commande)
add_executable(p3_etape_3 src/main_p3_etape3.c ${STATISTIQUES})

# réglages communs aux cibles de la partie 3 (les mêmes que pour les parties 1 et 2)
foreach (cible p3_etape_1 p3_etape_2 p3_etape_3)
    target_include_directories(${cible} PRIVATE include utils/include)

    target_compile_options(${cible} PRIVATE -Wall -Wextra)

    # bibliothèque mathématique (math.h), inutile sous MSVC
    if (NOT MSVC)
        target_link_libraries(${cible} PRIVATE m)
    endif ()
endforeach ()
