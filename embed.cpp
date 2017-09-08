#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 32
#define LENGTH 32
#define HEIGHT 32

int
main(int argc, char *argv[])
{
	/* PyObjects */
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pRealArgs; 

    int i; // 
    /* For generating random number */
    srand(time(NULL));    

    // char path[] = {"."};
    char module[] = {"modules.array"};
    char function[] = {"func"};   

    int array [LENGTH * WIDTH * HEIGHT];
    int array_size = LENGTH * WIDTH * HEIGHT;
    int array_num = 1;

    Py_Initialize();
    /* setting $PYTHONPATH */
    //'PySys_SetPath(path)' gives rise to new problems
    PyRun_SimpleString(
        "import sys\n"
		"sys.path.append('.')\n"
    );  
    pName = PyString_FromString(module);
    pModule = PyImport_Import(pName);
    
    Py_DECREF(pName);
    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, function);
        /* pFunc is a new reference */
        if (pFunc && PyCallable_Check(pFunc)) {
            /* making a tuple */	
            pRealArgs = PyTuple_New(array_num);
            pArgs = PyList_New(array_size);
            for (i = 0; i < array_size; ++i) {
				array[i] = rand() % 256;
                pValue = PyInt_FromLong(array[i]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                PyList_SetItem(pArgs, i, pValue);
            }
            PyTuple_SetItem(pRealArgs, 0, pArgs);

            pValue = PyObject_CallObject(pFunc, pRealArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pRealArgs);
            
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    Py_Finalize();
    return 0;
}
