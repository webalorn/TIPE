import numpy as np
import theano
import os
from dml import *
import itertools

preprocess = None

def readDatasFrom(folders, labels, augment=None):
	datas = ImageDataFlow(preprocess, augment=augment)
	for folderPath, idClass in zip(folders, labels):
		datas.addSource(
			folderPath,
			idClass
		)
	return datas.getAll()

def getDataSet(dirPath, augment=None, maxCharSet=None):
	charSets = [os.path.join(dirPath, d) for d in os.listdir(dirPath)]
	charSets = list(filter(os.path.isdir, charSets))
	if maxCharSet:
		charSets = charSets[:maxCharSet]
	charDirs = list(itertools.chain(*[[os.path.join(d, p) for p in os.listdir(d)] for d in charSets]))
	charDirs = list(filter(os.path.isdir, charDirs))

	return readDatasFrom(
		charDirs,
		list(range(len(charDirs))),
		augment
	)

# def printMetricResults(metrics, name):
# 	print("\n===== DataSet", name)

# 	total, success, rate = metrics.nbElems, metrics.nbRightElems, metrics.accuracy()
# 	print("\nTest accuracy: {:.2f}% ({} over {} examples)".format(rate*100, success, total), "\n")

# 	for iClass in range(NB_CLASSES):
# 		print(CLASS_NAME[iClass],
# 			"=> nb rightly classified :", metrics.nbRightInClass[iClass],
# 			"/", metrics.nbElInClass[iClass],
# 			"| nb false positive :", metrics.nbMisclassIn[iClass])
# 	print()

# 	grid = metrics.getClassGrid()

# 	for iClass in range(NB_CLASSES):
# 		print(CLASS_NAME[iClass], "-> ", end="")
# 		for iClass2 in range(NB_CLASSES):
# 			print(grid[iClass][iClass2], "[", CLASS_NAME[iClass2], "] ", sep="", end="")
# 		print()

# def printResults(network, name, dataset):
# 	printMetricResults(network.checker.evaluate(network, dataset), name)