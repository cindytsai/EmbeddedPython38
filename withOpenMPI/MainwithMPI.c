#include <mpi.h>
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include "mockLIBYT.h"

int myrank;
int randomData(int SEED, int MAX, int MIN, int *array, int array_len);

int main(int argc, char *argv[])
{
	/*
	Run a .py file directly.
	 */
	// const char *full_fname = "./inline.py";
	// FILE *fp;
	
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

  	// Generate random length array
  	int *array;
  	int array_len = 10;
  	array = (int*) malloc(array_len * sizeof(int));
  	if ( randomData(100, 100, 0, array, array_len) != 0) {
  		printf("On rank %d, randomData() failed.\n", myrank);
  		exit(1);
  	}

  	// Update the libyt python module __dict__
  	// if(yt_set_parameter("prop1", array, array_len) != 0) {
  	// 	printf("On rank %d, yt_set_parameter('prop1', array, array_len) \n", myrank);
  	// 	exit(1);
  	// }

    // Append dictionary
    int num = 3;
    struct yt_field *field_list = malloc( num * sizeof(*field_list));

    for (int i = 0; i < num; i++){
      // field_list[i].field_name = "Dens";
      field_list[i].field_define_type = "cell-centered";
    }

    char a[] = "FIELD_NAME";
    char b[10] = "B_FIELD";

    field_list[0].field_name = "Dens";
    field_list[1].field_name = a;
    field_list[2].field_name = b;

    // if(yt_set_field_list("prop3", num, &field_list) != 0) {
    //   printf("On rank %d, yt_set_field_list('prop3', num, field_list) \n", myrank);
    //   exit(1);
    // }

  	// Run def inline() in "fname".py python script
  	if(yt_inline(fname) != 0) {
  		printf("On rank %d, yt_inline(%s) failed \n", myrank, fname);
  		exit(1);
  	}

	/*
	Run a .py file directly.
	 */
  	// fp = fopen(full_fname, "r");
  	// PyRun_SimpleFile(fp, full_fname);

    Py_Finalize();  /* since it's python that are using OpenMPI */
  	MPI_Finalize(); /* MPI should be finalized before finalizing python*/
  	

  	return 0;
}

int randomData(int SEED, int MAX, int MIN, int *array, int array_len) {
	
	int num;

	srand(SEED + myrank);

	for(int i = 0; i < array_len; i = i+1) {
		num = rand() % (MAX + 1 - MIN) + MIN;
		array[i] = num;
		// printf("num = %d\n", num);
	}
	
	return 0;
}

