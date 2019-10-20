[TOC]

## Part 1 : RNA

## Part 2 : Siamese NNet

## Part 3 : KNN efficace

### Delaunay

Pour un esemble de points $P$ dans le plan, une triangulation de cet ensemble est un découpage de l’enveloppe convexe de ces points en en ensemble $DT(T)$ de triangles tels que tous les sommets soient des points de $P$, et que chaque point de $P$ soit sommet d’au moins un triangle. La triangulation de Delaunay est une triangulation telle que pour chaque triangle de $DT(T)$, il n’y ai aucun point de $P$ contenu dans le cercle circonscrit à ce triangle privé de son bord. On peut représenter cette triangulation par un graphe.

#### Existence

#### Propriété utile

>   Si un cercle passant par deux points ne contient aucun autre point à l’intérieur, alors le segment les reliant fait partie de la triangulation de Delaunay



#### Preuve de l'optimalité d'un sur-graphe de Delaunay pour KNN

(faire pour $\leqslant$ ?)

On considère l’algorithme gloutton consistant à toujours se rapprocher du voisin le plus proche de la cible $t$. On considère $v_0\in P$ le point le plus proche de $x$, minimisant $|t-v_0|$. On va considérer les points dans l’ordre $v_0, v_1\dots v_n/|v_0-t|<|v_1-t|<\dots<|v_n-t|$. On « ajoute » ainsi les éléments par cercles concentriques. On va montrer que a chaque étape, le point considéré est lié dans Delaunay a un point a une distance strictement plus petite.