#include <stdio.h>
#include <readline.h>
#include <history.h>
#include <Python.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

	/* initialize */
	int mpi_rank, mpi_size;
	int root = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	Py_Initialize();

	/* create prompt interface. */
	char ps1[] = ">>> ";
	char ps2[] = "... ";
	char *prompt = ps1;
	bool done = false;

	/* parsing code and error msg */
	int input_len, code_len;
	char *err_msg, *input_line, *code = NULL;

	/* get current builtins. 
	 * [TODO] We might not need this, since we are working inside module's namespace. */
	PyObject *local_var, *global_var;
	local_var = PyDict_New();
	global_var = PyDict_New();
	PyDict_SetItemString(global_var, "__builtins__", PyEval_GetBuiltins());

	while(!done) {
		/* read code in root */
		if (mpi_rank == root) {

			input_line = readline(prompt);
            
			if (input_line == NULL) {
				done = true;
			}
			else {
				// get input line length
				input_len = strlen(input_line);
				if (input_len > 0) add_history(input_line);
				
				// get code length
				if (code == NULL)  code_len = 0;
				else               code_len = strlen(code);

				// reallocate code for input line
				code = (char*) realloc(code, input_len + code_len + 2);
				if (code == NULL) exit(1);

				// append input line to code
				if (code_len == 0) code[0] = '\0';
				strncat(code, input_line, input_len);
				code[code_len + input_len] = '\n';
				code[code_len + input_len + 1] = '\0';

				// compile to check if this code is complete yet.
				PyObject *src = Py_CompileString(code, "<stdin>", Py_single_input); //TODO: [NewRef]

				if (src != NULL) {                                    // compiled works fine
					if (prompt == ps1 || code[code_len + input_len - 1] == '\n') {
						// broadcast to other ranks
						int temp = (int) strlen(code); // TODO: truncate to int first
						MPI_Bcast(&temp, 1, MPI_INT, root, MPI_COMM_WORLD);
						MPI_Bcast(code, strlen(code), MPI_CHAR, root, MPI_COMM_WORLD);

						// execute code
						PyObject *dum = PyEval_EvalCode(src, global_var, local_var);

						// print error
						if (PyErr_Occurred()) PyErr_Print();

						// clean up
						Py_XDECREF(dum);
						free(code);
						code = NULL;
						prompt = ps1;
					}
				}
				else if (PyErr_ExceptionMatches(PyExc_SyntaxError)) { // code might not comlete yet
					// save current exception if there is any
					PyObject *exc, *val, *traceback, *obj;
					PyErr_Fetch(&exc, &val, &traceback);

					// compare to error msg that indicates user hasn't done input yet.
					PyArg_ParseTuple(val, "sO", &err_msg, &obj);

					if ( strcmp(err_msg, "unexpected EOF while parsing") == 0 ) {
						Py_XDECREF(exc);
						Py_XDECREF(val);
						Py_XDECREF(traceback);

						prompt = ps2;
					}

					// it's a real error
					else {
						PyErr_Restore(exc, val, traceback);
						PyErr_Print();

						// clean up
						free(code);
						code = NULL;
						prompt = ps1;
					}
				}
				else { // real error in code
					// clean up
					PyErr_Print();
					free(code);
					code = NULL;
					prompt = ps1;
				}

				// clean up
				free(input_line);
				Py_XDECREF(src);
			}

		}
		/* For non-root ranks, receive broadcast from root and execute code. */
		else {
			// get code length
			int code_len;
			MPI_Bcast(&code_len, 1, MPI_INT, root, MPI_COMM_WORLD);

			// get code
			code = (char*) malloc(code_len * sizeof(char));
			MPI_Bcast(code, code_len, MPI_CHAR, root, MPI_COMM_WORLD);

			// compile and execute code
			PyObject *src = Py_CompileString(code, "<libyt-root>", Py_single_input);
			PyObject *dum = PyEval_EvalCode(src, global_var, local_var);
			if (PyErr_Occurred()) {
				PyErr_Print();
			}

			// clean up
			free(code);
			Py_XDECREF(src);
			Py_XDECREF(dum);
		}
	}

	// clean up
	Py_XDECREF(global_var);
	Py_XDECREF(local_var);

	/* finalize */
	Py_Finalize();
	MPI_Finalize();

	return 0;
}
