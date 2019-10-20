# Architecture

https://github.com/vitruvianscience/OpenDeep

- Fonctions pour apprendre les paramètres / trouver les bias, etc...


TODO

- outSameAs
- same outputs dans les fonctions "run" -> ignorer ces output

- variance / moyenne dans BatchNorm as a matrix ??
- weakReLU 0.1 ? (tester) [multiple weaks]

- Allow inputs larger than the network FOR SOME LAYERS ONLY (not dense)

- architecture : multi-inputs, multi-outputs (nom pour les entrées), outputs with the same datas
- Cost functions per layer ?

- learning rate decade
- theano shared variables for learning rate, etc... that can be changed during training
- batch norm without biases ?

- autres algos (Adam, Adagrad, Nesterov momentum, Nadam, etc...)
- SumLayer, mergeLayer function, redimensionner (FCL, fixed matrix...)
- ResNets (Skip connexions)

- Monitor across multiple networks


- Data augmentation, rotations, etc... (https://www.coursera.org/learn/convolutional-neural-networks/lecture/AYzbX/data-augmentation)
	-> random cropping
	-> color shifting
	-> gaussian blur


- AUTO SAVE BROKEN ?
- Why padding same is so slow ?

- Flow for testing




- Layers shortcuts (ConvLayer, FCL layer, with activation and dropout)
- Pre-builts ? (SimpleClassifier, LinearMultiClassifier, etc...)
- Save preprocessor datas with network ?

- Inception networks
- Intermediate outputs for training ONLY (cf outSameAs) (regularization effet) (monitors configurables)
- Transfert learning
	- Freeze parameters
	- Learn another problem BEFORE