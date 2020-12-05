#include <mpi.h>
#include <Python.h>
#include "mockLIBYT.h"


int main(int argc, char *argv[])
{
	// file
	const char *fname = "./inline.py";
	FILE *fp;

  	MPI_Init(&argc, &argv);
  	Py_SetProgramName(Py_DecodeLocale("yt_inline", NULL));
  	Py_Initialize();

  	fp = fopen(fname, "r");
  	PyRun_SimpleFile(fp, fname);

  	MPI_Finalize(); /* MPI should be finalized */
  	Py_Finalize();  /* after finalizing Python, since it's python that are using OpenMPI */

  	return 0;
}