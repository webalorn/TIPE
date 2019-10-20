# Plan présentation TIPE

À faire (présentation) :

-   Titre
    -   Reconnaissance efficace d’objets sous-marins
-   Problématique
    -   Comment rapidement détecter et savoir reconnaitre de nouveaux objets sous-marins avec peu de données ?
-   Ligne directrice OK ?
-   Contact avec un professionel / une entreprise ?
    -   Données ?
-   Meilleurs données ? (APRÈS KNN !!!) [autres types, possiblement] (ou fonds, divers types de fonds, algues, etc...)
-   Si nouvelles données, ajouter
-   TRES IMPORTANT : Annexes avec tout ce qui ne tient pas, des trucs en plus, les dérivées d’un réseau, de tanh, sigmoid, etc… (+ des stats ?)
-   Beamer : refaire intro + plan

ENS : beamer ? Comment se passe la présentation ? Faire un beamer support pout tout, plutôt ?

À ajouter :

-   Libs utilisées, parties codées ou non. (update DML ; tout en DML ?)
-   Architectures
-   Complexités
-   Démos

Réaliser des illustrations

### Rapport ENS :

-   6 pages MAX + références bibliographiques, annexes et figures
    -   synthétique, lisible et citer ses sources
    -   tous les candidats incluent en annexe l’intégralité de leur code
    -   biblio de bonne qualité scientifique
-   Dans le cas d’un algorithme d’approximation, il est important de prouver des garanties en terme de qualité de la solution (facteur d’approximation) et de performance
    -   Analyse fine des choix, paramètres, etc...

## Déroulement Général

### Intro

Objectif : Système pouvant reconnaitre un nombre large de catégories avec peu d’images et sans ré-entrainer le système pour une nouvelle catégorie.

### Partie 0 : détection d'objets

Supprimer ou garder ? Algos de segmentation, NN de détection, pixel-wise detection

### Partie 1 : classifications basiques

-   KNN image
-   Classification avec un ANN
    -   Définition (1 neuronne), puis vecteurs
    -   Construction, différents layers (layer = type d’opération fait entre les neuronnes)
        -   Entrainables (fully-connected, conv)
        -   Paramétriques (maxpool, BatchNorm, autres...)
    -   Fonction de coût
    -   Entrainement

Donner résultats avec 3, 6, et toutes les classes

### Partie 2 : transformation de l'image

On utilise un réseau de neuronne pour aller de l’espace des images $128\times 128$ vers un espace plus petits des caractéristiques de l’image.

-   Siamese network
-   Triplets (anchor, positive, negative) et fonction de coût
-   Transfer learning et freeze

Résultats (stats et entrainement).

### Partie 3 : classifiation rapide avec KNN

TODO

2 problèmes :

-   réussite (contenir delaunay pour un graphe, etc...)
-   Efficacité (bon noeud d’entrée ? NSW ?)

### Conclusion

-   Donnés de mauvaise qualité et encore trop peu de classes
    -   Beaucoup de parasites
    -   Stats en dessous de ce qu’on pourrait avoir
    -   Overfitting très large, difficulté de généralisation
    -   Cependant, reconaissance fonctionnelle en partie, et apprentissage réalisé sur les images tests
-   En utilisant un CNN entrainé sur Image-Net, résultats hauts avant même d'entrainer

Le système est donc fonctionnel, mais nécessiterait plus de données, de meilleure qualité, et plus de puissance de calcul pour aller à son potentiel maximum. Entrainer sur des données / problèmes plus larges au préalable est une voie.



## Beamer



## Rapport	