#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <mpi.h>
#include "methods.h"

static int numargs=0;
void (*foo) (int);


static PyObject* emb_NumpyArray(PyObject *self, PyObject *args)
{
	// Parse the input argument from python method
	int   gid;
	char *field_name;
	
    if(!PyArg_ParseTuple(args, "ls", &gid, &field_name)){
        return NULL;
    }

    // Create the C array to be wrapped by Numpy API and will be return
    double *output = (double *) malloc( gid * sizeof(double) );
    for (int i = 0; i < gid; i++){
    	output[i] = (double) i;
    }

    // Numpy Array
    int      nd = 1;
    npy_intp dims[1] = {gid};
    int      typenum = NPY_DOUBLE;

    PyArrayObject *numpy_array = PyArray_SimpleNewFromData(nd, dims, typenum, output);
	// To free memory as soon as the ndarray is deallocated, set the OWNDATA flag on the returned ndarray.
    PyArray_ENABLEFLAGS(numpy_array, NPY_ARRAY_OWNDATA); // This line is important.

    // Call function (*foo)
    (*foo) (gid);
    printf("field_name = %s\n", field_name);
    
    // return numpy array object
    return PyArray_Return(numpy_array);
}

static PyObject* emb_NumpyArray3D(PyObject *self, PyObject *args)
{
	// Parse the input argument from python method
	int   gid;
	char *field_name;
	
    if(!PyArg_ParseTuple(args, "ls", &gid, &field_name)){
        return NULL;
    }

    // Create the C array to be wrapped by Numpy API and will be return
    double *output = (double *) malloc( gid * gid * gid * sizeof(double) );
    for (int i = 0; i < gid * gid * gid; i++){
    	output[i] = (double) i;
    	printf("[%p] = %lf\n", &(output[i]), output[i]);
    }

    // Numpy Array
    int      nd = 3;
    npy_intp dims[3] = {gid, gid, gid};
    int      typenum = NPY_DOUBLE;

    PyArrayObject *numpy_array = PyArray_SimpleNewFromData(nd, dims, typenum, output);
	// To free memory as soon as the ndarray is deallocated, set the OWNDATA flag on the returned ndarray.
    PyArray_ENABLEFLAGS(numpy_array, NPY_ARRAY_OWNDATA); // This line is important.

    // Call function (*foo)
    (*foo) (gid);
    printf("field_name = %s\n", field_name);
    
    // return numpy array object
    return PyArray_Return(numpy_array);
}

static PyMethodDef EmbMethods[] = {
    {"numpy_array", emb_NumpyArray, METH_VARARGS,
     "Return numpy array."},
    {"numpy_array3D", emb_NumpyArray3D, METH_VARARGS,
     "Return 3D numpy array."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_emb(void)
{
    return PyModule_Create(&EmbModule);
}

int main( int argc, char *argv[]){

    MPI_Init(&argc, &argv);

	numargs = argc;

	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_SetProgramName( Py_DecodeLocale("yt_inline", NULL) );
	Py_InitializeEx( 0 );

	// Import numpy
	import_array();

	if( !Py_IsInitialized() ) {
		printf("Py_IsInitialized() == False\n");
		exit(1);
	}

   	if ( PyRun_SimpleString( "import sys; sys.path.insert(0,'.');" ) != 0 ) {
   		printf("Failed setting sys.path.\n");
   		exit(1);
   	}

   	if( PyRun_SimpleString("import gc") != 0 ) {
   		printf("Failed importing garbage collector.\n");
   		exit(1);
   	}

   	/*
   	Load and run C function dynamically
   	 */

   	foo = temp_func;

   	char *command = "import inline";
   	if (PyRun_SimpleString(command) != 0) {
		printf("%s Failed.\n", command);
		exit(1);
	}

	command = "inline.yt_inline()";
	if (PyRun_SimpleString(command) != 0) {
		printf("temp_func %s Failed.\n", command);
		exit(1);
	}

	foo = temp_func2;
	command = "inline.yt_inline()";
	if (PyRun_SimpleString(command) != 0) {
		printf("temp_func2 %s Failed.\n", command);
		exit(1);
	}

	Py_Finalize();
    MPI_Finalize();

	return 0;

}