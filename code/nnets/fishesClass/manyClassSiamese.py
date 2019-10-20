import numpy as np
import theano
import os
from dml import *
from dml.knearest import *
import common
from common import *
import random

DIR_SPECIES = 'datas/fishes_species'
classFolders = [dirName for dirName, e, files in os.walk(DIR_SPECIES) if len(dirName) > 2 + len(DIR_SPECIES)]

IMG_SHAPE = (50, 50)
IMG_COLOR_SHAPE = (3,) + IMG_SHAPE

# Test Datas
common.NB_CLASSES = 3
common.CLASS_FOLD = ["01", "03", "04", "08", "09", "10"]
common.CLASS_NAME = ["First fish(C1)", "Black fish(C2)", "Clown fish(C3)", "C4", "C5", "C6"]

common.preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=False)

FILE_NAME = "sia_mc_fishes_" + str(random.randint(1, 1e4))
FILE_NAME = "sia_mc_fishes_test"
FILE_NAME_LOAD = "sia_mc_fishes_test"

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
		network = SiameseNNet([
				InputLayer(IMG_COLOR_SHAPE),
				Dense(400),
				Activation(tanh),
				Dense(20),
				Activation(tanh),
			],
			defaultLoss=l2cost,
			# dataProvider=RandomSiameseDataProvider
		)

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
		trainingDatas = readDatasFrom(classFolders, list(range(len(classFolders))), labelBinRow=False)

	if fromFile:
		network.loadParameters("fishes/saves/"+FILE_NAME_LOAD+".mat")
	else:
		print("Start training")
		network.train(
			trainingDatas,
			nbEpochs = 10,
			batchSize = 40,
			algo = MomentumGradient(0.0004),
			# algo = GradientAlgo(0.004),
			monitors = StdOutputMonitor([
				# ("validation", validationDatas),
				# ("test", testDatas),
			]),
			regul = 0.0#1
		)

		network.saveParameters("fishes/saves/"+FILE_NAME+".mat")

	def mean(l):
		if len(l) == 0:
			return None
		if isinstance(l[0], list):
			return mean([mean(el) for el in l])
		return sum(l)/len(l)

	ids = list(range(0, 2000, 20))
	r1 = network.runBatch(DirectDataFlow(trainingDatas).getDatas(ids)[0])[0]
	r2 = network.runBatch(DirectDataFlow(trainingDatas).getDatas([i+1 for i in ids])[0])[0]
	print("Dist same class", mean([sum([(a-b)**2 for a, b in zip(r1[i], r2[i])]) for i in range(len(r1))]))
	print("Dist diff class", mean([[sum([(a-b)**2 for a, b in zip(r1[i], r2[j])]) for j in range(i+1, len(r2))]for i in range(len(r1)-1)]))

	transformDatas(validationDatas, network)
	if not quickTest:
		transformDatas(testDatas, network)
		# transformDatas(trainingDatas, network)

	classifier = KNearestClassifier(nbClasses=common.NB_CLASSES, k=K_NEIGHBORS, datas=validationDatas)
	
	print("Predict datas with K =", K_NEIGHBORS)
	# printMetricResults(classifier.evalDataset(validationDatas), "validation")
	printMetricResults(classifier.evalDataset(testDatas), "test")

if __name__ == '__main__':
	main()