# TODO

Ce qu’il reste RÉELLEMENT A FAIRE de manière pratique (en terme de codes / runs)

- complexité pour tout (et run du NNET pour une image)
- Etudier convergence ANN simple
- KNN, les coder :
	-   Delauney
	    -   2D (essayer plusieurs méthodes)
	      -   Sweeping line + inv
	    -   ND (En brute-force ? Avec enveloppe convexe ?)
	    -   ajouter des long-range links ?
	-   efficient KNN
	  - Multi-layer
	  - idée optim : 1 hub par cls -> aprox delaunay des hubs, delaunay des cls, delaunay de tout, merge.
	  - Papier INRIA : cellules ; toutes les directions
      - Projecter des rayons et inverse du produit scalaire
	  - Améliorer le réseau **après**
	  - Long range links (random ? d-harmonic distrib ? ($\frac{1}{l^d}$))
	  - Nombre de voisins : $2d+1$ -> $3d+1$
	  - “hubs” ? Entrypoints ?
- https://en.wikipedia.org/wiki/Nearest_neighbor_search#Greedy_search_in_proximity_neighborhood_graphs

-   KNN : donner des classes et évaluer la CLASSE trouvée