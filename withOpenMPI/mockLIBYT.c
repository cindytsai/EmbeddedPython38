#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

extern int myrank;
PyObject *libytSub_module = NULL;
PyObject *libytSub_module_dict = NULL;
PyObject *libytSub_prop1, *libytSub_prop2, *libytSub_prop3;

typedef struct yt_field
{
	char *field_name;
	char *field_define_type;
};

// New Custom Error, will initialize in PyInit_fputs
static PyObject *StringTooShortError = NULL;

// C extended python method
static PyObject* method_fputs(PyObject *self, PyObject *args){
	char *str, *filename = NULL;
	int bytes_copied = -1;

	// Parse Arguments
	if ( !PyArg_ParseTuple(args, "ss", &str, &filename) ){
		return NULL; // return NULL to indicate failure.
	}

	// Raising Custom Exception in python C extension module,
	// even though we can't raise exception in C.
	if ( strlen(str) < 10 ){
		PyErr_SetString(StringTooShortError, "String length must be greater than 10.");
		return NULL; // return NULL to indicate failure.
	}

	FILE *fp = fopen(filename, "w");
	bytes_copied = fputs(str, fp);
	fclose(fp);

	// The variable to be returned when the function is invoked in python
	// You must return a PyObject* from you python C extension module 
	// back to the python interpreter.
	return PyLong_FromLong(bytes_copied);
}

static PyMethodDef libyt_method_list[] = {
	{"fputs", method_fputs, METH_VARARGS, "Python interface for fputs C library function."},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef libyt_module_definition = {
	PyModuleDef_HEAD_INIT,
	"libyt",
	"libyt documentation",
	-1,
	libyt_method_list,
	NULL, NULL, NULL, NULL
};

static PyMethodDef libytSub_method_list[] = {
	{NULL, NULL, NULL, NULL}
};

static struct PyModuleDef libytSub_module_definition = {
	PyModuleDef_HEAD_INIT,
	"libytSub",
	"libyt submodule documentation",
	-1,
	libytSub_method_list,
	NULL, NULL, NULL, NULL
};

static PyObject* PyInit_libyt(void) {

	// Create module
	PyObject *module = PyModule_Create( &libyt_module_definition );

	if (module == NULL){
		return NULL;
	}

	// Add custom exception
	StringTooShortError = PyErr_NewException("libyt.StringTooShortError", NULL, NULL);
	PyModule_AddObject(module, "StringTooShortError1", StringTooShortError);

	// Add Constant
	PyModule_AddIntConstant(module, "LIBYT_FLAG", 64);

	// Add dictionary
	libytSub_prop1 = PyDict_New();
	libytSub_prop2 = PyDict_New();
	libytSub_prop3 = PyDict_New();
	PyModule_AddObject(module, "prop1", libytSub_prop1);
	PyModule_AddObject(module, "prop2", libytSub_prop2);
	PyModule_AddObject(module, "prop3", libytSub_prop3);
	
	// Add submodule 
	libytSub_module = PyModule_Create( &libytSub_module_definition );
	PyModule_AddObject(module, "libytSub", libytSub_module);

	return module;
}

int create_libyt_module() {
	PyImport_AppendInittab("libyt", &PyInit_libyt);
	return 0;
}

int init_libyt_module(const char *fname) {


	// Check if successfully create libyt python module
	// if((libytSub_module = PyImport_AddModule("libyt.libytSub")) == NULL) {
	// 	printf("On rank %d, libyt_module = PyImport_AddModule('libyt')) == NULL\n", myrank);
	// 	exit(1);
	// }

	// // Check if we can get the dictionary from libyt python module
	// if((libytSub_module_dict = PyModule_GetDict(libytSub_module)) == NULL) {
	// 	printf("On rank %d, libyt_module_dict = PyModule_GetDict(libyt_module)) == NULL\n", myrank);
	// 	exit(1);
	// }

	// // Create properties and parameters for libyt python module

	// libytSub_prop1 = PyDict_New();
	// libytSub_prop2 = PyDict_New();
	// libytSub_prop3 = PyDict_New();

	// PyDict_SetItemString(libytSub_module_dict, "prop1", libytSub_prop1);
	// PyDict_SetItemString(libytSub_module_dict, "prop2", libytSub_prop2);
	// PyDict_SetItemString(libytSub_module_dict, "prop3", libytSub_prop3);

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

int yt_set_parameter(const char *prop_name, const int *data_array, const int data_array_len) {

	// // Check that libyt_module_dict is type dictionary
	// if(!PyDict_Check(libytSub_module_dict)) {
	// 	printf("On rank %d, PyDict_Check(libyt_module_dict) != true\n", myrank);
	// 	exit(1);
	// }

	// // Check that libyt __dict__ has key "prop_name"
	// if(!PyDict_Contains(libytSub_module_dict, Py_DecodeLocale(prop_name, NULL))) {
	// 	printf("On rank %d, No key %s in libyt_module_dict\n", myrank, prop_name);
	// 	exit(1);
	// }

	// Convert data_array to python list
	PyObject *list_val = PyList_New(0);
	PyObject *item;
	for (int i = 0; i < data_array_len; i = i+1){
		item = PyLong_FromLong(data_array[i]);
		if (PyList_Append(list_val, item) != 0) {
			printf("On rank %d, list_val.append(%d) failed\n", myrank, data_array[i]);
			exit(1);
		}
	}

	// Update dictionary
	if(PyDict_SetItemString(libytSub_prop1, prop_name, list_val) != 0) {
		printf("On rank %d, Update dict failed.\n", myrank);
		exit(1);
	}

	return 0;
}

int yt_set_field_list(const char *prop_name, int num, struct yt_field **field_list) {

	PyObject *dict_val = PyDict_New();
	PyObject *key, *val;

	// Set items for dictionary dict_val
	for (int i = 0; i < num; i++){
		key = PyUnicode_FromString((*field_list)[i].field_name); // after python3.5
		val = PyUnicode_FromString((*field_list)[i].field_define_type);
		if( PyDict_SetItem(dict_val, key, val) != 0){
			printf("On rank %d, add key:val pair to dict_val failed\n", myrank);
			exit(1);
		}
	}

	if(PyDict_SetItemString(libytSub_module_dict, prop_name, dict_val) != 0) {
		printf("On rank %d, Update dict failed.\n", myrank);
		exit(1);
	}

	Py_DECREF(dict_val);
	Py_DECREF(key);
	Py_DECREF(val);

	return 0;
}

int yt_inline(const char *fname) {

	// Run def yt_inline() in script
	const int command_width = 13 + strlen( fname );   // 13 = ".yt_inline()" + '\0'
	char *command = (char*) malloc(command_width * sizeof(char));
	sprintf(command, "%s.yt_inline()", fname);
	if (PyRun_SimpleString(command) != 0) {
		printf("On rank %d, %s Failed.\n", myrank, command);
		exit(1);
	}
	free(command);
	
	return 0;
}

