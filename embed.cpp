#include <Python.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    PyObject *pName;
    PyObject *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue, *pRealArgs; 
    int i;

    char path[] = {"."};
    
    int foo [5] = {1, 2, 3, 4, 5};
    
    printf("argv0:%s\n", argv[0]);
    printf("argv1:%s\n", argv[1]);
    printf("argv2:%s\n", argv[2]);

    Py_Initialize();
    
    PySys_SetPath(path);
       
    pName = PyString_FromString(argv[1]);
    pModule = PyImport_Import(pName);
    
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */
        if (pFunc && PyCallable_Check(pFunc)) {
        	/* making a tuple */
        	
        	pRealArgs = PyTuple_New(1);
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }
            
            PyTuple_SetItem(pRealArgs, 0, pArgs);
            pValue = PyObject_CallObject(pFunc, pRealArgs);
            Py_DECREF(pArgs);
            Py_DECREF(pRealArgs);
            
            if (pValue != NULL) {
                printf("Result of call: %d\n", PyInt_AsLong(pValue));
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
