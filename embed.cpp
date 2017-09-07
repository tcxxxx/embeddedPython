#include <Python.h>
#include <stdio.h>
int
main(int argc, char *argv[])
{
    // char dir;
    PyObject *pName;
    // char * pName;
    PyObject *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    // PySys_SetPath(dir);    

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }
    
    printf("argv0:%s\n", argv[0]);
    printf("argv1:%s\n", argv[1]);
    printf("argv2:%s\n", argv[2]);
    printf("argv3:%s\n", argv[3]);

    Py_Initialize();
    pName = PyString_FromString(argv[1]);
    /* Error checking of pName left out */
    // pName = argv[1];
    pModule = PyImport_Import(pName);
    // pModule = PyImport_ImportModule(pName);
    
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
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