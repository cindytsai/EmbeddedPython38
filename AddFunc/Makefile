BIN  := AddFunc.out
FILE := main.c methods.c
COMPILER := mpicc

$(BIN): $(FILE)
	$(COMPILER) $(shell python3.8-config --cflags --embed) -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed)

clean:
	rm -f *.out
	rm -rf log
	rm -rf __pycache__
