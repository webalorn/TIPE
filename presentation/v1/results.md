# Colors density

## 3 species

```
Colors: [[0.47785795 0.6165395  0.6006762 ]
 [0.33543053 0.5242495  0.46693128]
 [0.41172656 0.46226212 0.46689957]]

===== DataSet test

Test accuracy: 67.50% (405 over 600 examples) 

First fish(C1) => nb rightly classified : 137 / 200 | nb false positive : 97
Black fish(C2) => nb rightly classified : 118 / 200 | nb false positive : 34
Clown fish(C3) => nb rightly classified : 150 / 200 | nb false positive : 64

First fish(C1) -> 137[First fish(C1)] 49[Black fish(C2)] 48[Clown fish(C3)] 
Black fish(C2) -> 32[First fish(C1)] 118[Black fish(C2)] 2[Clown fish(C3)] 
Clown fish(C3) -> 31[First fish(C1)] 33[Black fish(C2)] 150[Clown fish(C3)] 
```

## 6 specides

```
Colors: [[0.47785795 0.6165395  0.6006762 ]
 [0.33543053 0.5242495  0.46693128]
 [0.41172656 0.46226212 0.46689957]
 [0.302485   0.55521464 0.48293608]
 [0.34717366 0.58538115 0.50991786]
 [0.35145992 0.35783157 0.35086522]]

===== DataSet test

Test accuracy: 39.73% (298 over 750 examples) 

First fish(C1) => nb rightly classified : 133 / 200 | nb false positive : 110
Black fish(C2) => nb rightly classified : 32 / 200 | nb false positive : 20
Clown fish(C3) => nb rightly classified : 54 / 200 | nb false positive : 45
C4 => nb rightly classified : 25 / 50 | nb false positive : 43
C5 => nb rightly classified : 14 / 50 | nb false positive : 47
C6 => nb rightly classified : 40 / 50 | nb false positive : 187

First fish(C1) -> 133[First fish(C1)] 36[Black fish(C2)] 46[Clown fish(C3)] 10[C4] 17[C5] 1[C6] 
Black fish(C2) -> 11[First fish(C1)] 32[Black fish(C2)] 1[Clown fish(C3)] 5[C4] 3[C5] 0[C6] 
Clown fish(C3) -> 25[First fish(C1)] 9[Black fish(C2)] 54[Clown fish(C3)] 1[C4] 1[C5] 9[C6] 
C4 -> 7[First fish(C1)] 30[Black fish(C2)] 1[Clown fish(C3)] 25[C4] 5[C5] 0[C6] 
C5 -> 11[First fish(C1)] 31[Black fish(C2)] 2[Clown fish(C3)] 3[C4] 14[C5] 0[C6] 
C6 -> 13[First fish(C1)] 62[Black fish(C2)] 96[Clown fish(C3)] 6[C4] 10[C5] 40[C6] 
```

# K-nearest neighbors

Manhatan distance, pondéré par 1/d

## 3 species

```
Predict datas with K = 5

===== DataSet test

Test accuracy: 91.67% (550 over 600 examples) 

First fish(C1) => nb rightly classified : 162 / 200 | nb false positive : 11
Black fish(C2) => nb rightly classified : 194 / 200 | nb false positive : 28
Clown fish(C3) => nb rightly classified : 194 / 200 | nb false positive : 11

First fish(C1) -> 162[First fish(C1)] 6[Black fish(C2)] 5[Clown fish(C3)] 
Black fish(C2) -> 27[First fish(C1)] 194[Black fish(C2)] 1[Clown fish(C3)] 
Clown fish(C3) -> 11[First fish(C1)] 0[Black fish(C2)] 194[Clown fish(C3)]
```

## 6 species

