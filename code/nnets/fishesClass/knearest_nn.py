import numpy as np
import theano
import os
from dml import *
from dml.knearest import *
import common as cc
from common import *

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

K_NEIGHBORS = 1

def transformDatas(dataset, nnet):
	dataset[0] = nnet.runBatch(dataset[0])

def main():
	# debugOn()
	quickTest = False
	
	network = Network.loadFrom("fishes/saves/fishes0.json")
	network.build() # First build to load parameters
	network.loadParameters("fishes/saves/fishes0.mat")

	print("=> Network built!")
	print("Read datas...")


	nbToRemove = 2

	nbLayers = len(network.layers)
	network.remove(list(range(nbLayers-nbToRemove, nbLayers)))
	network.addOutput(network.layers[-1])
	network.build() # Build the new model

	validationDatas = getDataSet("validation")
	transformDatas(validationDatas, network)

	if quickTest:
		testDatas, trainingDatas = validationDatas, validationDatas
	else:
		testDatas = getDataSet("test")
		trainingDatas_larg = getDataSet("train", [0, 1, 2])
		trainingDatas_small = getDataSet("train", [3, 4, 5], augment)
		trainingDatas = mergeIODatas([trainingDatas_small, trainingDatas_larg])
		transformDatas(testDatas, network)
		transformDatas(trainingDatas, network)

	# printResults(network, "test", testDatas)
	classifier = KNearestClassifier(nbClasses=cc.NB_CLASSES, k=K_NEIGHBORS, datas=trainingDatas)
	
	print("Predict datas with K =", K_NEIGHBORS)
	# printMetricResults(classifier.evalDataset(validationDatas), "validation")
	printMetricResults(classifier.evalDataset(testDatas), "test")

if __name__ == '__main__':
	main()