import numpy as np
import theano
import os
from dml import *

IMG_SHAPE = (50, 50)
IMG_COLOR_SHAPE = (3,) + IMG_SHAPE
NB_CLASSES = 3
CLASS_FOLD = ["01", "03", "04"]
CLASS_NAME = ["First fish", "Black fish", "Clown fish"]
preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=False)

def fp(path): # Format the path, convert "/" to "\" under windows
	return os.path.join(*path.split("/"))

def readDatasFrom(folders, labels):
	datas = ImageDataFlow(preprocess)
	for folderPath, idClass in zip(folders, labels):
		datas.addSource(fp(folderPath), binRow(NB_CLASSES, idClass))
	return datas.getAll()

def getDataSet(name):
	folder = "datas/fishes/"
	return readDatasFrom(
		[os.path.join(folder, fold, name) for fold in CLASS_FOLD],
		list(range(NB_CLASSES))
	)

def main():
	# debugOn()
	
	network = Sequential([
		InputLayer(IMG_COLOR_SHAPE),

		Dense(800),
		Activation(tanh),

		Dense(300),
		Activation(tanh),

		Dense(100),
		Activation(tanh),

		Dense(NB_CLASSES),
		Activation(tanh),
	], #defaultLoss=logLikelihoodCost
	)

	network.setChecker(OneClassChecker())

	network.build()

	print("=> Network built!")
	print("Read datas...")

	validationDatas = getDataSet("validation")
	testDatas = getDataSet("test")
	trainingDatas = getDataSet("train")
	# testDatas, trainingDatas = validationDatas, validationDatas

	print("Start training")
	network.train(
		trainingDatas,
		nbEpochs = 60,
		batchSize = 40,
		# algo = MomentumGradient(0.3),
		algo = GradientAlgo(0.02),
		monitors = StdOutputMonitor([
			("train", trainingDatas),
			("validation", validationDatas),
			("test", testDatas),
		]),
		regul = 0.000
	)

	metrics = network.checker.evaluate(network, testDatas)
	total, success, rate = metrics.nbElems, metrics.nbRightElems, metrics.accuracy()
	print("Test accuracy: {:.2f}% ({} over {} examples)".format(rate*100, success, total))

	for iClass in range(NB_CLASSES):
		print(CLASS_NAME[iClass],
			"=> nb rightly classified :", metrics.nbRightInClass[iClass],
			"/", metrics.nbElInClass[iClass],
			"| nb false positive :", metrics.nbMisclassIn[iClass])

	grid = metrics.getClassGrid()

	for iClass in range(NB_CLASSES):
		print(CLASS_NAME[iClass], "-> ", end="")
		for iClass2 in range(NB_CLASSES):
			print(grid[iClass][iClass2], "[", CLASS_NAME[iClass2], "] ", sep="", end="")
		print()


if __name__ == '__main__':
	main()