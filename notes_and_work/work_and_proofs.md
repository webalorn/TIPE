[TOC]

# Part 1 : ANN, autres méthodes

## K plus proches voisins

On se place dans un espace de dimension fixée, qui va correspondre à la dimension à laquelle on redimensionne toutes les images (ici, dimension 30000). A chaque image est associée un label. On choisit ensuite une norme de l’espace : ici, $L_2$. On considère l’ensemble $D$ des images dont on connait le label, et une image $m$ que l’on cherche a classer. On va lui associer le label de l’image $\underset{m’\in A}{argmin}\ ||m-m’||$.

**Cas de la densité de couleurs**

En considérant les images comme une matrice $3\times n\times n$, on applique la transformation $A’=\{[avg(m[0]), avg(m[1]), avg(m[2])]\ /\ m\in A\}$. Cela va accélérer la recherche par un facteur $n^2$ mais réduire la précision.

## Réseau de neurone


```
### Neurones et couches (PAS UP TO DATE)

Un neuronne prend en entrée un vecteur $A=\pmatrix{a_1\\\vdots\\a_n}$. Il possède certaines valeurs réelles qui sont ses paramètres : $W=\pmatrix{w_1\\\vdots\\w_n}$ qui sont les poids et $b$ le biais. La valeur de ces paramètres seront appris par le réseau de neurones. Une *fonction d’activation* $\sigma$ est aussi associée au neurone. Le neuronne calcule sa *valeur d'activation* $z=b +\sum_{i=1}^n w_ia_i$, puis sa sortie $y=\sigma(z)$. 

Les fonctions d’activation peuvent être :

| Sigmoïde                       | Tanh                | ReLU                 | Weak ReLU                           |
| ------------------------------ | ------------------- | -------------------- | ----------------------------------- |
| $\sigma(z)=\frac{1}{1+e^{-z}}$ | $\sigma(z)=tanh(z)$ | $\sigma(z)=max(z,0)$ | $\sigma(z)=max(z,\epsilon\times z)$ |

Les neurones sont organisés en couches. On peut donc vectoriser les calculs, et considérer une couche comme une seule fonction associant à un vecteur d’entrée de taille $n$ un vecteur de sortie de taille $p$. On notera $A^l=\pmatrix{a_1\\\vdots\\a_n}$ l’entrée de la couche numéro $l$, $W^l=\begin{pmatrix} 
			w_{1,1} & \cdots &w_{1,n} \\
			w_{2,1} & \cdots &w_{2,n} \\
			\vdots & \ddots & \vdots \\
			w_{p,1} & \cdots &w_{p,n}
			\end{pmatrix}$ la matrice des poids de tous les neuronnes, $B^l=\pmatrix{b_1\\\vdots\\b_p}$ le vecteur de tous les biais, et $Y^l=W^l\times A^l + B^l$ la sortie de la couche $l$. Pour un réseau disposant de $k$ couche, si l’entrée est $A^1$, on aura $A^2=Y^1$, $A^3=Y^2$, ..., et la sortie du réseau sera $Y^k$.

```

### Neurones et couches

Un réseau de neurone prend en entrée une matrice ou un vecteur $X$, voir plusieurs, représentant les données, et en sortie un ou plusieurs vecteur(s) $Y$. Avec une seule entrée, c’est une fonction de la forme $Y=f(X)$. Elle est généralement constituée d’un succession de couches, représentant des fonctions de la sortie de la couche précédente ie $f=f_1\circ f_2\circ \dots\circ f_l$.

### Entrainement

Pour une entrée $X$, on considère $Y=\pmatrix{y_1\\\vdots\\y_m}$ la sortie donnée par le réseau, et $\hat{Y}=\pmatrix{\hat{y}_1\\\vdots\\\hat{y}_m}$ la sortie attendue. $f$ est le réseau de neuronnes. Dans le cas d’un problème de classification, en général, tous les $\hat{y}_i$ vaudront 0, sauf celui associé au label de l’entrée $X$. On définit d’abord une *fonction de coût* qui mesura la distance entre la sortie attendue et celle fournie par le réseau. Le but est de minimiser la valeur moyenne de cette fonction sur l'ensemble des entrées que l'on possède. On utilise un algorithme de descente de gradient, ou un algorithme dérivé.

