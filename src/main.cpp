#include <iostream>
#include "core/Ecore.hpp"
#include "util/LogHelper.hpp"

int main(int argc, char* argv[])
{
  story::Core::Ecore* core = story::Core::Ecore::getInstance();

  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg.compare("--debug") == 0) {
      Log::setLogLevel(DBUG.value);
    } else {
      Log::setLogLevel(INFO.value);
    }
  }

  core->Start();

  story::Core::Ecore::releaseInstance();

  return 0;
}
