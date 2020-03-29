# Plan présentation TIPE

À faire (présentation) :

-   Titre
    -   Reconnaissance rapide d’objets sous-marins
-   Mettre les répartitions des donnnées
-   Parler des caractères
-   exppliquer bien la nouvelle problématique : gérer l'innatendu
-   Problématique
    -   Comment rapidement détecter et savoir reconnaitre de nouveaux objets sous-marins avec peu de données ?
    -   Plus de ressemblance avec un être vivant  / autonome en apprenant ?
-   Ligne directrice OK ?
-   Si nouvelles données, ajouter
-   TRES IMPORTANT : Annexes avec tout ce qui ne tient pas, des trucs en plus, les dérivées d’un réseau, de tanh, sigmoid, etc… (+ des stats ?)
-   Beamer : refaire intro + plan
-   Annexe beamer + rapport : toutes les couches, les fonctions, etc sous forme de doc de reférence + les architectures

ENS : beamer ? Comment se passe la présentation ? Faire un beamer support pout tout, plutôt ?

À ajouter :

-   Libs utilisées, parties codées ou non. (update DML ; tout en DML ?)
-   Architectures
-   Complexités
-   Démos

Réaliser des illustrations

Solutions constantes plutôt qu’aléatoire ? Parler systèmes déjà existants, et de reconaissance signature / faciale. Pas mettre en avant le jargon.

### Rapport ENS :

-   6 pages MAX (notice : 2 à 5 pages, max 12 500 caractères) + références bibliographiques, annexes et figures
    -   synthétique, lisible et citer ses sources
    -   tous les candidats incluent en annexe l’intégralité de leur code
    -   biblio de bonne qualité scientifique
-   Dans le cas d’un algorithme d’approximation, il est important de prouver des garanties en terme de qualité de la solution (facteur d’approximation) et de performance
    -   Analyse fine des choix, paramètres, etc...

## Déroulement Général

### Intro

Objectif : Système pouvant reconnaitre un nombre large de catégories avec peu d’images et sans ré-entrainer le système pour une nouvelle catégorie, ou ajouter de nouvelles données.

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

Une autre voie est de segmenter l’image, supprimer le fond et identifier clairement l’objet. On pourrait aussi entrainer le réseau de neuronnes à donner des caractéristiques choisies à la main pour chaque image.



## Beamer



## Rapport	