#### Descente de gradient stochastique

Pour un paramètre $\lambda$ fixé, pour tout les coefficiens / matrices de coefficiens $W$, on met à jour :
$$
W=W - \lambda \frac{\partial C}{\partial W}
$$

#### Avec moment

$\lambda >0$ et $\beta\in[0;1]$, $M_W$ initialisé à $0$. A chaque mise à jour :
$$
M_W=\beta\ M_W + (\beta-1)\frac{\partial C}{\partial W}\\
W=W - \lambda M_W
$$

#### Algorithme d'Adam

$\lambda >0$ et $\beta_1,\beta_2\in[0;1]$ (souvent $0.9,0.999$), $M_W$ initialisé à $0$. A chaque mise à jour :
$$
g_W=\frac{\partial C}{\partial W}\\
m_W=\beta_1\ m_W + (\beta_1-1)g_W\\
v_W=\beta_2\ v_W + (\beta_2-1)g_W^2\\
\hat{m}_W=\frac{m_W}{\beta_1-1}\ \ \ \hat{v}_W=\frac{v_W}{\beta_2-1}\\
W=W-\lambda\frac{\hat{m}_W}{\sqrt{\hat{v}_W}+\varepsilon}
$$
$m_W$ est une estimation de la moyenne du gradient, $v_W$ de sa variance non centrée, $\hat{m}_W$ et $\hat{v}_W$ les estimation corigées.

### Fonctions de coût

Une première fonction de coût est la fonction $L_2$ :
$$
C=\frac{1}{m}\sum_{i=1}^m (\hat{y}_i-y_i)^2
$$

On défini aussi la fonction de coût log-likelihood, ou vraissemblance logarithmique. Elle est utilisée avec une couche softmax. La sortie attendue doit être de la forme $(0,\dots,0,1,0,\dots0)$, avec $i_0$ notant l’emplacement du $1$.
$$
C=-\log(y^L_{i_0})
$$
En classification, on utilisera souvent la fonction (binary) cross entropy, ou entropie croisée, si la dernière couche n’est pas un softmax.
$$
-\sum_i \hat{y}_i\ \log(y_i)
$$


### Couches de neuronne



#### Couche dense

$X\in M_{n,1}(\mathbb{R})$ désigne l’entrée, $Y\in M_{m,1}(\mathbb{R})$ la sortie. On a de pluse une matrice de paramètres $W\in M_{m,n}$, représentant les poids des arrêtes, et de biais $B\in M_{m,1}(\mathbb{R})$. On cacule la valeur d’activation $Z=W\times X+B$, puis la sortie $Y=\sigma(Z)$, avec $\sigma$ une fonction d’activation. Si on n’intègre pas la couche d’activation, la sortie est simplement $Z$.

#### Couche d'activation

C’est une séparation purement théorique, en réalité elle suit toujours une autre couche : couche dense, ou couche de convolution, en général. Elle calcule $Y=\sigma(X)$, terme par terme.

Les fonctions d’activation peuvent être :

| Sigmoïde                       | Tanh                | ReLU                 | Weak ReLU                           |
| ------------------------------ | ------------------- | -------------------- | ----------------------------------- |
| $\sigma(z)=\frac{1}{1+e^{-z}}$ | $\sigma(z)=tanh(z)$ | $\sigma(z)=max(z,0)$ | $\sigma(z)=max(z,\epsilon\times z)$ |

On peut aussi utiliser la fonction softmax pour représenter une densité de probabilité. La couche sera alors aussi nomée *couche softmax*. Uitilisé en dernière couche, lorsqu’il y a exactement une classe attendue
$$
\sigma(z)=\frac{e^{z}}{\sum_i e^{z_i^L}}
$$

#### Normalisation par batch

Le but est de normaliser les données pour faciliter l’apprentissage. On normalise chaque variable séparément, et on associe à une matrice $X$ la même matrice $Y$, où chaque valeur à été normalisée. Pour une variable $x\in X$, on note $x_1,\dots,x_m$ les différentes valeurs de $x$ pour un batch de données. On calcule ensuite la moyenne $\mu_B=\sum_{i=1}^m x_i$, la variance (carré de la déviation standard) $\sigma^2=\frac{1}{m}\sum_{i=1}^m (x_i-\mu)^2$, puis les valeurs normalisées $\hat{x}_i=\frac{x_i-\mu}{\sqrt{\sigma^2+\varepsilon}}$, et enfin la sortie $y_i=\gamma\hat{x_i}+\beta$ , avec deux paramètres qui sont donc appris.

