#include "util/PythonScript.hpp"
#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"
#include "core/Ecore.hpp"



std::vector<std::wstring> PythonScript::path_list;
PythonScript* PythonScript::instance = nullptr;

// bool PythonScript::state_ipc = true;
// std::thread PythonScript::thread_python;
// std::thread PythonScript::thread_ipc;
// static void thread_loop_ipc(bool& running_state);

#define __PYTHON_AVAILABLE
#ifdef __PYTHON_AVAILABLE
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <stdio.h>
#include <stdlib.h>
/* To remove python debug lib - not found error */
#ifdef _DEBUG
//#include <crtdefs.h>
#undef _DEBUG
  #include <Python.h>
#define _DEBUG
#else
  #include <Python.h>
#endif


// A friendly class.
class hello
{
public:
  hello(const std::string& country) { this->country = country; }
  std::string greet() const {
    LOG_INFO("Hello from %s", country.c_str());
    return "Hello from " + country;
  }
private:
  std::string country;
};

// A function taking a hello object as an argument.
std::string invite(const hello& w) {
  return w.greet() + "! Please come soon!";
}

class ScriptHost
{
public:
  ScriptHost() {
    this->core = story::Core::Ecore::getInstance();
  }
  std::string getBasePath() const {
    return core->getBasePath();
  }
private:
  story::Core::Ecore* core;
};

namespace py = pybind11;
using namespace py::literals;
/**********************************************************************
 * Module Start
 **********************************************************************/
PYBIND11_EMBEDDED_MODULE(pyemb_test, m) {
    py::class_<hello>(m, "hello")
      .def(py::init<std::string>())
      .def(py::init([](std::string s) {
        return new hello(s);
      }))
      // .def_property_readonly("greet", &hello::greet)
      .def("greet", &hello::greet)
      .def("add", [](const hello& self, int i, int j) { return i + j; })
      .def("invite", invite);
}

PYBIND11_EMBEDDED_MODULE(scriptHostModule, m) {
    py::class_<ScriptHost>(m, "scriptHost")
      .def(py::init<>())
      .def_property_readonly("getBasePath", &ScriptHost::getBasePath);
}
/**********************************************************************
 * Module End
 **********************************************************************/

void PythonScript::preparePath(std::string script_path)
{
  if (!script_path.empty()) {
    std::wstring ws;
    ws.assign(script_path.begin(), script_path.end());
    path_list.push_back(ws);
  }
}


void PythonScript::initialize()
{
  py::scoped_interpreter guard{};

  // Use only for custom defined exceptions
  // py::register_exception_translator([](std::exception_ptr p) {
  //   try {
  //     if (p) std::rethrow_exception(p);
  //   } catch (const pybind11::error_already_set &pe) {
  //     LOG_ERR("PYTHON error :: %s", pe.what());
  //     PyErr_SetString(PyExc_RuntimeError, pe.what());
  //   }
  // });

  try {
    auto pyemb_test = py::module::import("pyemb_test");
    auto obj = pyemb_test.attr("hello")("Korean");
    auto message = obj.attr("greet")().cast<std::string>();
    LOG_ERR("From Python :: %s", message.c_str());

    auto val = obj.attr("add")(3, 7).cast<int>();
    LOG_ERR("From Python :: %d", val);

    message = obj.attr("invite")().cast<std::string>();
    LOG_ERR("From Python :: %s", message.c_str());

    // append source dir to sys.path, and python interpreter would find your custom python file
    py::module_ sys = py::module_::import("sys");
    py::list path = sys.attr("path");
    for (auto p : path_list) {
      path.attr("append")(p);
    }

    message = "Path :: {}"_s.format(path);
    LOG_ERR("From Python :: %s", message.c_str());

    // import custom python class and call it
    py::module_ tokenize = py::module_::import("entry_point");
    py::type scriptEntryPointClass = tokenize.attr("ScriptEntryPoint");
    py::object scriptEntryPoint = scriptEntryPointClass();
    py::object res = scriptEntryPoint.attr("check")();
    message = res.cast<std::string>();
    LOG_ERR("From Python :: %s", message.c_str());
  }
  catch(std::exception &e) {
    LOG_ERR("PYTHON error :: %s", e.what());
  }
}

void PythonScript::finalize()
{
  // LOG_INFO("Wait for IPC thread termination..");
  // state_ipc = false;
  // if (thread_ipc.joinable())
  //   thread_ipc.join();
  // LOG_INFO("  Done");
}

std::string PythonScript::runString(std::string cmd)
{
  std::string result;
  if (nullptr != instance) {
    LOG_DBG("Command :");
    LOG_DBG("%s", cmd.c_str());
    PyRun_SimpleString(cmd.c_str());
  } else {
    result = std::string("Not initialized yet !");
  }

  return result;
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

void PythonScript::preparePath(std::string script_path)
{
  if (!script_path.empty()) {
    std::wstring ws;
    ws.assign(script_path.begin(), script_path.end());
    path_list.push_back(ws);
  }
}

std::string PythonScript::runString(std::string cmd)
{
  LOG_DBG("Not supported");
  return std::string("Not supported");
}

#endif

PythonScript::PythonScript()
{
  IPCServer ipc_server22;
}

PythonScript::~PythonScript()
{
}

/**********************************************************************
 * Threading 
 **********************************************************************/

// static void thread_loop_ipc(bool& running_state)
// {
//   IPCServer ipc_server;
//   void *handle = ipc_server.CreateIPC();
//   if (nullptr == handle) {
//     LOG_ERR("Failed to initialize IPC");
//     return;
//   }

//   char pData[1024] = {0,};
//   size_t tDataSize = 1023;
//   unsigned long ret;

//   // with blocking manner
//   while(running_state) {
//     // LOG_DBG("Waiting for data receiving");
//     ret = ipc_server.RecvIPC(handle, pData, tDataSize);
//     // LOG_DBG("Received data [%lu]", ret);

//     std::this_thread::sleep_for(std::chrono::seconds(1));
//   }

//   // On terminate
//   ipc_server.DestroyIPC();
// }

/**********************************************************************
 * Threading end
 **********************************************************************/
