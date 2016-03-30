#include <stdio.h>
#include "Ecore.hpp"

int main(int argc, char* args[])
{
	Ecore* core = Ecore::getInstance();

	core->Start();

	Ecore::releaseInstance();

	return 0;
}
