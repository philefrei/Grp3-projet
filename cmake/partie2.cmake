# cmake/partie2.cmake : cibles de la partie 2 du projet (la forêt), incluses par CMakeLists.txt
# les chemins sont relatifs au dossier du projet ; aucun fichier de la partie 1 n'est modifié

# fichiers de la partie 1 utilisés par toutes les cibles de la partie 2
set(PARTIE1
        src/arbre.c include/arbre.h
        src/queue.c include/queue.h
        src/croissance.c include/croissance.h
        src/croissance_step.c include/croissance_step.h
        utils/src/arbres_utils.c utils/include/arbres_utils.h
        utils/src/dessin.c utils/include/dessin.h
        utils/src/alea.c utils/include/alea.h)

# fichiers de la partie 2, fournis vides : l'arbre de la forêt (étapes 1 et 3), la pousse selon la lumière (étape 2)
set(PARTIE2_ARBRE
        src/arbre_foret.c include/arbre_foret.h
        src/croissance_lumiere.c include/croissance_lumiere.h)

# la forêt (étapes 4 à 6), fournie vide
set(PARTIE2_FORET
        src/foret.c include/foret.h)

# étape 1 : les mesures d'un arbre de la forêt
add_executable(p2_etape_1 src/main_p2_etape1.c ${PARTIE2_ARBRE} ${PARTIE1})

# étape 2 : le nombre de nouveaux rameaux selon la lumière
add_executable(p2_etape_2 src/main_p2_etape2.c ${PARTIE2_ARBRE} ${PARTIE1})

# étape 3 : pousser avec une lumière donnée
add_executable(p2_etape_3 src/main_p2_etape3.c ${PARTIE2_ARBRE} ${PARTIE1})

# étape 4 : la grille d'occupation
add_executable(p2_etape_4 src/main_p2_etape4.c ${PARTIE2_FORET} ${PARTIE2_ARBRE} ${PARTIE1})

# étape 5 : la lumière
add_executable(p2_etape_5 src/main_p2_etape5.c ${PARTIE2_FORET} ${PARTIE2_ARBRE} ${PARTIE1})

# étape 6 : la forêt qui pousse, et sa sauvegarde par saveForest (sauver_foret.c, fourni)
add_executable(p2_etape_6 src/main_p2_etape6.c
        src/sauver_foret.c include/sauver_foret.h
        ${PARTIE2_FORET} ${PARTIE2_ARBRE} ${PARTIE1})

# réglages communs aux cibles de la partie 2 (les mêmes que pour la partie 1)
foreach (cible p2_etape_1 p2_etape_2 p2_etape_3 p2_etape_4 p2_etape_5 p2_etape_6)
    target_include_directories(${cible} PRIVATE include utils/include)

    target_compile_options(${cible} PRIVATE -Wall -Wextra)

    # bibliothèque mathématique (math.h), inutile sous MSVC
    if (NOT MSVC)
        target_link_libraries(${cible} PRIVATE m)
    endif ()
endforeach ()
