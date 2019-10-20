import numpy as np
import theano
import os
from dml import *

# print(os.environ['Path'])
# print(os.environ['GPUARRAY_CUDA_VERSION'])


# from theano import function, config, shared, tensor
# import time

# vlen = 10 * 30 * 768  # 10 x #cores x # threads per core
# iters = 1000

# rng = np.random.RandomState(22)
# x = shared(np.asarray(rng.rand(vlen), config.floatX))
# f = function([], tensor.exp(x))
# print(f.maker.fgraph.toposort())
# t0 = time.time()
# for i in range(iters):
#     r = f()
# t1 = time.time()
# print("Looping %d times took %f seconds" % (iters, t1 - t0))
# print("Result is %s" % (r,))
# if np.any([isinstance(x.op, tensor.Elemwise) and
#               ('Gpu' not in type(x.op).__name__)
#               for x in f.maker.fgraph.toposort()]):
#     print('Used the cpu')
# else:
#     print('Used the gpu')
# exit(0)

IMG_SHAPE = (28, 28)


def readDatasFrom(filename):
	with open(filename, 'r') as infile:
		lines = infile.readlines()
	nbEntries = int(lines[0])
	x, y = [], []

	for l in range(1, len(lines)-1, 2):
		x.append(
			np.array(
				list(map(float, lines[l].split())), dtype=theano.config.floatX
			).reshape(28, 28),
		)
		y.append( np.array(list(map(float, lines[l+1].split())), dtype=theano.config.floatX) )

	x = np.array(x)
	y = np.array(y)
	return [[x, x], [y, y]]

def fp(path): # Format the path, convert "/" to "\" under windows
	return os.path.join(*path.split("/"))

def main():
	# debugOn()
	fromFile = False
	quickTest = True

	if fromFile:
		print("Load network from file")
		network = Sequential.loadFrom(fp('saves/mnist.json'))
	else:
		"""network = Sequential([
			InputLayer(IMG_SHAPE),
			# BatchNorm(),

			Convolution2D((5, 5), 20, padding=None),
			Pool2D((2, 2)),
			Activation(reLU),

			Convolution2D((5, 5), 40, padding=None),
			Activation(reLU),
			# Pool2D((2, 2)),

			Convolution2D((3, 3), 80, padding=None),
			Activation(reLU),
			Pool2D((2, 2)),

			Dense(800),
			Activation(reLU),

			Dense(300),
			Activation(reLU),

			Dense(100),
			Activation(reLU),
			# BatchNorm(),

			Dense(10),
			Activation(softmax),
		])"""
		network = Sequential(defaultLoss=logLikelihoodCost)
		l0 = network.add(InputLayer(IMG_SHAPE))
		l01 = network.add(InputLayer(IMG_SHAPE)) # TODO

		m = network.add(Merge()).withInput([l0, l01]) # TODO

		# l1 = network.add(Dense(300))
		# a1 = network.add(Activation(reLU))

		l2 = network.add(Dense(10))
		a2 = network.add(Activation(softmax))

		s2 = network.add(Activation(softmax)).withInput(l2) # TODO

		network.addOutput([a2, s2])

		network.setChecker(OneClassChecker())

		network.build()

		print("=> Network built!")
		print("Read datas...")

		if not quickTest:
			trainingDatas = readDatasFrom(fp("datas/mnist/training.in"))
			validationDatas = readDatasFrom(fp("datas/mnist/validation.in"))
			testDatas = readDatasFrom(fp("datas/mnist/test.in"))
		else:
			trainingDatas = readDatasFrom(fp("datas/mnist/test.in"))
			testDatas = trainingDatas
			validationDatas = trainingDatas

		print("Start training")
		network.train(
			trainingDatas,
			nbEpochs = 60,
			batchSize = 30,
			algo = MomentumGradient(0.3),
			# algo = GradientAlgo(0.3),
			monitors = StdOutputMonitor([
				("validation", validationDatas),
				("test", testDatas),
			], #autoSave=fp('saves/mnist.json')
			),
			regul = 0.000
		)

	testDatas = readDatasFrom(fp("datas/mnist/test.in"))
	network.checker.evaluate(network, testDatas)
	total, success, rate = network.checker.getAccuracyMetrics()
	print("Test accuracy: {:.2f}% ({} over {} examples)".format(rate*100, success, total))

if __name__ == '__main__':
	main()