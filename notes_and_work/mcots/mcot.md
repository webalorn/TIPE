# Reconnaissance efficace d’objets sous-marins

### Motivation

M’intéressant au diverses formes d’apprentissage automatisés, j’ai vu la place important que cette branche de l’informatique pouvait avoir dans la création de sous-marins autonomes. J’ai donc décidé d’explorer un moyen de se rapprocher de la vision humaine à l’aide de l’informatique.

### En quoi votre étude s'inscrit-elle dans le thème de l'année ?

Un apport important de l’informatique aux activités humaines liées à l’océan peut être l’automatisation de taches, dans ce milieu inhospitalier. L’un de ces aspect est la reconnaissance des objets environnant un système équipé d’une caméra à l’aide d'images, ce qui est le sujet de mon TIPE.

### Positionnement thématique

Informatique théorique, Informatique pratique

### Mots-clés

| Français                                                     | Anglais                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Reconnaissance d'image<br />Apprentissage automatique<br />Réseaux de neurones siamois<br />Plus proches voisins<br />Efficacité temporelle | Image recognition<br />Machine learning<br />Siamese networks<br />Nearest neighbour<br />Time efficiency |

### Bibliographie commentée

La reconnaissance d’image est un domaine de l’informatique qui a pour but de permettre à une machine de comprendre le monde physique, au moyen de données comportant des imprécisions et de grandes variations. Ainsi ce domaine est très fortement lié à l’apprentissage automatique, et a largement profité des améliorations des réseaux de neurones artificiels. L’avancée la plus importante à été l’introduction de couches de convolution [1]. Cette technique est systématiquement retrouvée dans les avancées modernes en reconnaissance d'image, et certaines architectures l’emploient intensivement, comme MobileNet [2].

Les réseaux de neurones sont souvent utilisés comme classificateurs, mais cela pose le problème d’un nombre fixé de classes, et d’un long apprentissage préalable. Ainsi on peut utiliser des réseaux de neurones pour extraire des informations des images, que ce soit des informations prédéterminée, où des information que le réseau apprend. Cette dernière possibilité est permise notamment par les réseaux siamois. La fonction qu’ils représentent peut être entrainée et utilisée pour mesurer la similarité entre deux images [3].

Lorsque l’on dispose d’une fonction de distance entre des objets à comparer, une approche intéressante est celle des k-plus proches voisins. Cependant, les solution exactes dont on dispose, outre leur complexité, ne sont rapides qu’en basse dimensions, et deviennent très lente lorsque celle-ci augmente. Ainsi, des structures de données adaptées au stockage de tels objets ont été mises au point : on peut citer les arbres kd [4]. Cependant, elles souffrent aussi d’une perte trop importante de rapidité en grande dimension, ce qui pousse à la réalisation d’algorithmes probabilistes. Les graphes de voisinages [5] sont un champ de recherche de tels algorithmes, où l’on cherche généralement à approximer des graphes proches de la triangulation de Delaunay, tout en contenant des arcs assurant la vitesse de la recherche.

### Problématique retenue

Faces aux limites des différentes approches utilisées pour la reconnaissance d’image, on constate qu’il est difficile d’obtenir une méthode de classification disposant des capacités que l’on retrouve chez les organismes vivants. Il s’agit donc de trouver un compromis entre les avantages de ces algorithmes pour proposer une approche plus souple.

### Objectifs du TIPE

Les réseaux de neurones artificiels constituent le domaine de l’apprentissage automatique le plus efficace actuellement, disposant d’une grande souplesse dans leurs architectures et dans leur applications. Cependant leur utilisation classique directement en tant que classificateurs pose problème quant au temps demandé pour apprendre de nouvelles données, reconnaitre des objets lorsque peu de données sont disponibles, ou détecter des objets inconnus.

Ainsi, après avoir constaté les limites de deux méthodes classiques que sont les plus proches voisins et les réseaux de neurones artificiels, je montrerait comment ces derniers peuvent êtres utilisés pour extraire des information ayant un sens pour la distance euclidienne des plus proches voisins.

Je montrerai ensuite comment cette méthode peut être appliquée de façon efficace en grande dimension pour pouvoir, avec un réseau de neurone, former une structure de donnée capable de répondre rapidement aux requêtes de détection et d’apprentissage de données, que ce soit sur un système distribué ou embarqué.

### Objectifs du TIPE, en 100 mot (limite SCEI...)

Après avoir constaté les limites de deux méthodes classiques que sont les plus proches voisins et les réseaux de neurones artificiels en tant que simples classifieurs, je montrerai comment ces derniers peuvent êtres utilisés pour extraire des informations ayant un sens pour la distance euclidienne des plus proches voisins.

Je montrerai ensuite comment cette méthode peut être appliquée de façon efficace en grande dimension pour pouvoir, avec un réseau de neurone, former une structure de donnée capable de répondre rapidement aux requêtes de détection et d’apprentissage de données, que ce soit sur un système distribué ou embarqué.

### Références bibliographiques

**[1]** Yann LeCun, Patrick Haffner, Léon Bottou, Yoshua Bengio : « Object recognition with Gradient-Based Learning » http://yann.lecun.com/exdb/publis/pdf/lecun-99.pdf | Type de réseau que j’utilise comme base de la plupart de mes expérimentations  
**[2]** Mark Sandler, Andrew Howard, Menglong Zhu, Andrey Zhmoginov, Liang-Chieh Chen : « MobileNetV2: Inverted Residuals and Linear Bottlenecks » https://arxiv.org/abs/1801.04381  
**[3]** Jane Bromley, Isabelle Guyon, Yann LeCun, Eduard Sickinger and Roopak Shah : « Signature Verification using a "Siamese" Time Delay Neural Network » http://papers.nips.cc/paper/769-signature-verification-using-a-siamese-time-delay-neural-network.pdf  
**[4]** Jon Louis Bentley (Stanford University) : « Multidimensional Binary Search Trees Used for Associative Searching » : http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.160.335&rep=rep1&type=pdf | KD-tree  
**[5]** Yury Malkov, Alexander Ponomarenko, Andrey Logvinov, Vladimir Krylov : « Approximate nearest neighbor algorithm based on navigable small world graphs » https://publications.hse.ru/mirror/pubs/share/folder/x5p6h7thif/direct/128296059 | Graphes de proximité  
**[6]** Diederik P. Kingma, Jimmy Lei Ba : « Adam: A method for stochastic optimization » https://arxiv.org/pdf/1412.6980v8.pdf | Algorithme d’Adam que j’utilise beaucoup à la place d’une simple descente de gradient

__