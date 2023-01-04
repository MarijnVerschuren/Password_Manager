import sys
import os


run = lambda x: os.system(x)
clear_console = lambda: os.system("cls" if os.name in ["nt", "dos"] else "clear")

compile_folder = os.path.dirname(os.path.abspath(__file__))
root_folder = os.path.dirname(compile_folder)



if __name__ == "__main__":
	argv = sys.argv
	if "-help" in argv:
		print("[-force-clean]:\tdelete all previous build files before building again", sep="\n", end="\n\n")
		exit(0);
	if "-force-clean" in argv:
		run(rf"echo y|rmdir {compile_folder}\build /S")
		run(rf"mkdir {compile_folder}\build")

	os.chdir(rf"{compile_folder}\build")
	run(rf"cmake {compile_folder} -DPYTHON_EXECUTABLE='C:\Users\marij\AppData\Local\Programs\Python\Python38\python.exe'")
	run(f"cmake --build . -v -j {os.cpu_count()}")

	run(rf"echo F|xcopy {compile_folder}\build\Debug\*.pyd {root_folder}\py_lib.pyd /y")

	#clear_console()