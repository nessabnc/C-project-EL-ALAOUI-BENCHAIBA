
Durant le projet, on a eu plein de problèmes qui se sont enchaînés, surtout avec CLion et la configuration du projet.
D’abord, CLion ne reconnaissait pas du tout le projet comme un projet CMake.
Du coup, à chaque fois qu’on essayait de compiler, ça compilait uniquement main.c, ce qui donnait plein d’erreurs “undefined symbols”.
On ne comprenait surtout pas pourquoi tous les fichiers .c n’étaient pas compilés.
Ensuite, on a découvert que le vrai problème était que CMake n’était même pas installé sur la machine.
C’est pour ça qu’il n’y avait jamais le popup “Reload CMake Project” dans CLion, et que le bouton de compilation ne compilait seulement le main.
On a dû installer Homebrew, puis installer CMake, puis ajouter Brew dans le PATH parce que le terminal ne reconnaissait aucune commande (brew, cmake).
Même après ça, il a fallu corriger la ligne cmake_minimum_required dans CMakeLists.txt parce que CMake refuse les versions trop anciennes.
Après avoir fais les démarches nécessaires, tout s’est compilé correctement, et l’exécutable markov s’est enfin créé dans le dossier build.
