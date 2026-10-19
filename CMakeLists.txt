# CMakeLists.txt du projet TI301 - la forêt et la lumière
#
# Pour CLion ou en ligne de commande :
#   cmake -S . -B build
#   cmake --build build

cmake_minimum_required(VERSION 3.10)
project(foret C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(foret
        src/main.c
        src/arbre.c include/arbre.h
        src/queue.c include/queue.h
        src/croissance.c include/croissance.h
        src/croissance_step.c include/croissance_step.h
        src/sauver_arbre.c include/sauver_arbre.h
        utils/src/arbres_utils.c utils/include/arbres_utils.h
        utils/src/dessin.c utils/include/dessin.h
        utils/src/alea.c utils/include/alea.h)

# étape 1 : prise en main de l'environnement (lire, afficher, dessiner, animer un arbre)
add_executable(etape_1
        src/main_etape1.c
        src/arbre.c include/arbre.h
        utils/src/arbres_utils.c utils/include/arbres_utils.h
        utils/src/dessin.c utils/include/dessin.h
        utils/src/alea.c utils/include/alea.h)

# étape 2 : la pousse d'un arbre en pleine lumière (fonctions de croissance, file de rameaux)
add_executable(etape_2
        src/main_etape2.c
        src/arbre.c include/arbre.h
        src/queue.c include/queue.h
        src/croissance.c include/croissance.h
        utils/src/arbres_utils.c utils/include/arbres_utils.h
        utils/src/dessin.c utils/include/dessin.h
        utils/src/alea.c utils/include/alea.h)

# étape 4 : sauvez les arbres ! (sauvegarde d'un arbre dans un fichier, par un parcours en largeur)
add_executable(etape_4
        src/main_etape4.c
        src/arbre.c include/arbre.h
        src/queue.c include/queue.h
        src/croissance.c include/croissance.h
        src/sauver_arbre.c include/sauver_arbre.h
        utils/src/arbres_utils.c utils/include/arbres_utils.h
        utils/src/alea.c utils/include/alea.h)

# réglages communs à toutes les cibles
foreach (cible foret etape_1 etape_2 etape_4)
    # les .h du projet et ceux des utilitaires fournis
    target_include_directories(${cible} PRIVATE include utils/include)

    target_compile_options(${cible} PRIVATE -Wall -Wextra)

    # bibliothèque mathématique (math.h), inutile sous MSVC
    if (NOT MSVC)
        target_link_libraries(${cible} PRIVATE m)
    endif ()
endforeach ()

# parties suivantes du projet : chaque fichier, fourni plus tard, déclare ses cibles et leurs réglages
include(cmake/partie2.cmake OPTIONAL)
include(cmake/partie3.cmake OPTIONAL)