```
Predict datas with K = 5

===== DataSet test

Test accuracy: 87.87% (659 over 750 examples) 

First fish(C1) => nb rightly classified : 163 / 200 | nb false positive : 21
Black fish(C2) => nb rightly classified : 190 / 200 | nb false positive : 50
Clown fish(C3) => nb rightly classified : 193 / 200 | nb false positive : 13
C4 => nb rightly classified : 33 / 50 | nb false positive : 6
C5 => nb rightly classified : 32 / 50 | nb false positive : 1
C6 => nb rightly classified : 48 / 50 | nb false positive : 0

First fish(C1) -> 163[First fish(C1)] 6[Black fish(C2)] 6[Clown fish(C3)] 7[C4] 2[C5] 0[C6] 
Black fish(C2) -> 26[First fish(C1)] 190[Black fish(C2)] 1[Clown fish(C3)] 8[C4] 15[C5] 0[C6] 
Clown fish(C3) -> 11[First fish(C1)] 0[Black fish(C2)] 193[Clown fish(C3)] 1[C4] 0[C5] 1[C6] 
C4 -> 0[First fish(C1)] 4[Black fish(C2)] 0[Clown fish(C3)] 33[C4] 1[C5] 1[C6] 
C5 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 1[C4] 32[C5] 0[C6] 
C6 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 48[C6] 
```

## After NN, 6 species

- 97.73% -> [NO 98.89%]
- 2 layers removed

```

```

```
===== DataSet validation

Test accuracy: 98.89% (356 over 360 examples) 

First fish(C1) => nb rightly classified : 99 / 100 | nb false positive : 2
Black fish(C2) => nb rightly classified : 98 / 100 | nb false positive : 1
Clown fish(C3) => nb rightly classified : 100 / 100 | nb false positive : 0
C4 => nb rightly classified : 19 / 20 | nb false positive : 0
C5 => nb rightly classified : 20 / 20 | nb false positive : 1
C6 => nb rightly classified : 20 / 20 | nb false positive : 0

First fish(C1) -> 99[First fish(C1)] 2[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 0[C6] 
Black fish(C2) -> 1[First fish(C1)] 98[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 0[C6] 
Clown fish(C3) -> 0[First fish(C1)] 0[Black fish(C2)] 100[Clown fish(C3)] 0[C4] 0[C5] 0[C6] 
C4 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 19[C4] 0[C5] 0[C6] 
C5 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 1[C4] 20[C5] 0[C6] 
C6 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 20[C6] 
```

# Fish species classification : Neural network

## 3 species : fish_01, fish_03, fish_04

200 test, 100 validation, max 5000 train

### Fully-connected network
- 60 epochs, mini-batch = 40
- GradienAlgo(0.02)
- D(800) -> tanh -> D(300) -> tanh -> D(100) -> tanh -> D(3) -> tanh 

```
Test accuracy: 96.83%

First fish => nb rightly classified : 189 / 200 | nb false positive : 8
Black fish => nb rightly classified : 193 / 200 | nb false positive : 8
Clown fish => nb rightly classified : 199 / 200 | nb false positive : 3

First fish -> 189[First fish] 7[Black fish] 1[Clown fish] 
Black fish -> 8[First fish] 193[Black fish] 0[Clown fish] 
Clown fish -> 3[First fish] 0[Black fish] 199[Clown fish]
```

### Fully-connected + softmax et Momentum

- 60 epochs, mini-batch = 40
- MomentumGradient(0.02)
- D(800) -> tanh -> D(300) -> tanh -> D(100) -> tanh -> D(3) -> sigmoid 

```
Test accuracy: 97.50%

First fish => nb rightly classified : 199 / 200 | nb false positive : 14
Black fish => nb rightly classified : 191 / 200 | nb false positive : 1
Clown fish => nb rightly classified : 195 / 200 | nb false positive : 0

First fish -> 199[First fish] 9[Black fish] 5[Clown fish] 
Black fish -> 1[First fish] 191[Black fish] 0[Clown fish] 
Clown fish -> 0[First fish] 0[Black fish] 195[Clown fish]
```

### Batch Norm

99.33%

### Conv layers

- 60 epochs, mini-batch = 40
- MomentumGradient(0.06)

