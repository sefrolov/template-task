#pragma once

#include <set>
#include <stdexcept>

namespace ct {

inline void throwing_func() {
  throw std::logic_error("some exception");
}

} // namespace ct