#### Convolution

Entrée $X\in M_{n,m,c}(\mathbb{R})$, sortie $Y\in M_{n’,m’,c'}(\mathbb{R})$, filtres de convolution : $W\in M_{c’,c,a,b}(\mathbb{R})$.

Avec un pas (stride) de 1, et sans compléter les bords, on a $n’=m-a+1$, $m’=m-a+1$. Si on remplie les bords de 0, $n=n’,\ m=m’$.

#### (Max) pooling

Entrée $X\in M_{n,m,c}(\mathbb{R})$. 

### Différentiation automatique

Différente de la différentiation numérique (approximation $\frac{f(x+h)-f(x)}{h}$), et de la différentiation symbolique qui utilise les formules mathématiques brutes. Elle différentie des « Blocs de code », en utilisant la règle de dérivation en chaine. Implémenté sous forme d’un graphe.

On commence par représenter la fonction à dérivé par un graphe (DAG). Chaque noeud va représenter une fonction $f$, et va contenir le code nécessaire pour calculer, étant donné les valeurs de $x_1,\dots,x_n$, les valeurs de $f(x_1,\dots,x_n)$ et $\frac{\partial f’(x_1,\dots,x_n)}{\partial x_i}$.  Étant donné une entrée, on effectue une passe avant pour calculer les valeur de sorties de chacun des fonctions, puis une passe arrière pour calculer les dérivée par rapport à chacun des paramètres.

Si on note $y=h(f(x_1,\dots,x_n))$ la sortie, et $w=f(x_1,\dots,x_n)$, alors $\frac{\partial y}{\partial x_i}=\frac{\partial h(w)}{\partial w}\frac{\partial w}{\partial x_i}$. Ainsi, si les $h_1,\dots,h_p$ sont les fonctions « filles » du noeud de $f$ dans le graphe en les considérant uniquement en tant que variable de $w$, on aura déjà calculé les $\frac{\partial h_j(w)}{\partial w}$, donc finalement, $\frac{\partial y}{\partial x_i}=\frac{\partial w}{\partial x_i}\times\sum_j \frac{\partial h_j(w)}{\partial w}$.



### Complexité des opérations

-   Couche dense $n\mapsto m$ : $O(nm)$
-   Fonction d’activation, pooling, normalisation : linéaire en la taille de la matrice d'entrée
-   Couche de convolution : $O(cc'nmab)$ 

# Part 2 : Siamese NNet

On cherche à créer une fonction $d : x,y\mapsto\mathbb{R}$ telle que pour $x$ et $y$ des données d’entrée, $d(x,y)$ soit une mesure arbitraire de la distance entre les deux valeurs. En associant à chaque objet $x$ sa classe $c_x$, on désirerait que $d(x,y)\simeq 0$ si $c_x=c_y$, et $d(x,y)> \varepsilon$ sinon. On va utiliser une fonction $f:x\mapsto\mathbb{R}^n$. J’ai arbitrairement choisit d’utiliser un réseau de neuronnes pour $f$. On définit $d(x,y)=\Vert f(x)-f(y)\Vert$ . J’ai choisit la norme euclidienne.

Pour entrainer le réseau de neuronnes, on va ajouter une contrainte : on voudrait $d(x,y)> m$ si $c_x\neq c_y$, avec $m>0$ la marge, qui est une constante fixée. Pour l’entrainement, on utilisera des batch constitués de trois objets à chaque fois : $x_a$, l’ancre, $x_p$ et $x_n$, les objets positifs et négatifs. $x_a$ et $x_p$ ont la même classe, qui est différente de celle de $x_n$. On utilisera alors la fonction de coût : 
$$
C=max(m+d(x_a,x_p)-d(x_a,x_n), 0)
$$
On utilise le max pour limiter le surapprentissage, en éloignant trop des éléments, ce qui est inutile. Cette fonction de coût vise à ce que pour chaque triplet, $d(x_a,x_p)<d(x_a,x_n)-m$, donc que les éléments d’une même classe soient bien plus proches entre eux qu’avec ceux des classes différentes.

Une fois le réseau entrainé, on calcule pour chaque objet son image, et on utilise la méthodes des K plus proches voisins. On n’a pas besoin d’entrainer le réseau à nouveau pour des donnés que l’on a jamais vu, si il a bien été entrainé.

## Idées d’amélioration :

-   Transfert-learning avec une base meilleure pour l’entrainement : par ex, avec mobile net
-   Feature extractor (nécessit de labeler toutes les images)

# Part 3 : KNN efficace

On a un ensemble $A$ de points en dimension $d$. Pour $k>0$ entier, et pour $x\in\mathbb{R}^n$, on cherche les éléments $\{a_1,\dots,a_k\}$ de $A$ minimisant $\Vert a_i-x\Vert$.

## Linéaire

On teste tous les éléments. Pas de temps de pré-calcul, mais chaque requête est en $O(\vert A\vert)$.

## Dichotomie

En dimension $d=1$, une solution est, en précalculant la liste triée des éléments ($O(n\log(n))$), d’utiliser une dichotomie ($O(\log n)$). Ces deux complexités sont optimales si on se limite à des opérations de comparaison entre les éléments.

## Utilisation d’un diagramme de Voronoi

C’est une méthode généralisable en dimension quelconque, mais elle devient plus complexe à implémenter, et sa complexité s’effondre en haute dimension. Cette méthode permet de trouver le point le plus proche d’une cible. On propose ici la version en dimension $d=2$.

On dispose d’un ensemble $A$ de points dans le plan, et on va associer à chaque point une cellule de Voronoi, l’ensemble des cellues de Voronoi découpant le plan en $\vert A\vert$ régions.  Sois $p=(x,y)\in A$. Pour chaque point $p’=(x’,y’)\in A, (x’,y’)\neq A$, on considère le demi-plan des points plus proche de $p$ que de $p’$. On définit la cellue de Voronoi comme l’intersection de tous ces hyperplans, qui sera l’ensemble des points plus proche de $p$ que de tout autre point. On défini les voisins d’un point $p$ dans le diagramme de Voronoi comme l’ensemble des points dont la cellule possède un coté commun avec celle du point $p$. 

Ainsi, si on cherche un point cible, il se trouvera dans au moins cellule de Voronoi, et il suffira de retourner le point associé à la cellule, qui en sera au moins aussi proche que tout autre point. On remarque que si on choisit une cellule quelconque, si le point $p$ associé n’est pas le plus proche de la cible, alors un des points voisins (selon la définition ci-dessus) est plus proche de la cible. En effet, la cible n’appartient pas à la cellule de $p$. On considère le diagramme de Voronoi formé uniquement par $p$ et ses voisins. La cellule de $p$ reste inchangée, car elle est construite uniquement comme l’intersections des hyperplans séparant $p$ de ses voisins. Donc la cible appartient à la cellue d’un voisin dans le nouveau diagramme, donc l’un de ces points est plus proches de la cible que $p$.

On construit alors le graphe induit par le diagramme de Voronoi, en assoçiant à chaque point un sommet, qui sera relié aux sommets associés aux voisins de ce point. Le graphe engendré est planaire (PREUVE !), et dans les cas non dégénérés (LESQUELS ?), c’est une triangulation du plan (PREUVE). Dans le cas contraire, on peut le compléter en une triangulation du plan si les points ne sont pas alignés (s’ils le sont, il n’existe pas de triangulation, mais le graphe vérifie les mêmes propriétés). On peux montrer que cette triangulation est une triangulation de Delaunay, ce qui explique que l’on parle souvent de ce graphe.

Selon ce qui a été dit précédement, on déduit que si on calcule la triangulation de delauney de $A$, alors l’algorithme glouton consistant à toujours s’approcher de la cible en cherchant un meilleur point parmis les voisins est optimal. Il fini, car la distance décroit toujours strictement. Sa complexité est cependant en $O(n)$ dans le pire cas.

### Algorithme de construction du diagramme

TODO : Algo + complexité + preuve

## KD-tree

On construit le KD-tree, qui est un arbre binaire, récursivement en coupant selon un hyperplan. La méthode utilisée ici est : on coupe alternativement selon les différentes dimensions. On trie selon la dimension choisie, on prend le point médian (pour assurer la profondeur de $\log(n)$). Ce point formera un noeud de l’arbre. Les fils gauche et droite seront les abres construits respectivements pour les points plus petits.

Pour rechercher le point le plus proche d'un point « cible »  dans un arbre, on recherche récursivement. Lorsque l’on est à un noeud donné, on met à jour la distance minimale si le point cible est plus proche de celui stocké dans le noeud que ceux que l’on a vu ensuite. Puis on poursuit la recherche récursivement dans le sous-arbre correspondant au demi-plan contenant le point. Ensuite, on regarde la distance minimale trouvée. Si l’hypersphère ayant ce rayon est située du même coté strictement du plan, on retourne le point correspondant. Sinon, on cherche récursivement dans l’autre abre.

Complexité de la construction : $O(n\log^2(n))$. Complexité de la recherche : aléatoire, $O(\log(n))$ dans le meilleur cas, $O(n)$ dans le pire cas. Dans le cas $d=1$, on sera en $O(\log(n))$, cette méthode sera presque optimale en construction, et optimale en recherche, car elle fera la même chose que la dichotomie.\



Une tentative d’optimisation à été de stocker dans chaque noeud les coordonnés de la zone contenant les points de chaque coté, pour pouvoir vérifier si le point peut être situé dedans avant de rechercher récursivement. Cela rajoute cependant un facteur $d$ à la recherche. Après tests, cela peut réduire par deux le nombre de sommets explorés en dimensions 32, ce qui ralenti donc 16 fois le programme.

Une autre optimisation, plus simple, était de marquer dans les noeuds les coordonnées du point à droite et à gauche, pour ne pas comparer avec l’hyperplan, mais avec ces points. Ne change pas la complexité. Lors des tests, explore un peu moins de noeuds, mais un peu plus lent.

## Graphe de proximité

Cette solution fonctionne en dimension quelconque, sans grande perte de rapidité. Elle est hautement parallélisable, mais approximative. Elle supporte l’ajout rapide d’éléments, ce qui est un des objectifs de ce TIPE.

On stocke les points sous forme d’un graphe. On désire deux types d’arrêtes : premièrement, pour chaque point, qu’il soit relié aux points les plus proches l’environant. Ces arrêtes doivent « approximer » la triangulation de Delauney, qui garantie de trouver le point optimal. Deuxièmement, on désire des arrêtes « longue portée » qui permettent de traverser le graphe rapidement. On construit le graphe en ajoutant successivement les points.

### Recherche dans le graphe

Pour recher dans le graphe, un algorithme heuristique consiste à se placer à une entrée choisie aléatoirement (ou fixée), et à se rapprocher de la cible tant que l’un des voisins est plus proche.

On peut rechercher les k plus proches avec un algorithme équivalent. On garde le groupe des $k$ plus proches, initialement constitué d’un seul élément. A chaque fois que l’on rencontre un nouvel élément plus proche que ceux que l’on a dans le groupe, on l’insère dedans, et on sort l’élément le plus loin de la cible si on a dépassé $k$ éléments. Puis on va regarder tous ses voisins pour tenter de trouver des points plus proches, et ainsi de suite tant que l’on trouve des points plus proches.

On peut optimiser les résultats avec quelques tricks. D’abord, on recherchera plus de points que voulu pour  éviter les minimuls locaux, y compris pour rechercher un seul point. Ensuite, on fera plusieurs essais à partir de points d’entrée aléatoires. Cela améliore considérablement le succès, en dégradant un peu les performances.



Une tentative d’optimisation a été de construire un KD-tree et effectuer une recherche simple en $\log(n)$ pour trouver un point assez peu éloigné avant de lancer l’algorithme. Cela n’a cependant pas été concluant, et briserait certaines propriétés recherchées.

### Insertion dans un graphe

Un premier algorithme heuristique est de chercher les $k$ plus proches, où $k$ sera le nombre de voisins, et de relier le nouveau noeud à chacun de ces points. Les long-range links seront créés lors des premières insertions, car les noeuds seront encore †rès éloignés.