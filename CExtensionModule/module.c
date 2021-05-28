#include <Python.h>
#include <stdio.h>

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

// Holds information about the methods in Python C extension Module.
// An array of PyMethodDef structure.
static PyMethodDef FputsMethods[] = {
    //{ FuncName in python, Corresponding C function, Tells Python accept arguments, Doc string for the method} 
	{"fputs", method_fputs, METH_VARARGS, "Python interface for fputs C library function."},
	{NULL, NULL, 0, NULL}
};

// Holds information about the module itself.
// A single structure that's used for module definition.
static struct PyModuleDef fputsmodule = {
	PyModuleDef_HEAD_INIT,
	"fputs", // Name of Python C extension module
	"Python interface for fputs C library function.", // Doc string for the module
	-1,
	FputsMethods // Reference to the method table.
};

// PyMODINIT_FUNC
// 1. It implicitly sets the return type of the function as PyObject*.
// 2. It declares any special linkages.
// 3. It declares the function as extern “C.” In case you’re using C++, 
//    it tells the C++ compiler not to do name-mangling on the symbols.
PyMODINIT_FUNC PyInit_fputs( void ){

	// (1) Assign module value
	PyObject *module = PyModule_Create( &fputsmodule );

	// (2) Initialize new exception object
	// Example:
	// Traceback (most recent call last):
	//   File "<stdin>", line 1, in <module>
	// fputs.StringTooShortError: String length must be greater than 10
	StringTooShortError = PyErr_NewException("fputs.StringTooShortError", NULL, NULL);

	// Add exception object to your module
	// The name added here will show inside dir(fputs)
	PyModule_AddObject(module, "StringTooShortError1", StringTooShortError);

	// (3) Add constant
	PyModule_AddIntConstant(module, "FPUTS_FLAG", 64);

	return module;
}
