
On comprenait que les SCC deviennent des nœuds, mais le document ne montre pas clairement un exemple complet (graphe → SCC → graphe réduit).
Ce n’était pas évident au début de voir comment organiser les nouveaux nœuds.-30/10

La partie sur le Hasse est assez brève, alors que le concept est nouveau.
On n’avait pas tout de suite compris ce qu’est exactement un diagramme de Hasse ni comment il se lit. -9/11


On avait du mal à comprendre comment réorganiser les nœuds par composantes.
On mélangeait les index et les identifiants, du coup tout était placé au mauvais endroit.-15/11

On confondait les états absorbants avec les états qui n’ont simplement pas de transitions entrantes.
On a dû revoir ce que veut dire “absorbant” dans une matrice. -19/11

vérification et fonctionnement du projet:
Durant le projet, on a eu plein de problèmes qui se sont enchaînés, surtout avec CLion et la configuration du projet.
D’abord, CLion ne reconnaissait pas du tout le projet comme un projet CMake.
Du coup, à chaque fois qu’on essayait de compiler, ça compilait uniquement main.c, ce qui donnait plein d’erreurs “undefined symbols”.
On ne comprenait surtout pas pourquoi tous les fichiers .c n’étaient pas compilés.
Ensuite, on a découvert que le vrai problème était que CMake n’était même pas installé sur la machine.
C’est pour ça qu’il n’y avait jamais le popup “Reload CMake Project” dans CLion, et que le bouton de compilation ne compilait seulement le main.
On a dû installer Homebrew, puis installer CMake, puis ajouter Brew dans le PATH parce que le terminal ne reconnaissait aucune commande (brew, cmake).
Même après ça, il a fallu corriger la ligne cmake_minimum_required dans CMakeLists.txt parce que CMake refuse les versions trop anciennes.
Après avoir fais les démarches nécessaires, tout s’est compilé correctement, et l’exécutable markov s’est enfin créé dans le dossier build.
-23/11/2025
