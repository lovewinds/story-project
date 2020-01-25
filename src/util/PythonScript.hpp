#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "IPCHelper.hpp"

class PythonScript
{
public:
	PythonScript();
	~PythonScript();

	static void preparePath(std::string path);
	static void initialize();
	static void finalize();

	static std::string runString(std::string cmd);

private:
	static std::vector<std::wstring> path_list;
	static PythonScript *instance;

	static bool state_ipc;

	static std::thread thread_python;
	static std::thread thread_ipc;
};
