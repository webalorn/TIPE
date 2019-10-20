import random
import numpy as np

NB_FILES = 2

DIMS = 32
NEIGHBOURS = 1
SCALE = 1000

NB_POINTS = 10000
NB_QUERIES = 10000

def find_nearest(points, target):
	dists = points - target
	dists = np.sqrt(np.sum(dists**2, axis=1))
	return dists.argmin()

def get_answers(points, queries):
	if NEIGHBOURS == 1:
		return [[find_nearest(points, q) for q in queries]]
	else:
		print("NOT IMPLEMENTED")
		exit(-1)

def create_file(points, queries, answers=None, i_file=0):
	if not answers:
		answers = get_answers(points, queries)
	keys = {"k": NEIGHBOURS, "N": NB_POINTS, "dim": DIMS, "Q": NB_QUERIES, "i": i_file}
	filename = "k_{k}_dim_{dim}_N_{N}_Q_{Q}_test_{i}.in".format(**keys)
	lines = [[DIMS, NB_POINTS, NEIGHBOURS, NB_QUERIES]]
	lines.extend(points)
	lines.extend(queries)
	lines.extend(answers)

	lines = [" ".join([str(el) for el in l]) for l in lines]

	filepath = "tests/" + filename
	with open(filepath, "w") as file:
		file.write("\n".join(lines))
	print("Wrote {}".format(filename))

def gen_random_points_uniform(nb_points):
	return np.random.uniform(0, SCALE, [nb_points, DIMS])

def main():
	for i_file in range(NB_FILES):
		points = gen_random_points_uniform(NB_POINTS)
		queries = gen_random_points_uniform(NB_QUERIES)
		create_file(points, queries, i_file=i_file)

if __name__ == '__main__':
	main()