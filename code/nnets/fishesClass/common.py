import numpy as np
import theano
import os
from dml import *

NB_CLASSES = 0
CLASS_FOLD = []
CLASS_NAME = []
preprocess = None

def fp(path): # Format the path, convert "/" to "\" under windows
	return os.path.join(*path.split("/"))

def readDatasFrom(folders, labels, augment=None, labelBinRow=True, nbClasses=None):
	datas = ImageDataFlow(preprocess, augment=augment)
	for folderPath, idClass in zip(folders, labels):
		datas.addSource(
			fp(folderPath),
			binRow(nbClasses or NB_CLASSES, idClass) if labelBinRow else idClass
		)
	return datas.getAll()

def getDataSet(name, ids=None, augment=None):
	if not ids:
		ids = range(NB_CLASSES)
	ids = list(ids)
	folders = [CLASS_FOLD[i] for i in ids]

	folder = "datas/fishes/"
	return readDatasFrom(
		[os.path.join(folder, fold, name) for fold in folders],
		ids,
		augment
	)

def printMetricResults(metrics, name):
	print("\n===== DataSet", name)

	total, success, rate = metrics.nbElems, metrics.nbRightElems, metrics.accuracy()
	print("\nTest accuracy: {:.2f}% ({} over {} examples)".format(rate*100, success, total), "\n")

	for iClass in range(NB_CLASSES):
		print(CLASS_NAME[iClass],
			"=> nb rightly classified :", metrics.nbRightInClass[iClass],
			"/", metrics.nbElInClass[iClass],
			"| nb false positive :", metrics.nbMisclassIn[iClass])
	print()

	grid = metrics.getClassGrid()

	for iClass in range(NB_CLASSES):
		print(CLASS_NAME[iClass], "-> ", end="")
		for iClass2 in range(NB_CLASSES):
			print(grid[iClass][iClass2], "[", CLASS_NAME[iClass2], "] ", sep="", end="")
		print()

def printResults(network, name, dataset):
	printMetricResults(network.checker.evaluate(network, dataset), name)