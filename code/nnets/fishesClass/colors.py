import numpy as np
import theano
import os
from dml import *
import common as cc
from common import *

IMG_SHAPE = (50, 50)
# IMG_COLOR_SHAPE = (6,) + IMG_SHAPE
# IMG_COLOR_SHAPE = IMG_SHAPE
cc.NB_CLASSES = 6
cc.CLASS_FOLD = ["01", "03", "04", "08", "09", "10"]
cc.CLASS_NAME = ["First fish(C1)", "Black fish(C2)", "Clown fish(C3)", "C4", "C5", "C6"]

cc.preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=False)

def getColorDensity(example):
	return np.mean(example, axis=tuple(range(1, example.ndim)))

def getDataSetDensities(datas):
	classes = [[] for _ in range(cc.NB_CLASSES)]
	nbExamples = len(datas[0][0])
	for i in range(nbExamples):
		classes[datas[1][0][i].argmax()].append(getColorDensity(datas[0][0][i]))

	return np.array([np.mean(l, axis=0) for l in classes])

def distFct(a, b):
	return np.sum((a-b)**2)

def predictClass(example, colors):
	d = [0] * cc.NB_CLASSES
	for c in range(cc.NB_CLASSES):
		d[c] = distFct(getColorDensity(example), colors[c])
	return np.array(d).argmin()

def predictDataSet(datas, colors):
	metrics = OneClassMetrics(cc.NB_CLASSES)
	nbExamples = len(datas[0][0])
	for i in range(nbExamples):
		metrics.addResult(
			datas[1][0][i].argmax(),
			predictClass(datas[0][0][i], colors)
		)
	return metrics

def main():
	quickTest = False

	print("Read datas...")
	if quickTest:
		validationDatas = getDataSet("validation")
		testDatas, trainingDatas = validationDatas, validationDatas
	else:
		trainingDatas = getDataSet("train")
		validationDatas = getDataSet("validation")
		testDatas = getDataSet("test")

	print("Compute colors...")
	colors = getDataSetDensities(trainingDatas)
	print("Colors:", colors)

	# print("Predict results")
	# printMetricResults(predictDataSet(validationDatas, colors), "validation")
	printMetricResults(predictDataSet(testDatas, colors), "test")

if __name__ == '__main__':
	main()