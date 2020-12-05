#include <mpi.h>
#include <Python.h>
#include <stdlib.h>
#include "mockLIBYT.h"

int myrank;

int main(int argc, char *argv[])
{
	const char *fname = "./inline.py";
	FILE *fp;

  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  	if(create_libyt_module() != 0) {
  		printf("On rank %d, create_libyt_module failed.\n", myrank);
  		exit(0);
  	}

  	if(init_python(argc, argv) != 0) {
  		printf("On rank %d, init_python failed.\n", myrank);
  		exit(0);
  	}

  	if(init_libyt_module() != 0) {
  		printf("On rank %d, init_libyt_module failed.\n", myrank);
  		exit(0);
  	}

  	

  	// Run the python script.
  	fp = fopen(fname, "r");
  	PyRun_SimpleFile(fp, fname);

  	MPI_Finalize(); /* MPI should be finalized */
  	Py_Finalize();  /* after finalizing Python, since it's python that are using OpenMPI */

  	return 0;
}