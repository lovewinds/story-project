#include <stdio.h>
#include "core/Ecore.hpp"

int main(int argc, char* argv[])
{
  story::Core::Ecore* core = story::Core::Ecore::getInstance();

  core->Start();

  story::Core::Ecore::releaseInstance();

  return 0;
}