```
Articial neural network : Sequential
- Layer 0 : InputLayer           (3, 50, 50)                 
- Layer 1 : BatchNorm            (3, 50, 50)   [1 inputs: 0] 
- Layer 2 : Convolution2D        (20, 46, 46)  [1 inputs: 1] 
- Layer 3 : Pool2D               (20, 23, 23)  [1 inputs: 2] 
- Layer 4 : Activation tanh      (20, 23, 23)  [1 inputs: 3] 
- Layer 5 : BatchNorm            (20, 23, 23)  [1 inputs: 4] 
- Layer 6 : Convolution2D        (20, 19, 19)  [1 inputs: 5] 
- Layer 7 : Pool2D               (20, 9, 9)    [1 inputs: 6] 
- Layer 8 : Activation tanh      (20, 9, 9)    [1 inputs: 7] 
- Layer 9 : BatchNorm            (20, 9, 9)    [1 inputs: 8] 
- Layer 10 : Dense               (800,)        [1 inputs: 9] 
- Layer 11 : Activation tanh     (800,)        [1 inputs: 10]
- Layer 12 : BatchNorm           (800,)        [1 inputs: 11]
- Layer 13 : Dense               (100,)        [1 inputs: 12]
- Layer 14 : Activation tanh     (100,)        [1 inputs: 13]
- Layer 15 : Dense               (3,)          [1 inputs: 14]
- Layer 16 : Activation softmax  (3,)          [1 inputs: 15]
```

```
Test accuracy: 99.50% (597 over 600 examples) 

First fish => nb rightly classified : 199 / 200 | nb false positive : 2
Black fish => nb rightly classified : 198 / 200 | nb false positive : 0
Clown fish => nb rightly classified : 200 / 200 | nb false positive : 1

First fish -> 199[First fish] 2[Black fish] 0[Clown fish] 
Black fish -> 0[First fish] 198[Black fish] 0[Clown fish] 
Clown fish -> 1[First fish] 0[Black fish] 200[Clown fish]
```

## 6 species : fish_01, fish_03, fish_04 + fish_08, fish_09, fish_10

Nouveaux : 50 test, 20 validation

(avant : 98.50%)

### Fully-connected

```
Articial neural network : Sequential
- Layer 0 : InputLayer           (3, 50, 50)                
- Layer 1 : BatchNorm            (3, 50, 50)  [1 inputs: 0] 
- Layer 2 : Dense                (800,)       [1 inputs: 1] 
- Layer 3 : Activation tanh      (800,)       [1 inputs: 2] 
- Layer 4 : BatchNorm            (800,)       [1 inputs: 3] 
- Layer 5 : Dense                (300,)       [1 inputs: 4] 
- Layer 6 : Activation tanh      (300,)       [1 inputs: 5] 
- Layer 7 : BatchNorm            (300,)       [1 inputs: 6] 
- Layer 8 : Dense                (100,)       [1 inputs: 7] 
- Layer 9 : Activation tanh      (100,)       [1 inputs: 8] 
- Layer 10 : Dense               (6,)         [1 inputs: 9] 
- Layer 11 : Activation softmax  (6,)         [1 inputs: 10]
=> Network built!
Read datas...
Start training
Start building training function...

Training with 11590 in mini-batches of size 40 for 60 epochs
-> optimizer (algorithm) : MomentumGradient
-> learning rate : 0.04
-> regularization : 0.0
```

```
===== DataSet test

Test accuracy: 95.20% (714 over 750 examples) 

First fish(C1) => nb rightly classified : 191 / 200 | nb false positive : 17
Black fish(C2) => nb rightly classified : 197 / 200 | nb false positive : 9
Clown fish(C3) => nb rightly classified : 200 / 200 | nb false positive : 5
C4 => nb rightly classified : 33 / 50 | nb false positive : 2
C5 => nb rightly classified : 45 / 50 | nb false positive : 2
C6 => nb rightly classified : 48 / 50 | nb false positive : 1

First fish(C1) -> 191[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 13[C4] 2[C5] 1[C6] 
Black fish(C2) -> 5[First fish(C1)] 197[Black fish(C2)] 0[Clown fish(C3)] 3[C4] 1[C5] 0[C6] 
Clown fish(C3) -> 2[First fish(C1)] 0[Black fish(C2)] 200[Clown fish(C3)] 0[C4] 2[C5] 1[C6] 
C4 -> 1[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 33[C4] 0[C5] 0[C6] 
C5 -> 0[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 1[C4] 45[C5] 0[C6] 
C6 -> 1[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 48[C6] 
```

### ConvNet, data augmentation, in grayscale

