#pragma once

#include <stddef.h>
#include <stdint.h>

#include <stdexcept>
#include <vector>
#include <map>

namespace ct {

inline void throwing_func() {
  throw std::logic_error("some exception");
}

size_t foo(const std::vector<int32_t>&);

} // namespace ct
