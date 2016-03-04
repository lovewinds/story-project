#include <iostream>
#include <Python.h>

void PrintMyFunc() {
	PyObject *myfunc = PyImport_ImportModule("scripts.py_test");

	if (myfunc) {
		PyObject *printHello = PyObject_GetAttrString(myfunc, "PrintMyFunc");
		if (printHello) {
			PyObject *r = PyObject_CallFunction(printHello, NULL);

			if (r == Py_None) {
				std::cout << "None is returned. \n" << std::endl;
				Py_XDECREF(r);
			}
			Py_XDECREF(printHello);
		}
		Py_XDECREF(myfunc);
	}
}

int Multiply(int dx, int dy) {
	PyObject *myfunc = PyImport_ImportModule("scripts.py_test");
	int result = 0;

	if (myfunc) {
		PyObject *multiply = PyObject_GetAttrString(myfunc, "Multiply");
		if (multiply && PyCallable_Check(multiply)) {
			PyObject *r = PyObject_CallFunction(multiply, "ii", dx, dy);
			if (r) {
				result = (int)PyInt_AS_LONG(r);
				Py_XDECREF(r);
			}
			Py_XDECREF(multiply);
		}
		Py_XDECREF(myfunc);
	}

	return result;
}

void syspath()
{
	std::cout << "sys.path:" << std::endl;
	PyRun_SimpleString("import sys\nprint sys.path\nprint\n");
}

int main(int argc, char** argv)
{
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	std::cout << "GetProgramName: " << Py_GetProgramName() << std::endl << std::endl;

	if (Py_IsInitialized()) {
		PySys_SetArgv(argc, argv);
		std::cout << "GetPath: " << Py_GetPath() << std::endl << std::endl;
		//PyRun_SimpleString("print \'Hello Python !\'\n");
		syspath();
		PrintMyFunc();
		std::cout << "10 * 15 = " << Multiply(10, 15) << std::endl << std::endl;
		Py_Finalize();
	}

	return 0;
}
