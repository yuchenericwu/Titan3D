#pragma once

#include <exception>
#include <string>

namespace Exception {

  class ExceptionBase : public std::exception {
  public:
    ExceptionBase(const std::string& message) : message_(message) {}

    virtual const char* what() const _NOEXCEPT {
      return message_.c_str();
    }

  protected:
    std::string message_;
  };

  class InvalidFile : public ExceptionBase {
  public:
    InvalidFile(const std::string& message) : ExceptionBase(message) {}
  };

  class NumericBounds : public ExceptionBase {
  public:
    NumericBounds(const std::string& message) : ExceptionBase(message) {}
  };

} // namespace Exceptions