```
- Layer 0 : InputLayer           (50, 50)                    
- Layer 1 : BatchNorm            (50, 50)      [1 inputs: 0] 
- Layer 2 : Convolution2D        (20, 46, 46)  [1 inputs: 1] 
- Layer 3 : Pool2D               (20, 23, 23)  [1 inputs: 2] 
- Layer 4 : Activation tanh      (20, 23, 23)  [1 inputs: 3] 
- Layer 5 : BatchNorm            (20, 23, 23)  [1 inputs: 4] 
- Layer 6 : Convolution2D        (40, 19, 19)  [1 inputs: 5] 
- Layer 7 : Pool2D               (40, 9, 9)    [1 inputs: 6] 
- Layer 8 : Activation tanh      (40, 9, 9)    [1 inputs: 7] 
- Layer 9 : BatchNorm            (40, 9, 9)    [1 inputs: 8] 
- Layer 10 : Convolution2D       (80, 5, 5)    [1 inputs: 9] 
- Layer 11 : Pool2D              (80, 2, 2)    [1 inputs: 10]
- Layer 12 : Activation tanh     (80, 2, 2)    [1 inputs: 11]
- Layer 13 : BatchNorm           (80, 2, 2)    [1 inputs: 12]
- Layer 14 : Dense               (1000,)       [1 inputs: 13]
- Layer 15 : Activation tanh     (1000,)       [1 inputs: 14]
- Layer 16 : BatchNorm           (1000,)       [1 inputs: 15]
- Layer 17 : Dense               (400,)        [1 inputs: 16]
- Layer 18 : Activation tanh     (400,)        [1 inputs: 17]
- Layer 19 : BatchNorm           (400,)        [1 inputs: 18]
- Layer 20 : Dense               (200,)        [1 inputs: 19]
- Layer 21 : Activation tanh     (200,)        [1 inputs: 20]
- Layer 22 : Dense               (6,)          [1 inputs: 21]
- Layer 23 : Activation softmax  (6,)          [1 inputs: 22]
=> Network built!
Read datas...
Start training
Start building training function...

Training with 13782 in mini-batches of size 40 for 60 epochs
-> optimizer (algorithm) : MomentumGradient
-> learning rate : 0.04
-> regularization : 0.0
```

```
===== DataSet test

Test accuracy: 97.20% (729 over 750 examples) 

First fish(C1) => nb rightly classified : 197 / 200 | nb false positive : 12
Black fish(C2) => nb rightly classified : 197 / 200 | nb false positive : 2
Clown fish(C3) => nb rightly classified : 199 / 200 | nb false positive : 1
C4 => nb rightly classified : 39 / 50 | nb false positive : 1
C5 => nb rightly classified : 48 / 50 | nb false positive : 4
C6 => nb rightly classified : 49 / 50 | nb false positive : 1

First fish(C1) -> 197[First fish(C1)] 2[Black fish(C2)] 0[Clown fish(C3)] 9[C4] 0[C5] 1[C6] 
Black fish(C2) -> 0[First fish(C1)] 197[Black fish(C2)] 0[Clown fish(C3)] 1[C4] 1[C5] 0[C6] 
Clown fish(C3) -> 0[First fish(C1)] 0[Black fish(C2)] 199[Clown fish(C3)] 0[C4] 1[C5] 0[C6] 
C4 -> 0[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 39[C4] 0[C5] 0[C6] 
C5 -> 2[First fish(C1)] 0[Black fish(C2)] 1[Clown fish(C3)] 1[C4] 48[C5] 0[C6] 
C6 -> 1[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 49[C6] 
```


### ConvNet + BatchNorm

