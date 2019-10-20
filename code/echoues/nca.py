"""
	Neighbourhood components analysis implementation using numpy
	Try to optimize transform function using a method inspired of the stochastic gradient descent
	https://en.wikipedia.org/wiki/Neighbourhood_components_analysis
"""

import numpy as np
from dml.algos import *
# import theano
# import theano.tensor as T

def ncaOptimClassifier(clf, learningRate=0.1, epochs=10): # classifier -> instance of KNearestClassifier or subclass
	# transforms = [theano.shared(t) for t in clf.transforms]
	costs = []
	gradients = []
	# print("Build tensors")

	# for iLayer in range(len(clf.datas[0])):
	# 	A = transforms[iLayer]
	# 	x = clf.datas[0][iLayer][np.newaxis].T
	# 	eij = [
	# 		[
	# 			T.exp(-T.sqr( T.sum(T.dot(A, x[i]) - T.dot(A, x[j])) ))
	# 			for j in range(clf.nbExamples)
	# 		] for i in range(clf.nbExamples)
	# 	]
	# 	xij = [
	# 		[x[i]-x[j] for j in range(clf.nbExamples)]
	# 		for i in range(clf.nbExamples)
	# 	]
	# 	s = [T.sum([eij[i][j] for j in range(clf.nbExamples)]) for i in range(clf.nbExamples)]

	# 	p = [
	# 		T.sum([
	# 				eij[i][j] / s[i]
	# 				for j in range(clf.nbExamples)
	# 				if i != j and clf.getClassOf(i) == clf.getClassOf(j)
	# 		]) for i in range(clf.nbExamples)
	# 	]
	# 	costs.append(T.sum(p))

	# 	# s_terms = [
	# 	# 	[]
	# 	# ]
	# 	# gradients.append(2*T.dot(A,
	# 	# 	T.sum([])
	# 	# ))
	# print("Build train function")
	# cost = T.sum(costs)

	# cost = T.sum(costs)
	# algo = GradientAlgo(learningRate)
	# trainAlgo = algo.trainFct(cost, [], [], [], transforms)

	# exit(0)

	# print("Build train function")
	# cost = T.sum(costs)

	print("Start training")
	for iEpoch in range(epochs):
		trainCost = 0
		for iLayer in range(len(clf.datas[0])):
			A = clf.transforms[iLayer]
			x = [ex.flatten()[np.newaxis].T for ex in clf.datas[0][iLayer]]
			eij = [
				[
					np.exp(-np.square( np.sum(np.dot(A, x[i]) - np.dot(A, x[j]) )))
					for j in range(clf.nbExamples)
				] for i in range(clf.nbExamples)
			]
			# xij = [
			# 	[x[i]-x[j] for j in range(clf.nbExamples)]
			# 	for i in range(clf.nbExamples)
			# ]
			# s = [np.sum([eij[i][j] for j in range(clf.nbExamples)]) for i in range(clf.nbExamples)]

			# pij = [
			# 	[
			# 		eij[i][j] / s[i] if i!= j else 0
			# 		for j in range(clf.nbExamples)
			# 	] for i in range(clf.nbExamples)
			# ]

			# p = [
			# 	np.sum([
			# 		pij[i][j]
			# 		for j in range(clf.nbExamples)
			# 		if i != j and clf.getClassOf(i) == clf.getClassOf(j)
			# 	]) for i in range(clf.nbExamples)
			# ]
			# costs.append(np.sum(p))

			# s_terms = [
			# 	[pij[i][j] * np.dot(xij[i][j], xij[i][j]) for j in range]
			# 	for i in range(clf.nbExamples)
			# ]
			# gradients.append(2*np.dot(A,
			# 	np.sum([
			# 		p[i] * np.sum([s_terms[k] for k in range(clf.nbExamples)], axis=0)
			# 		- np.sum([s_terms[j] for j in range(clf.nbExamples) if clf.getClassOf(i) == clf.getClassOf(j)], axis=0)
			# 		for i in range(clf.nbExamples)
			# 	], axis=0)
			# ))
		print(gradients)
		
		print("Epoch", iEpoch, ": trainCost =", trainCost)
		exit(0)
		# TODO: update

	clf.transformDatas()