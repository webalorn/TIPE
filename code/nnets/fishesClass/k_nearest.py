import numpy as np
import theano
import os
from dml import *
from dml.knearest import *
import common as cc
from common import *

IMG_SHAPE = (50, 50)
# IMG_COLOR_SHAPE = (6,) + IMG_SHAPE
# IMG_COLOR_SHAPE = IMG_SHAPE
cc.NB_CLASSES = 6
cc.CLASS_FOLD = ["01", "03", "04", "08", "09", "10"]
# cc.CLASS_FOLD = ["08", "09", "10"]
cc.CLASS_NAME = ["First fish(C1)", "Black fish(C2)", "Clown fish(C3)", "C4", "C5", "C6"]
# cc.CLASS_NAME = ["Clown fish(C3)", "C4", "C5", "C6"]

cc.preprocess = ImagePreprocess(newShape=IMG_SHAPE, grayscale=False)

K_NEIGHBORS = 5

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

	classifier = KNearestClassifier(nbClasses=cc.NB_CLASSES, k=K_NEIGHBORS, datas=trainingDatas, distFct=manhanttanDist)
	
	print("Predict datas with K =", K_NEIGHBORS)
	printMetricResults(classifier.evalDataset(testDatas), "test")

if __name__ == '__main__':
	main()