```
- Layer 0 : InputLayer           (3, 50, 50)                 
- Layer 1 : BatchNorm            (3, 50, 50)   [1 inputs: 0] 
- Layer 2 : Convolution2D        (20, 46, 46)  [1 inputs: 1] 
- Layer 3 : Pool2D               (20, 23, 23)  [1 inputs: 2] 
- Layer 4 : Activation reLU      (20, 23, 23)  [1 inputs: 3] 
- Layer 5 : BatchNorm            (20, 23, 23)  [1 inputs: 4] 
- Layer 6 : Convolution2D        (40, 19, 19)  [1 inputs: 5] 
- Layer 7 : Pool2D               (40, 9, 9)    [1 inputs: 6] 
- Layer 8 : Activation reLU      (40, 9, 9)    [1 inputs: 7] 
- Layer 9 : BatchNorm            (40, 9, 9)    [1 inputs: 8] 
- Layer 10 : Convolution2D       (80, 5, 5)    [1 inputs: 9] 
- Layer 11 : Pool2D              (80, 2, 2)    [1 inputs: 10]
- Layer 12 : Activation reLU     (80, 2, 2)    [1 inputs: 11]
- Layer 13 : BatchNorm           (80, 2, 2)    [1 inputs: 12]
- Layer 14 : Dense               (1000,)       [1 inputs: 13]
- Layer 15 : Activation tanh     (1000,)       [1 inputs: 14]
- Layer 16 : BatchNorm           (1000,)       [1 inputs: 15]
- Layer 17 : Dense               (400,)        [1 inputs: 16]
- Layer 18 : Activation tanh     (400,)        [1 inputs: 17]
- Layer 19 : Dense               (200,)        [1 inputs: 18]
- Layer 20 : Activation tanh     (200,)        [1 inputs: 19]
- Layer 21 : Dense               (6,)          [1 inputs: 20]
- Layer 22 : Activation softmax  (6,)          [1 inputs: 21]
=> Network built!
Read datas...
Start training
Start building training function...

Training with 11590 in mini-batches of size 40 for 60 epochs
-> optimizer (algorithm) : MomentumGradient
-> learning rate : 0.04
-> regularization : 0.0
```

```
===== DataSet test

Test accuracy: 97.73% (733 over 750 examples) 

First fish(C1) => nb rightly classified : 195 / 200 | nb false positive : 10
Black fish(C2) => nb rightly classified : 198 / 200 | nb false positive : 1
Clown fish(C3) => nb rightly classified : 200 / 200 | nb false positive : 1
C4 => nb rightly classified : 41 / 50 | nb false positive : 3
C5 => nb rightly classified : 50 / 50 | nb false positive : 1
C6 => nb rightly classified : 49 / 50 | nb false positive : 1

First fish(C1) -> 195[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 8[C4] 0[C5] 1[C6] 
Black fish(C2) -> 1[First fish(C1)] 198[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 0[C6] 
Clown fish(C3) -> 1[First fish(C1)] 0[Black fish(C2)] 200[Clown fish(C3)] 0[C4] 0[C5] 0[C6] 
C4 -> 2[First fish(C1)] 1[Black fish(C2)] 0[Clown fish(C3)] 41[C4] 0[C5] 0[C6] 
C5 -> 0[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 1[C4] 50[C5] 0[C6] 
C6 -> 1[First fish(C1)] 0[Black fish(C2)] 0[Clown fish(C3)] 0[C4] 0[C5] 49[C6] 
```


### species : fish_08, fish_09, fish_10

### ConvNet

```
Articial neural network : Sequential
- Layer 0 : InputLayer           (50, 50)                    
- Layer 1 : BatchNorm            (50, 50)      [1 inputs: 0] 
- Layer 2 : Convolution2D        (20, 46, 46)  [1 inputs: 1] 
- Layer 3 : Pool2D               (20, 23, 23)  [1 inputs: 2] 
- Layer 4 : Activation tanh      (20, 23, 23)  [1 inputs: 3] 
- Layer 5 : BatchNorm            (20, 23, 23)  [1 inputs: 4] 
- Layer 6 : Convolution2D        (40, 19, 19)  [1 inputs: 5] 
- Layer 7 : Pool2D               (40, 9, 9)    [1 inputs: 6] 
- Layer 8 : Activation tanh      (40, 9, 9)    [1 inputs: 7] 
- Layer 9 : BatchNorm            (40, 9, 9)    [1 inputs: 8] 
- Layer 10 : Convolution2D       (80, 5, 5)    [1 inputs: 9] 
- Layer 11 : Pool2D              (80, 2, 2)    [1 inputs: 10]
- Layer 12 : Activation reLU     (80, 2, 2)    [1 inputs: 11]
- Layer 13 : BatchNorm           (80, 2, 2)    [1 inputs: 12]
- Layer 14 : Dense               (1000,)       [1 inputs: 13]
- Layer 15 : Activation tanh     (1000,)       [1 inputs: 14]
- Layer 16 : BatchNorm           (1000,)       [1 inputs: 15]
- Layer 17 : Dense               (400,)        [1 inputs: 16]
- Layer 18 : Activation tanh     (400,)        [1 inputs: 17]
- Layer 19 : BatchNorm           (400,)        [1 inputs: 18]
- Layer 20 : Dense               (200,)        [1 inputs: 19]
- Layer 21 : Activation tanh     (200,)        [1 inputs: 20]
- Layer 22 : Dense               (3,)          [1 inputs: 21]
- Layer 23 : Activation softmax  (3,)          [1 inputs: 22]
=> Network built!
Read datas...
Start training
Start building training function...

Training with 548 in mini-batches of size 40 for 60 epochs
-> optimizer (algorithm) : MomentumGradient
-> learning rate : 0.06
-> regularization : 0.0
```

