BIN  := NumpyExtension.out
FILE := main.c methods.c
COMPILER := mpicc
INCLUDE := -I/home/cindytsai/software/python/python3.8/lib/python3.8/site-packages/numpy/core/include

$(BIN): $(FILE)
	$(COMPILER) $(INCLUDE) $(shell python3.8-config --cflags --embed) -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed)

clean:
	rm -f *.out
	rm -rf log
	rm -rf __pycache__
