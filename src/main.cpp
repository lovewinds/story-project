#include <stdio.h>
#include "Ecore.h"

int main(int argc, char* args[])
{
	Ecore* core = Ecore::getInstance();

	core->Start();

	return 0;
}
