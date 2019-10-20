https://ml-cheatsheet.readthedocs.io

# Comment reconnaitre a un emplacement / découper ? Obtenir d'autres types de résultats ?
Comment apprendre a jouer à un jeu ?
Comment voir en couleur ?

Unsupervised et reinforcment learning for NN

Cross validation & choisir paramètres auto



-   Single number evaluation metric [recal + accuracy -> harmonic average ; average ; implement in lib]
-   Transfer learning [train all / only new layers / some layers]
-   Multi-task learning
-   Multiple input channels ; input shape = (n1, n2, n3, ...) Conv layer use last dims
-   Pool : max, average, etc...

# Proximimité des points

- proximity neighborhood graphs
	- Gabriel graph : https://en.wikipedia.org/wiki/Gabriel_graph
	- https://arxiv.org/pdf/1603.09320.pdf <-----------------------------
- R-tree : https://en.wikipedia.org/wiki/R-tree (voir `R*-tree`)


# Techniques d'IA

http://neuralnetworksanddeeplearning.com/chap6.html
https://www.coursera.org/learn/machine-learning#syllabus
https://machinelearningmastery.com/deep-learning-with-python/ : conv + rec with python
http://deeplearning.cs.cmu.edu/ -> Assez poussé
https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/DeepLearning-NowPublishing-Vol7-SIG-039.pdf -> specifique par sujet
http://deeplearning.net/tutorial/deeplearning.pdf -> RNN, DBN, etc...

recurrent neural networks
Boltzmann machines
generative models
transfer learning
reinforcement learning
Deep belief networks
LSTM
Hyperparameters tunning
Sequence models
Sliding windows ? Other algorithms ?
Suivre dans une vidéo


TODO:
- print to a file
- Momentum option
- Optimize & multiprocess ?
- Outils de distortion / generation de datas
- Séparer pour avoir NN + autres algos (traitement image, etc...)
- Hyperparameters than can be changed (learning rate, decay, dropout), et d'autres pas
- Creer rapidement plusieurs layers avec une liste au lieu des parametres, passer des listes, nommer layers ; tout in-liner automatiquement
- Sequential ; layers pour activations ET neuronnes ; add, etc... ; build / compile [option for lightweight (no training) + initial_datas / load from] ; multiple inputs & outputs
- Transfer learning, because dataset is small

TRICKS:
- normaliser
- toujours vecteurs 2 dims (même si (1, n))


### Fonctions de coût

- L2
- Cross entropy
- Cross entropy for softmax

### Regularization

- L2
- L1
- Dropout
- Artificially expanding the training data
- Early stopping

### initialisation

- Gaussian init with standard `deviation = 1/(n[input]^0.5)` (2 pour ReLU, other possible for tanh)

### Autres

- Learning rate schedule / decay
- early stoping
- Multiples networks
- Tunning hyperparameters with random
- Normalize activations (output de layers servant d'entrée au prochain) [https://arxiv.org/abs/1502.03167]

- Normalize inputs : mean and variance (même décalage sur test et input : dans input layer)
- Random mini-batchs

### Gradient descent alternative

- momentum descent
- RMSprop
- Adam (momentum + RMSprop)
- hessian
- Conjugate gradient
- BFGS
- L-BFGS

### Neuron models

- **sigmoid**
- **tanh**: tanh(w⋅x+b)  [almost alwais superior to sigmoid]]]
- Softmax (good for output)
- ***rectified linear unit*** (ReLU): max(0,w⋅x+b) [efficient and quick to train]
- **weak ReLU** : max(0.01 * z, z)


# Papiers reherche

- Best practices for convolutional neural networks applied to visual document analysis: https://ieeexplore.ieee.org/document/1227801
- Improving neural networks by preventing co-adaptation:https://arxiv.org/pdf/1207.0580.pdf
- Practical Recommendations: https://arxiv.org/pdf/1206.5533v2.pdf (p14-15 for init help)
- benefits of variable learning rates : https://arxiv.org/abs/1003.0358
- Optimiser automatiquement les hyper-paramètres: http://papers.nips.cc/paper/4522-practical-bayesian-optimization-of-machine-learning-algorithms.pdf
- Backpropagation techniques : http://yann.lecun.com/exdb/publis/pdf/lecun-98b.pdf
- Classification d'images : https://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks.pdf
- Theano-based Large-Scale Visual Recognition with Multiple GPUs : https://arxiv.org/abs/1412.2302


- On the number of response regions of deep feedforward networks: https://arxiv.org/pdf/1312.6098.pdf

# Tutoriels

Tout : https://github.com/ChristosChristofidis/awesome-deep-learning

- http://neuralnetworksanddeeplearning.com
- https://cs.stanford.edu/~quocle/tutorial1.pdf

# Autres ressources

- Competition & datasets: https://www.kaggle.com/
- ML cheatset: https://ml-cheatsheet.readthedocs.io/en/latest/index.html
- Caffe model zoo : http://caffe.berkeleyvision.org/model_zoo.html

# Autres techniques de ML

# Data Sets

- MNIST: http://yann.lecun.com/exdb/mnist/
- Dogs VS cats: https://www.kaggle.com/c/dogs-vs-cats

## Fishes & ocean animals

- Lot of images / videos : http://groups.inf.ed.ac.uk/f4k/F4KDATASAMPLES/INTERFACE/DATASAMPLES/search.php
- Detect if this is fish ?? https://swfscdata.nmfs.noaa.gov/labeled-fishes-in-the-wild/
- Fish Recognition Ground-Truth data (>10 000): http://groups.inf.ed.ac.uk/f4k/GROUNDTRUTH/RECOG/
- Fish Dataset (3,960): https://wiki.qut.edu.au/display/cyphy/Fish+Dataset
- http://groups.inf.ed.ac.uk/f4k/F4KDATASAMPLES/INTERFACE/DATASAMPLES/search.php
- Protected dataset: https://www.st.nmfs.noaa.gov/aiasi/DataSets.html
- Image-net: http://www.image-net.org/search?q=fish  and  http://www.image-net.org/synset?wnid=n02512053
- Fish Database of Taiwan : https://www.gbif.org/dataset/b7dd8170-98a0-11de-b4d9-b8a03c50a862#description
- Kaggle competition : https://www.kaggle.com/c/the-nature-conservancy-fisheries-monitoring/data
- Open-images: https://storage.googleapis.com/openimages/web/visualizer/index.html?set=train&c=%2Fm%2F0ch_cf

- Alvin : https://www.whoi.edu/page.do?pid=11038&c=2&cid=25586&tid=282
- Fishbase : https://www.fishbase.de/search.php
- Maris : http://rimlab.ce.unipr.it/Maris.html

- CIFAR 100 : https://www.cs.toronto.edu/~kriz/cifar.html (10 classes pour l'eau, autre classes, a tester !)