```
===== DataSet test

Test accuracy: 98.00% (147 over 150 examples) 

C4 => nb rightly classified : 47 / 50 | nb false positive : 0
C5 => nb rightly classified : 50 / 50 | nb false positive : 3
C6 => nb rightly classified : 50 / 50 | nb false positive : 0

C4 -> 47[C4] 0[C5] 0[C6] 
C5 -> 3[C4] 50[C5] 0[C6] 
C6 -> 0[C4] 0[C5] 50[C6] 
```

### ConNet with data aumentation

- rotations
- mirror

```
Articial neural network : Sequential
- Layer 0 : InputLayer           (3, 50, 50)                 
- Layer 1 : BatchNorm            (3, 50, 50)   [1 inputs: 0] 
- Layer 2 : Convolution2D        (20, 46, 46)  [1 inputs: 1] 
- Layer 3 : Pool2D               (20, 23, 23)  [1 inputs: 2] 
- Layer 4 : Activation tanh      (20, 23, 23)  [1 inputs: 3] 
- Layer 5 : BatchNorm            (20, 23, 23)  [1 inputs: 4] 
- Layer 6 : Convolution2D        (40, 19, 19)  [1 inputs: 5] 
- Layer 7 : Pool2D               (40, 9, 9)    [1 inputs: 6] 
- Layer 8 : Activation tanh      (40, 9, 9)    [1 inputs: 7] 
- Layer 9 : BatchNorm            (40, 9, 9)    [1 inputs: 8] 
- Layer 10 : Convolution2D       (80, 5, 5)    [1 inputs: 9] 
- Layer 11 : Pool2D              (80, 2, 2)    [1 inputs: 10]
- Layer 12 : Activation reLU     (80, 2, 2)    [1 inputs: 11]
- Layer 13 : BatchNorm           (80, 2, 2)    [1 inputs: 12]
- Layer 14 : Dense               (1000,)       [1 inputs: 13]
- Layer 15 : Activation tanh     (1000,)       [1 inputs: 14]
- Layer 16 : BatchNorm           (1000,)       [1 inputs: 15]
- Layer 17 : Dense               (400,)        [1 inputs: 16]
- Layer 18 : Activation tanh     (400,)        [1 inputs: 17]
- Layer 19 : BatchNorm           (400,)        [1 inputs: 18]
- Layer 20 : Dense               (200,)        [1 inputs: 19]
- Layer 21 : Activation tanh     (200,)        [1 inputs: 20]
- Layer 22 : Dense               (3,)          [1 inputs: 21]
- Layer 23 : Activation softmax  (3,)          [1 inputs: 22]
=> Network built!
Read datas...
TRAIN DATAS : 2 1 (2740, 3, 50, 50)
Start training
Start building training function...

Training with 2740 in mini-batches of size 40 for 60 epochs
-> optimizer (algorithm) : MomentumGradient
-> learning rate : 0.06
-> regularization : 0.0
```

```
===== DataSet test

Test accuracy: 99.33% (149 over 150 examples) 

C4 => nb rightly classified : 49 / 50 | nb false positive : 0
C5 => nb rightly classified : 50 / 50 | nb false positive : 1
C6 => nb rightly classified : 50 / 50 | nb false positive : 0

C4 -> 49[C4] 0[C5] 0[C6] 
C5 -> 1[C4] 50[C5] 0[C6] 
C6 -> 0[C4] 0[C5] 50[C6] 
```