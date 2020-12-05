#pragma once

#include <iostream>
#include <string>
#include <memory>


namespace story {
namespace Core {

class Util
{
public:
  Util();

  template<typename ... Args>
  static std::string format(const std::string& format, Args ... args)
  {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format.c_str(), args ...);
    return std::string(buffer.get(), buffer.get() + size - 1);
  }
};

} /* namespace Core */
} /* namespace story */
