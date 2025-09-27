#include "solution.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ct_test {

TEST_CASE("ABI") {
  REQUIRE_THROWS_AS(ct::throwing_func(), std::logic_error);
}

TEST_CASE("Vector test") {
  std::vector<int32_t> a;
  REQUIRE(a.size() == 0);
}

} // namespace ct_test
