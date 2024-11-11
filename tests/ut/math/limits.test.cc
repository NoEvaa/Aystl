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
#include "aystl/math/limits.hpp"

using namespace iin;

TEST_CASE("num limits signed int") {
    using int8_limits = num_limits<std::int8_t>;

    CHECK(int8_limits::isMax(int8_limits::vMax()));
    CHECK(int8_limits::isMin(int8_limits::vMin()));
    CHECK(int8_limits::isInf(int8_limits::vInf()));
    CHECK(int8_limits::isInf(-int8_limits::vInf()));
    CHECK(int8_limits::isNan(int8_limits::vNan()));

    CHECK(int8_limits::vMax() < int8_limits::vInf());
    CHECK(int8_limits::vMin() > -int8_limits::vInf());
}

TEST_CASE("num limits unsigned int") {}

