#include <Python.h>
#include <stdio.h>
#include <mpi.h>
#include "methods.h"

static int numargs=0;
void (*foo) (int); // TODO: fixed prototype type

/* Return the number of arguments of the application command line */
static PyObject* emb_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    
    FILE *fp = fopen("write.txt", "w");
    fputs("Write to file", fp);
    fclose(fp);

    (*foo) (1);
    
    return PyLong_FromLong(numargs);
}

static PyMethodDef EmbMethods[] = {
    {"numargs", emb_numargs, METH_VARARGS,
     "Return the number of arguments received by the process."},
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