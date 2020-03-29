[toc]

Une des contraintes lors de la mise au point de véhicules sous-marins autonomes est de lui apprendre à reconnaitre son environement. Je me suis ainsi concentré sur l’aspect de la classification des objets repérés. Étant donné la large variété d’espèces et d’objets sous-marins, j’ai cherché à mettre au point une méthode permettant à la fois de classifier les objets déjà connus, et repérer de nouvelles catégories en gardant des performances acceptables.

Pour cela, après avoir constaté les limites de techniques simples telles les plus proches voisins où les réseau de neuronnes, j’ai essayé d’utiliser ceux-ci afin de fournir une fonction de distance plus efficace pour effectuer la méthode des k plus proches voisins. J’ai ensuite utilisé des structures de donnés plus efficaces pour trouver les voisins dans un espace de grande dimension.

# 1. Méthodes simples de classification

