#pragma once

#include "Exceptions.h"

#include <string>

namespace ads {

class KeyNotFoundException : public std::exception {
public:
  explicit KeyNotFoundException(std::string keyAsString)
      : errorReasonWhat_("KeyNotFound: " + std::move(keyAsString)) {}

  const char *what() const noexcept { return errorReasonWhat_.c_str(); }

private:
  std::string errorReasonWhat_;
};

} // namespace ads
