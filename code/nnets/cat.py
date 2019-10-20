import numpy as np
import theano, dml
import os
from dml import *
# import matplotlib.pyplot as plt

# 12500 dogs, 12500 cats
# 11500 for training, 1000 for testing

IMG_SHAPE = (256, 256)
IMG_CHAN_SHAPE = (3,)+IMG_SHAPE

def fp(path): # Format the path, convert "/" to "\" under windows
	return os.path.join(*path.split("/"))

def getInfos(name):
	label, num, ext = name.split(".")
	labelClass = 0 if label == 'dog' else 1
	return int(num), binRow(2, labelClass)

def keepTrain(name):
	num, label = getInfos(name)
	# return num < 150
	return num < 11500

def getLabel(name):
	num, label = getInfos(name)
	return label



import os, psutil

k = 0
def debugImg(name):
	global k
	if k%100 == 0:
		print(k, psutil.Process(os.getpid()).memory_info().rss / 1e6, "MB")
	k += 1

def main():
	# dml.debugOn()

	preprocess = ImagePreprocess(newShape=IMG_SHAPE)

	print("Read training datas...")
	trainingDatas = ImageDataFlow(preprocess)
	trainingDatas.addSource(fp('datas/dogs/train'), getLabel, keepTrain)


	print("Read validation datas...")
	# validationDatas = trainingDatas.getAll()
	validationDatas = readImagesFrom(fp('datas/dogs/train'), getLabel,
		lambda name : getInfos(name)[0] >= 11500 + 500,
		# lambda name : getInfos(name)[0] <= 200,
		preprocess=preprocess,
	)

	network = Sequential([
		InputLayer(IMG_CHAN_SHAPE),

		# Convolution2D((3, 3), 32, padding='same'),
		# Activation(weakReLU),
		# Pool2D((2, 2)),

		# Convolution2D((3, 3), 32, padding='same'),
		# Activation(weakReLU),
		# Pool2D((2, 2)),

		# Convolution2D((5, 5), 64, padding='same'),
		# Activation(tanh),
		# Convolution2D((5, 5), 64, padding='same'),
		# Activation(tanh),
		# Pool2D((2, 2)),

		# Convolution2D((5, 5), 256, padding='same'),
		# Activation(tanh),
		# Convolution2D((5, 5), 256, padding='same'),
		# Activation(tanh),
		# Pool2D((2, 2)),

		Dense(500),
		# Dropout(0.5),
		Activation(tanh),

		Dense(500),
		# Dropout(0.5),
		Activation(tanh),

		Dense(64),
		# Dropout(0.5),
		Activation(tanh),

		Dense(2),
		Activation(softmax),
	], maxBatch = 100)

	network.setChecker(OneClassChecker())

	network.build()

	print("=> Network built !")
	print("Read datas...")

	print("Start training")
	network.train(
		trainingDatas,
		nbEpochs = 60,
		batchSize = 60,
		algo = MomentumGradient(0.005), # 0.005 - 0.03 - 0.005
		monitors = StdOutputMonitor([
			("validation", validationDatas),
			# ("test", testDatas),
		], 
		# autoSave=fp('saves/cat.json')
		),
		regul = 0.000,
		loss = logLikelihoodCost
	)


	# for i in range(8):
	# 	trainX = theano.shared(datas[0][i:i+1], name="trainX")
	# 	trainY = theano.shared(datas[1][i:i+1], name="trainY")

	# 	print("TEST", datas[0].shape, datas[1].shape)
	# 	g = T.grad(network.cost, network.params)
	# 	f = theano.function([], g,
	# 		givens = {
	# 				network.inputTensors[0]: trainX,
	# 				network.expectY[0]: trainY,
	# 			}
	# 		)
	# 	f2 = theano.function([], network.cost,
	# 		givens = {
	# 				network.inputTensors[0]: trainX,
	# 				network.expectY[0]: trainY,
	# 			}
	# 		)
	# 	print(i, "Result:", f())
	# 	print(network.runSingleEntry(datas[0][i]), "instead of", datas[1][i])
	# 	print("Cost of", f2())
	# 	print()

if __name__ == '__main__':
	main()