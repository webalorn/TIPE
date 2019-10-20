import numpy as np
import theano
import os
from dml import *
from dml.knearest import *
import common
from common import *
import random

DIR_TRAIN = "datas/images_background"
DIR_TEST = "datas/images_evaluation"

IMG_SHAPE = (50, 50)

common.preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=True)

FILE_NAME = "char_1shot_" + str(random.randint(1, 1e4))
FILE_NAME = "char_1shot_test"
FILE_NAME_LOAD = "char_1shot_test"

print("Using FILE_NAME =", FILE_NAME)

def transformDatas(dataset, nnet):
	dataset[0] = nnet.runBatch(dataset[0])

K_NEIGHBORS = 1

def getRandomIds(dataset, nMax=1000):
	ids = list(range(len(dataset[0][0])-1))
	random.shuffle(ids)
	ids = ids[:nMax]
	return ids + [i+1 for i in ids]

def main():
	# debugOn()
	quickTest = False
	fromFile = False
	
	if fromFile:
		network = Network.loadFrom("saves/"+FILE_NAME_LOAD+".json")
	else:
		network = SiameseNNet([

				InputLayer(IMG_SHAPE),
				BatchNorm(),

				Convolution2D((5, 5), 20, padding=None),
				Pool2D((2, 2)),
				Activation(reLU),
				BatchNorm(),

				Convolution2D((5, 5), 40, padding=None),
				Pool2D((2, 2)),
				Activation(reLU),
				BatchNorm(),

				Dense(400),
				Activation(tanh), # TODO : ReLU ?

				Dense(20),
				Activation(tanh),
			],
			defaultLoss=l2cost,
			dataProvider=RandomSiameseDataProvider
		)
	
		network.setChecker(SiameseChecker())

	network.build()

	if not fromFile:
		network.saveTo("saves/"+FILE_NAME+".json")

	print("=> Network built!")
	print("Read datas...")

	
	if quickTest:
		testDatas = getDataSet(DIR_TEST, maxCharSet=1)
		trainingDatas = testDatas
	else:
		testDatas = getDataSet(DIR_TEST)
		trainingDatas = getDataSet(DIR_TRAIN)

	if fromFile:
		network.loadParameters("saves/"+FILE_NAME_LOAD+".mat")
	else:
		print("Start training")
		network.train(
			trainingDatas,
			nbEpochs = 10,
			batchSize = 32,
			# algo = MomentumGradient(0.000004),
			algo = GradientAlgo(0.00004),
			monitors = StdOutputMonitor([ # TODO
				("test", testDatas, {'sample': getRandomIds(testDatas)}),
				("training", trainingDatas, {'sample': getRandomIds(trainingDatas)}),
			]),
			regul = 0.0#1
		)

		network.saveParameters("saves/"+FILE_NAME+".mat")

	# TODO: print average distance

	def mean(l):
		if len(l) == 0:
			return None
		if isinstance(l[0], list):
			return mean([mean(el) for el in l])
		return sum(l)/len(l)

	# points = network.runBatch(trainingDatas[0])[0]
	# sameClassDists, diffClassDists = [], []
	# ids = list(range(len(trainingDatas[0][0])-1))
	# random.shuffle(ids)
	# ids = ids[:1000]
	# ids = ids + [i+1 for i in ids]

	# points = [points[i] for i in ids]
	# labels = [trainingDatas[1][0][i] for i in ids]

	# for p, clsId, i in zip(points, labels, range(len(points))):
	# 	for p2, cls2Id in zip(points[i+1:], labels[i+1:]):
	# 		# if id(p) != id(p2):
	# 		d = sum((p-p2)**2)
	# 		if clsId == cls2Id:
	# 			sameClassDists.append(d)
	# 		else:
	# 			diffClassDists.append(d)

	# print("Dist same class", mean(sameClassDists))
	# print("Dist diff class", mean(diffClassDists))
	
	# TODO: print prediction accuracy

	# transformDatas(validationDatas, network)
	# if not quickTest:
	# 	transformDatas(testDatas, network)
	# 	# transformDatas(trainingDatas, network)

	# classifier = KNearestClassifier(nbClasses=common.NB_CLASSES, k=K_NEIGHBORS, datas=validationDatas)
	
	# print("Predict datas with K =", K_NEIGHBORS)
	# # printMetricResults(classifier.evalDataset(validationDatas), "validation")
	# printMetricResults(classifier.evalDataset(testDatas), "test")

if __name__ == '__main__':
	main()