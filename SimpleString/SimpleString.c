
#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argc, char *argv[])
{

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();
    // PyRun_SimpleString("from time import time,ctime\n"
    //                    "print('Today is', ctime(time()))\n");
    
    // PyRun_SimpleString("try:\n"
    //                    "\tprint(x)\n" 
    //                    "except:\n"
    //                    "\tprint('no variable')\n");
    PyRun_SimpleString("import sys, traceback; sys.path.insert(0,'.');\n");

    PyRun_SimpleString("try:\n"
                       "    import inline\n"
                       "    inline.func()\n"
                       "except Exception as e:\n"
                       "    print('something is wrong')\n"
                       "    print(e)\n"
                       "    print('###############################')\n"
                       "    print(traceback.format_exc())\n");
    PyRun_SimpleString("if 1 == 1:\n"
                       "    print(1)\n");

    // PyRun_SimpleString("import IPython; IPython.embed();\n");

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);

    return 0;
}