import numpy as np
import theano
import os
from dml import *
from dml.knearest import *
import common as cc
from common import *
import random

IMG_SHAPE = (50, 50)
IMG_COLOR_SHAPE = (3,) + IMG_SHAPE
# IMG_COLOR_SHAPE = IMG_SHAPE
cc.NB_CLASSES = 6
cc.CLASS_FOLD = ["01", "03", "04", "08", "09", "10"]
cc.CLASS_NAME = ["First fish(C1)", "Black fish(C2)", "Clown fish(C3)", "C4", "C5", "C6"]

cc.preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=False)

augment = DataAugment()
augment.addRotations()
augment.addMirror()

FILE_NAME = "sia_fishes_" + str(random.randint(1, 1e4))
FILE_NAME = "sia_fishes_test"
FILE_NAME_LOAD = "sia_fishes1"

print("Using FILE_NAME =", FILE_NAME)

def transformDatas(dataset, nnet):
	dataset[0] = nnet.runBatch(dataset[0])

K_NEIGHBORS = 1

def main():
	# debugOn()
	quickTest = False
	fromFile = False
	
	if fromFile:
		network = Network.loadFrom("fishes/saves/"+FILE_NAME_LOAD+".json")
	else:
		# network = SiameseNNet([
		# 	InputLayer(IMG_COLOR_SHAPE),
		# 	BatchNorm(),

		# 	Convolution2D((5, 5), 20, padding=None),
		# 	Pool2D((2, 2)),
		# 	Activation(reLU),
		# 	BatchNorm(),

		# 	Convolution2D((5, 5), 40, padding=None),
		# 	Pool2D((2, 2)),
		# 	Activation(reLU),
		# 	BatchNorm(),

		# 	Convolution2D((5, 5), 80, padding=None),
		# 	Pool2D((2, 2)),
		# 	Activation(reLU),
		# 	BatchNorm(),

		# 	Dense(1000),
		# 	Activation(tanh),
		# 	BatchNorm(),

		# 	Dense(400),
		# 	Activation(reLU),

		# 	Dense(200),
		# 	Activation(reLU),

		# 	Dense(6),
		# 	Activation(softmax),
		# ], defaultLoss=l2cost
		# )

		network = SiameseNNet([
			InputLayer(IMG_COLOR_SHAPE),
			Dense(400),
			Activation(tanh),
			Dense(200),
			Activation(tanh),
			Dense(6),
			Activation(softmax),
		], defaultLoss=l2cost
		)
		network.build()
		# network.loadParameters("fishes/saves/fishes0.mat")
		# network.loadParameters("saves/fishes_1_small.mat")
		
		nbLayers = len(network.layers)
		network.remove([nbLayers-2, nbLayers-1])

		# network.setChecker(OneClassChecker())

	network.build()

	if not fromFile:
		network.saveTo("fishes/saves/"+FILE_NAME+".json")

	print("=> Network built!")
	print("Read datas...")

	validationDatas = getDataSet("validation")
	if quickTest:
		testDatas, trainingDatas = validationDatas, validationDatas
	else:
		testDatas = getDataSet("test")
		if cc.NB_CLASSES == 3:
			trainingDatas = getDataSet("train")
		else:
			trainingDatas_larg = getDataSet("train", [0, 1, 2])
			trainingDatas_small = getDataSet("train", [3, 4, 5], augment)
			trainingDatas = mergeIODatas([trainingDatas_small, trainingDatas_larg])

	if fromFile:
		network.loadParameters("fishes/saves/"+FILE_NAME_LOAD+".mat")
	else:
		print("Start training")
		network.train(
			trainingDatas,
			nbEpochs = 10,
			batchSize = 40,
			# algo = MomentumGradient(0.04),
			algo = GradientAlgo(0.004),
			monitors = StdOutputMonitor([
				# ("train", trainingDatas),
				# ("validation", validationDatas),
				# ("test", testDatas),
			]),
			regul = 0.0#1
		)

		network.saveParameters("fishes/saves/"+FILE_NAME+".mat")

	# printResults(network, "test", testDatas)


	transformDatas(validationDatas, network)
	if not quickTest:
		transformDatas(testDatas, network)
		transformDatas(trainingDatas, network)


	classifier = KNearestClassifier(nbClasses=cc.NB_CLASSES, k=K_NEIGHBORS, datas=trainingDatas)
	
	print("Predict datas with K =", K_NEIGHBORS)
	printMetricResults(classifier.evalDataset(validationDatas), "validation")
	printMetricResults(classifier.evalDataset(testDatas), "test")

if __name__ == '__main__':
	main()