#include <stdio.h>
#include <stdlib.h>
/* To remove python debug lib - not found error */
#ifdef _DEBUG
  #include <crtdefs.h>
  #undef _DEBUG
  #include <Python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif

#include "util/PythonScript.hpp"
#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"
#include "Ecore.hpp"

std::vector<std::string> PythonScript::path_list;
PythonScript* PythonScript::instance = nullptr;
void* PythonScript::handle = nullptr;

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
				result = (int)PyInt_AS_LONG(r);
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
		path_list.push_back(script_path);
	}
}

PythonScript::PythonScript()
{
}

PythonScript::~PythonScript()
{
}

void PythonScript::initialize()
{
	unsigned long err_code;

	if (nullptr == instance) {
		instance = new PythonScript();

		char *path;
		std::string new_path;
		Py_SetProgramName((char*)"story");
		Py_Initialize();
		LOG_DBG("GetProgramName: %s", Py_GetProgramName());

		if (Py_IsInitialized()) {
			//PySys_SetArgv(argc, argv);
			path = Py_GetPath();
			new_path = path;
			for (auto script_path : path_list) {
				//appendPath(script_path);
				if (Ecore::checkPlatform(std::string("Windows")))
					new_path.append(";");
				else
					new_path.append(":");
				new_path.append(script_path);
				LOG_DBG("   AppendPath: [%s]", script_path.c_str());
			}
			PySys_SetPath((char*)new_path.c_str());
			path = Py_GetPath();
			LOG_DBG("GetPath: [%s]", path);

			PrintMyFunc();
			LOG_DBG("10 * 15 = %d", Multiply(10, 15));
			Py_Finalize();
		}

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
	}
}

void PythonScript::finalize()
{
	IPCServer::DestroyIPC();

	if (nullptr != instance) {
		if (Py_IsInitialized())
			Py_Finalize();
		instance = nullptr;
	}
}
