#pragma once

#include <iostream>
#include <string>
#include <vector>

class PythonScript
{
public:
	PythonScript();
	~PythonScript();

	static void addPath(std::string path);
	static void initialize();
	static void finalize();

private:
	static std::vector<std::wstring> path_list;
	static PythonScript *instance;
	static void *handle;
};
