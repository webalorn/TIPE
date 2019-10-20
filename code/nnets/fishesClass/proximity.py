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

FILE_NAME = "proxy_fishes_" + str(random.randint(1, 1e4))
FILE_NAME = "proxy_fishes_test"
FILE_NAME_LOAD = "proxy_fishes_test"

print("Using FILE_NAME =", FILE_NAME)

def transformDatas(dataset, nnet):
	dataset[0] = nnet.runBatch(dataset[0])

K_NEIGHBORS = 1

def makeDataPairsSame(datas):
	print("Making same pairs....")
	newDatas = [[[], []], [[]]]
	idPerClass = []
	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		while len(idPerClass) <= iClass:
			idPerClass.append([])
		idPerClass[iClass].append(i)
	classIds = list(range(len(idPerClass)))

	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		random.shuffle(idPerClass[iClass])
		for i2 in idPerClass[iClass][:10]:
			newDatas[0][0].append(datas[0][0][i])
			newDatas[0][1].append(datas[0][0][i2])
			newDatas[1][0].append(0)

	newDatas[0][0] = np.asarray(newDatas[0][0])
	newDatas[0][1] = np.asarray(newDatas[0][1])
	newDatas[1][0] = np.asarray([[el] for el in newDatas[1][0]])
	print("Pairs made")
	return newDatas

def makeDataPairsDiff(datas):
	print("Making diff pairs....")
	newDatas = [[[], []], [[]]]
	idPerClass = []
	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		while len(idPerClass) <= iClass:
			idPerClass.append([])
		idPerClass[iClass].append(i)
	classIds = list(range(len(idPerClass)))

	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		random.shuffle(classIds)
		for iClass2 in [k for k in classIds[:20] if k!=iClass]:
			i2=random.choice(idPerClass[iClass2])
			newDatas[0][0].append(datas[0][0][i])
			newDatas[0][1].append(datas[0][0][i2])
			newDatas[1][0].append(1)

	newDatas[0][0] = np.asarray(newDatas[0][0])
	newDatas[0][1] = np.asarray(newDatas[0][1])
	newDatas[1][0] = np.asarray([[el] for el in newDatas[1][0]])
	print("Pairs made")
	return newDatas

def makeDataPairs(datas):
	print("Making pairs....")
	newDatas = [[[], []], [[]]]
	idPerClass = []
	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		while len(idPerClass) <= iClass:
			idPerClass.append([])
		idPerClass[iClass].append(i)
	classIds = list(range(len(idPerClass)))

	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		random.shuffle(idPerClass[iClass])
		for i2 in idPerClass[iClass][:10]:
			newDatas[0][0].append(datas[0][0][i])
			newDatas[0][1].append(datas[0][0][i2])
			newDatas[1][0].append(0)

	for i in range(len(datas[0][0])):
		iClass = int(datas[1][0][i])
		random.shuffle(classIds)
		for iClass2 in [k for k in classIds[:20] if k!=iClass]:
			i2=random.choice(idPerClass[iClass2])
			newDatas[0][0].append(datas[0][0][i])
			newDatas[0][1].append(datas[0][0][i2])
			newDatas[1][0].append(1)

	newDatas[0][0] = np.asarray(newDatas[0][0])
	newDatas[0][1] = np.asarray(newDatas[0][1])
	newDatas[1][0] = np.asarray([[el] for el in newDatas[1][0]])
	print("Pairs made")
	return newDatas


def main():
	# debugOn()
	quickTest = True
	fromFile = False
	
	if fromFile:
		network = Network.loadFrom("fishes/saves/"+FILE_NAME_LOAD+".json")
	else:
		network = Network(
			[],
			defaultLoss=l2cost,
			# dataProvider=RandomSiameseDataProvider
		)
		in1 = network.add(InputLayer(IMG_COLOR_SHAPE))
		in2 = network.add(InputLayer(IMG_COLOR_SHAPE))
		# flattIn1 = network.add(Flatten().withInput(in1))
		# flattIn2 = network.add(Flatten().withInput(in2))
		network.add([
			Merge(axis=1).withInput([in1, in2]),

			BatchNorm(),

			Convolution2D((5, 5), 20, padding=None),
			Pool2D((2, 2)),
			Activation(reLU),
			BatchNorm(),

			Convolution2D((5, 5), 40, padding=None),
			Pool2D((2, 2)),
			Activation(reLU),
			BatchNorm(),

			Convolution2D((5, 5), 80, padding=None),
			Pool2D((2, 2)),
			Activation(reLU),
			BatchNorm(),

			Dense(1000),
			Activation(tanh),
			BatchNorm(),

			Dense(400),
			Activation(reLU),

			Dense(200),
			Activation(reLU),

			Dense(1),
			Activation(tanh),
		])

		# network.setChecker(OneClassChecker())

	network.build()

	if not fromFile:
		network.saveTo("fishes/saves/"+FILE_NAME+".json")

	print("=> Network built!")
	print("Read datas...")

	validationDatas = getDataSet("validation")
	if quickTest:
		testDatas, baseTrainingDatas = validationDatas, validationDatas
		baseTrainingDatas[1][0] = np.array([l.argmax() for l in baseTrainingDatas[1][0]], dtype=theano.config.floatX)
	else:
		testDatas = getDataSet("test")
		baseTrainingDatas = readDatasFrom(classFolders, list(range(len(classFolders))), labelBinRow=False)


	trainingDatas = makeDataPairs(baseTrainingDatas)

	if fromFile:
		network.loadParameters("fishes/saves/"+FILE_NAME_LOAD+".mat")
	else:
		print("Start training")
		network.train(
			trainingDatas,
			nbEpochs = 10,
			batchSize = 40,
			algo = MomentumGradient(0.004),
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
	r1 = network.runBatch(makeDataPairsSame(baseTrainingDatas)[0])[0]
	r2 = network.runBatch(makeDataPairsDiff(baseTrainingDatas)[0])[0]
	print("Dist same class", r1.mean())
	print("Dist diff class", r2.mean())

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