#include "testlib.h"
#include "aystl/type_traits/optional.hpp"

using namespace iin;

TEST_CASE("wrap optional") {
    CHECK(std::is_same_v<wrap_optional_t<int>, std::optional<int>>);
    CHECK(std::is_same_v<wrap_optional_t<std::optional<int>>, std::optional<int>>);
}

