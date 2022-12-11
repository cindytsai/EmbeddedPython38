BIN  := valid_mpi.out
FILE := valid_mpi.cpp
COMPILER := mpicc

MPI_PATH := /home/cindytsai/software/openmpi/3.1.5-gnu

$(BIN): $(FILE)
	$(COMPILER) $(shell python3.8-config --cflags --embed) -I$(MPI_PATH)/include -I/usr/include/readline -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed) -L$(MPI_PATH)/lib -L/usr/lib/x86_64-linux-gnu -lreadline
	
	#$(COMPILER) -g -O0 $(shell python3.8-config --cflags --embed) -I$(MPI_PATH)/include -I/usr/include/readline -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed) -L$(MPI_PATH)/lib -L/usr/lib/x86_64-linux-gnu -lreadline

clean:
	rm -rf *.out __pycache__ log valid_mpi.out
