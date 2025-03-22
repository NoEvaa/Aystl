/**
 * Copyright 2024 NoEvaa
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "testlib.h"
#include "aystl/utility/extern_func.hpp"

using namespace iin;

template <> auto ay_ext_func<"return 1">() -> int { return 1; };
template <> auto ay_ext_func<"return 1">() -> bool { return 1; };

template <> auto ay_ext_func<"add">(int a, int b) -> int { return a + b; };

TEST_CASE("extern function") {
    CHECK(std::is_same_v<decltype(ay_ext_func<"return 1", int>()), int>);
    CHECK(ay_ext_func<"return 1", int>() == 1);
    CHECK(std::is_same_v<decltype(ay_ext_func<"return 1", bool>()), bool>);
    CHECK(ay_ext_func<"return 1", bool>() == bool(1));

    CHECK(std::is_same_v<decltype(ay_ext_func<"return 0", int>()), int>);
    CHECK(ay_ext_func<"return 0", int>() == 0);
    CHECK(std::is_same_v<decltype(ay_ext_func<"return 0", bool>()), bool>);
    CHECK(ay_ext_func<"return 0", bool>() == bool(0));

    CHECK(ay_ext_func<"add", int>(1, 2) == 3);
}

TEST_CASE("extern lambda") {
    CHECK(std::is_same_v<decltype(iin::ext_lambda<"return 1", int>()), int>);
    CHECK(ext_lambda<"return 1", int>() == 1);
    CHECK(std::is_same_v<decltype(ext_lambda<"return 1", bool>()), bool>);
    CHECK(ext_lambda<"return 1", bool>() == bool(1));

    CHECK(std::is_same_v<decltype(ext_lambda<"return 0", int>()), int>);
    CHECK(ext_lambda<"return 0", int>() == 0);
    CHECK(std::is_same_v<decltype(ext_lambda<"return 0", bool>()), bool>);
    CHECK(ext_lambda<"return 0", bool>() == bool(0));


    CHECK(ext_lambda<"add", int, int, int>(1, 2) == 3);

    CHECK(ext_lambda_t<"return 1", int>{}() == 1);
    CHECK(ext_lambda_t<"return 0", bool>{}() == bool(0));
    CHECK(ext_lambda_t<"add", int, int, int>{}(1, 2) == 3);
}

