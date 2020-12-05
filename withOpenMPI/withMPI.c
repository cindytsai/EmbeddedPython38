#include <mpi.h>
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "mockLIBYT.h"

int myrank;

int main(int argc, char *argv[])
{
	/*
	Run a .py file directly.
	 */
	const char *full_fname = "./inline.py";
	FILE *fp;
	
	const char *fname = "inline";

  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  	if(create_libyt_module() != 0) {
  		printf("On rank %d, create_libyt_module failed.\n", myrank);
  		exit(1);
  	}

  	if(init_python(argc, argv) != 0) {
  		printf("On rank %d, init_python failed.\n", myrank);
  		exit(1);
  	}

  	if(init_libyt_module(fname) != 0) {
  		printf("On rank %d, init_libyt_module failed.\n", myrank);
  		exit(1);
  	}

  	

	/*
	Run a .py file directly.
	 */
  	fp = fopen(full_fname, "r");
  	PyRun_SimpleFile(fp, full_fname);

  	MPI_Finalize(); /* MPI should be finalized */
  	Py_Finalize();  /* after finalizing Python, since it's python that are using OpenMPI */

  	return 0;
}