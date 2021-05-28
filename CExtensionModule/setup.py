# Install fputs module
# (1) With gcc or any C compiler, and write changed files to files.txt
# CC=gcc python3 setup.py install --record files.txt
# 
# (2) Use default, clang
# python3 setup.py install
# 
# (3) Or use pip
# pip38 install .
# 

from distutils.core import setup, Extension

def main():
	setup(name = "fputs",
		  version = "1.0.0",
		  description = "Python interface for the fputs C library function",
		  author = "Shin-Rong Tsai",
		  author_email = "turquoisea.tsai@gmail.com",
		  # Takes a list of objects of the "extensions" class.
		  # An object of the "extensions" class describes a single C/C++ extension module in a setup script
		  # [ name of the module, [ list of paths to files with the source code relative to the setup script ]]
		  ext_modules=[Extension("fputs", ["module.c"])])

if __name__ == "__main__":
	main()