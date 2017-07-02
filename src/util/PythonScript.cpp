#include "util/PythonScript.hpp"
#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"
#include "Ecore.hpp"

std::vector<std::wstring> PythonScript::path_list;
PythonScript* PythonScript::instance = nullptr;
void* PythonScript::handle = nullptr;

#define __PYTHON_AVAILABLE
#ifdef __PYTHON_AVAILABLE
#include <stdio.h>
#include <stdlib.h>
/* To remove python debug lib - not found error */
#ifdef _DEBUG
  //#include <crtdefs.h>
  #undef _DEBUG
  #if defined(PLATFORM_IOS)
  	#include <Python/Python.h>
  #else
  	#include <Python.h>
  #endif
  #define _DEBUG
#else
  #if defined(PLATFORM_IOS)
  	#include <Python/Python.h>
  #else
  	#include <Python.h>
  #endif
#endif

void PrintMyFunc() {
	//PyObject *module = PyImport_ImportModule("scripts.story");
	PyObject *module = PyImport_ImportModule("story");

	if (module) {
		PyObject *klass = PyObject_GetAttrString(module, "SpLog");
		if (klass) {
			//PyObject *instance = PyInstance_New(printHello, NULL, NULL);
			PyObject *instance = PyObject_CallObject(klass, NULL);
			if (instance) {
				PyObject *r = PyObject_CallMethod(instance, "dbg", "()");

				if (r == Py_None) {
					LOG_DBG("None is returned.");
					Py_XDECREF(r);
				}
				Py_XDECREF(instance);
			}
			Py_XDECREF(klass);
		} else {
			LOG_ERR("Object was not found");
		}
		Py_XDECREF(module);
	} else {
		LOG_ERR("Failed to import module");
	}
}

int Multiply(int dx, int dy) {
	PyObject *myfunc = PyImport_ImportModule("scripts._story");
	int result = -1;

	if (myfunc) {
		PyObject *multiply = PyObject_GetAttrString(myfunc, "Multiply");
		if (multiply && PyCallable_Check(multiply)) {
			PyObject *r = PyObject_CallFunction(multiply, (char*)"ii", dx, dy);
			if (r) {
				result = (int)PyLong_AsLong(r);
				Py_XDECREF(r);
			}
			Py_XDECREF(multiply);
		} else {
			LOG_ERR("Object was not found");
		}
		Py_XDECREF(myfunc);
	} else {
		LOG_ERR("Failed to import module");
	}

	return result;
}

void PythonScript::addPath(std::string script_path)
{
	if (!script_path.empty()) {
		std::wstring ws;
		ws.assign(script_path.begin(), script_path.end());
		path_list.push_back(ws);
	}
}

/**********************************************************************
 * Module Start
 **********************************************************************/
static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject*
emb_numargs(PyObject *self, PyObject *args)
{
	if(!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
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

static PyObject*
PyInit_emb(void)
{
    return PyModule_Create(&EmbModule);
}

/**********************************************************************
 * Module End
 **********************************************************************/

void PythonScript::initialize()
{
	unsigned long err_code;

	if (nullptr == instance) {
		instance = new PythonScript();

		wchar_t *path;
		std::wstring new_path;

		Py_SetProgramName(L"story");

		numargs = 8;
		PyImport_AppendInittab("emb", &PyInit_emb);

		Py_Initialize();
		LOG_DBG("GetProgramName: %ls", Py_GetProgramName());

		if (Py_IsInitialized()) {
			//PySys_SetArgv(argc, argv);
			path = Py_GetPath();
			new_path = path;
			for (auto script_path : path_list) {
				//appendPath(script_path);
				if (Ecore::checkPlatform(std::string("Windows")))
					new_path.append(L";");
				else
					new_path.append(L":");
				new_path.append(script_path);
				LOG_DBG("   AppendPath: [%ls]", script_path.c_str());
			}
			PySys_SetPath((wchar_t*)new_path.c_str());
			path = Py_GetPath();
			LOG_DBG("GetPath: [%ls]", path);

			PyRun_SimpleString(
				/*"from time import time, ctime\n"
				"print('Today is', ctime(time()))\n"*/
				"import sys\n"
				"import emb\n"
				"import story\n"
				"print(sys.path)\n"
				"story.SpLog()\n"
				"print('Number of arguments', emb.numargs())\n"
			);

			PrintMyFunc();
			LOG_DBG("10 * 15 = %d", Multiply(10, 15));

			Py_Finalize();
		}
#if 0
		handle = IPCServer::CreateIPC();
		if (handle) {
			char pData[1024] = {0,};
			size_t tDataSize = 1023;
			unsigned long ret;

			LOG_DBG("Waiting for data receiving");
			ret = IPCServer::RecvIPC(handle, pData, tDataSize);
			LOG_DBG("Received data [%lu]", ret);
		} else {
			LOG_ERR("Failed to initialize IPC");
		}
#endif
	}
}

void PythonScript::finalize()
{
//	IPCServer::DestroyIPC();

	if (nullptr != instance) {
		if (Py_IsInitialized())
			Py_Finalize();
		instance = nullptr;
	}
}
#else

void PythonScript::initialize()
{
	LOG_DBG("Cannot use Python");
}

void PythonScript::finalize()
{
	LOG_DBG("Cannot use Python");
}

void PythonScript::addPath(std::string script_path)
{
	if (!script_path.empty()) {
		path_list.push_back(script_path);
	}
}

#endif

PythonScript::PythonScript()
{
}

PythonScript::~PythonScript()
{
}
