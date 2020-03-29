import sys, os, json, fnmatch, time
from tabulate import tabulate

def matching(filename, patterns, path):
	if "big_on" not in patterns and "big" in (path+"/"+filename):
		return False
	if "big_only" in patterns and "big" not in (path+"/"+filename):
		return False

	for patt in patterns:
		if fnmatch.fnmatch(filename, patt):
			return True
	return False

def format_duration(duration):
	int_part, float_part = int(duration), duration-int(duration)
	t_min, t_s, t_ms = int_part//60, int_part%60, int(float_part*1000)
	if t_min:
		return "{}m {}s".format(t_min, t_s)
	elif t_s:
		return "{}s {}ms".format(t_s, t_ms)
	return "{}ms".format(t_ms)

def compute_new_stats(stats, duration):
	if "error" in stats:
		return stats
	stats["run_steps_total"] = sum(stats["run_steps"])
	stats["run_steps_min"] = min(stats["run_steps"])
	stats["run_steps_max"] = max(stats["run_steps"])
	stats["run_steps_avg"] = stats["run_steps_total"] / len(stats["run_steps"])
	stats["run_steps_avg_ratio"] = stats["run_steps_avg"] / stats["N"]
	stats["run_steps_avg_percent"] = "{:.2f}%".format(stats["run_steps_avg_ratio"]*100)
	stats["duration"] = format_duration(duration)
	return stats

def list_files_in(path, in_paterns):
	in_files = sorted([f for f in os.listdir(path) if matching(f, in_paterns, path)])
	return [os.path.join(path, f) for f in in_files]

def main():
	if len(sys.argv) < 2:
		print("Error, run.py needs at least one argument")
		exit(0)

	model = sys.argv[1]
	filename = "main"
	if ".cpp" == model[-4:]:
		model_l = model.split("/")
		model = "/".join(model_l[:-1])
		filename = model_l[-1][:-4]
	in_paterns = ["*.in"] + sys.argv[2:]
	real_filename = "{}/{}.cpp".format(model, filename)

	in_files_paths = list_files_in("tests", in_paterns)
	in_files_paths += list_files_in("tests/bigs", in_paterns)
	test_results = []

	print("Compiling...")
	comp_err = os.system("g++ {0}/{1}.cpp -Wall -std=c++11 -o {0}/{1}.o".format(model, filename))
	if comp_err:
		print("Compilation error")
		exit(-1)
	print("Running...")
	for test_path in in_files_paths:
		print("-", test_path, " "*30, end="\r")
		out_file = os.path.join(model, "out")
		start_at = time.time()
		os.system("{}/{}.o < {} > {}".format(model, filename, test_path, out_file))
		end_at = time.time()

		with open(out_file) as f:
			test_results.append(compute_new_stats(json.load(f), end_at-start_at))
	print(" "*60)

	if test_results:
		print(test_results[0].keys())

	headers = ["Test file", "Accuracy", "Build steps", "Run steps", "AVG steps", "AVG steps / N", "Duration"]
	r_keys = ["accuracy", "build_steps", "run_steps_total", "run_steps_avg", "run_steps_avg_percent", "duration"]
	table = []

	if len([True for r in test_results if "error" in r]):
		for r in test_results:
			if not "error" in r:
				r["error"] = ""
		headers.append("Errors")
		r_keys.append("error")

	for test_file, result in zip(in_files_paths, test_results):
		table.append([test_file.split("/")[-1]] + [result[k] if k in result else "-" for k in r_keys])

	print("\nTEST RESULTS FOR \"{}\" ({})".format(model, real_filename))
	print(tabulate(table, headers, tablefmt="grid"))
	

if __name__ == '__main__':
	main()