/**
 * Copyright 2025 NoEvaa
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
#include "aystl/core/math/common.hpp"

using namespace iin;

TEST_CASE("is pow 2") {
    CHECK(detail::_isPow2(0));
    CHECK(detail::_isPow2(1));
    CHECK(detail::_isPow2(2));
    CHECK(!detail::_isPow2(3));
    CHECK(detail::_isPow2(4));
    CHECK(!detail::_isPow2(5));
    CHECK(!detail::_isPow2(6));
    CHECK(!detail::_isPow2(7));
    CHECK(detail::_isPow2(8));
    CHECK(!detail::_isPow2(9));
}

