# TIPE de prépa

## Présentation

Ce repo contient tout un ensemble assez peu ordonné des morceaux du TIPE que je préparais pour le concours d'entrée à l'ENS en 2020, dont l'épreuve de TIPE a été annulée à cause de la pandémie. Le but était le suivant : à partir d'un ensemble d'images de poissons auxquelles sont associées leur espèces, être capable de donner l'espèce d'un poisson sur une autre image.

J'ai fait d'abord rapidement des tests avec k plus proches voisins, qui étaient évidemment mauvais dès que j'avais 23 espèces. Puis avec les mêmes images (de fish4knowledge), j'ai essayé des réseaux de neuronnes, qui donnaient tout de suite de bien meilleurs résultats. J'ai donc appris en même temps les réseaux de neuronnes, les différents layers (simple, convolution, etc...). J'ai ensuite utilisé des réseaux siamois dans le but de pouvoir apprendre à reconnaitre des espèces dont on ne dispose que de peu d'image. J'ai pour cela pris une base de données avec plus de 1500 espèces, mais peu d'images par espèce. Malheureusement, les résultats n'étaient pas excellents. Il faudrait affiner l'entrainement, peut-être apprendre d'abord sur un problème différent et utilise le transfer learningg (se base sur mobile-net améliorait considérablement les résultats), et simplement aussi trouver de meilleures images car celles-ci n'étaient pas très diversifiées.

Le but était ensuite d'utiliser la sortie du réseau siamois comme une projection de l'image dans un espace de plus petite dimension (32) de manière à ce que les paramètres aient plus de "sens" et permettent d'utiliser efficacement les k plus proches voisins. C'est pour cela que j'ai testé plusieurs techniques pour les k plus proches voisins, et surtout développé celle, efficace en 32 dimensions, consistant à utiliser un graphe naviguable (nsw pour être exacte). Je me suis rensegné sur le sujet, et ai essayé d'apported quelques améliorations à ce que j'ai trouvé, le résultat était assez efficace et parcourait une faible portion du graphe sur les très gros graphes, le rendant porbablement utilisable. Mes tests consistant à trouver LE point le plus proche, sur un graphe tel que les réseaux siamois donneraient, si il y a une dizaine d'image pas classe et qu'on cherche plusieurs points, on obtiendrait surement d'encore meilleurs résultats, car alors tout point proche serait acceptable.

Pour réaliser ces réseaux de neuronnes, j'ai commencé par implémenter une mini-librairie, DML ([visible ici](https://github.com/webalorn/deep-mola)) pour créer facilement les réseaux de neuronnes, basée sur theano. J'utilisait Theano pour effectuer rapidement les calculs et différencier automatiquement les fonctions que je créais, et DML construisait ces formules en fonctions des layers que je voulais. On peut voir ça comme un mini-keras basé sur theano, avec quelques routines en plus pour charger les données / afficher simplement les résultats (mais evidement beaucoup moins souple que keras-tensorflow). A partir des réseaux siamois, j'ai du utiliser google colab, et tensorflow me permettait d'utiliser facilement des cpu, nécessaires pour ne pas avoir des temps de chargement infernaux. De plus, switcher vers tensorflow / keras a aussi amélioré mes premiers résultats, car je pouvait faire tourner de plus gros résaux que sur mon laptop (plus quelques erreurs dans DML qui dégradaient peut-être les résultats).

Enfin, j'ai programmé les différentes méthodes pour les k plus proches voisins en C++, avec uniquement la librairie [JSON for Modern C++](https://github.com/nlohmann/json) pour stocker facilement des données. 

## Languages / frameworks utilisés

- Réseaux de neuronnes avec DML, au début
	- Python3
	- [Theano](https://github.com/Theano/Theano) « Theano is a Python library that allows you to define, optimize, and evaluate mathematical expressions involving multi-dimensional arrays efficiently. » - Developpement stoped
	- [DML](https://github.com/webalorn/deep-mola) La lib que j'ai codée, a des bugs et n'est pas super efficace, mais fut précieuse pour apprendre et comprendre les concepts du deep learning, mettre les mains dedans.
- Réseaux de neuronnes ensute
	- Python3, again
	- Keras + Tensorflow
- K plus proches voisins
	- C++

## Fichiers dans le repo

```
/code -> tout le code, sauf les notebooks
	/echoues -> Mauvaise tentative
	/knn -> les sous dossiers sont les différents méthodes pour K-NN
	/nnets
		/characters -> Tests avec MNIST
		/fishesClass -> Tous mes réseaux avec DML, entre autre
/notebooks -> codes faits sur google colab (les plus efficaces)
/presentation -> Des ébauches de présentations du TIPE
	/v1, v2, v3 -> Diaporamas pour l'oral
	/rapport -> Rapport ENS, jamais écrit
```