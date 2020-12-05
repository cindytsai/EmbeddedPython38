#include <string.h>
#include <Python.h>
#include <stdlib.h>
#include <mpi.h>

extern int myrank;


static PyMethodDef libyt_method_list[] = {
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef libyt_module_definition = {
	PyModuleDef_HEAD_INIT,
	"libyt",
	"libyt documentation",
	-1,
	libyt_method_list
};

static PyObject* PyInit_libyt(void) {
	return PyModule_Create(&libyt_module_definition);
}

int create_libyt_module() {
	PyImport_AppendInittab("libyt", &PyInit_libyt);
	return 0;
}

int init_libyt_module(const char *fname) {
	PyObject *libyt_module = NULL;
	PyObject *libyt_module_dict = NULL;

	// Check if successfully create libyt python module
	if((libyt_module = PyImport_AddModule("libyt")) == NULL) {
		printf("On rank %d, libyt_module = PyImport_AddModule('libyt')) == NULL\n", myrank);
		exit(1);
	}

	// Check if we can get the dictionary from libyt python module
	if((libyt_module_dict = PyModule_GetDict(libyt_module)) == NULL) {
		printf("On rank %d, libyt_module_dict = PyModule_GetDict(libyt_module)) == NULL\n", myrank);
		exit(1);
	}

	// Create properties and parameters for libyt python module
	PyObject *libyt_prop1, *libyt_prop2, *libyt_prop3;
	libyt_prop1 = PyDict_New();
	libyt_prop2 = PyDict_New();
	libyt_prop3 = PyDict_New();

	PyDict_SetItemString(libyt_module_dict, "prop1", libyt_prop1);
	PyDict_SetItemString(libyt_module_dict, "prop2", libyt_prop2);
	PyDict_SetItemString(libyt_module_dict, "prop3", libyt_prop3);

	// Load full "fname".py script
	const int command_width = 8 + strlen( fname );   // 8 = "import " + '\0'
	char *command = (char*) malloc(command_width * sizeof(char));
	sprintf(command, "import %s", fname);
	if (PyRun_SimpleString(command) != 0) {
		printf("On rank %d, %s Failed.\n", myrank, command);
		exit(1);
	}
	free(command);

	return 0;
}

int init_python(int argc, char *argv[]) {
	Py_SetProgramName( Py_DecodeLocale("yt_inline", NULL) );
	Py_InitializeEx( 0 );

	if( !Py_IsInitialized() ) {
		printf("On rank %d, Py_IsInitialized() == False\n", myrank);
		exit(1);
	}

	/*
	Can't use argc, argv on Ubuntu20.04, it reaches error.
	 */
   	// wchar_t **wchar_t_argv = (wchar_t **) malloc(argc * sizeof(wchar_t *));
   	// wchar_t wchar_temp[1000];
   	// for (int i = 0; i < argc; i = i+1) {
   	// 	printf("On rank %d, argc = %d, argv[%d] = %s\n", myrank, i, i, argv[i]);
    //   	mbtowc(wchar_temp, argv[i], 1000);
    //   	wchar_t_argv[i] = wchar_temp;
   	// }
   	// PySys_SetArgv( argc, wchar_t_argv );

   	if ( PyRun_SimpleString( "import sys; sys.path.insert(0,'.');" ) != 0 ) {
   		printf("On rank %d, Failed setting sys.path.\n", myrank);
   		exit(1);
   	}

   	if( PyRun_SimpleString("import gc") != 0 ) {
   		printf("On rank %d, Failed importing garbage collector.\n", myrank);
   		exit(1);
   	}

   	return 0;
}

// int libyt_set_parameter(int *data_array, int len) {

// 	return 0;
// }

