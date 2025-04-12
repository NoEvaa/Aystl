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
#include "aystl/core/type_traits/meta.hpp"

using namespace iin;

TEST_CASE("type") {
    CHECK(std::is_same_v<decltype(int(any_t{})), int>);

    CHECK(std::is_same_v<type_t<int>::type, int>);
    CHECK(value_t<6>::value == 6);

    CHECK(!TypeTType<int>);
    CHECK(TypeTType<type_t<int>>);
    CHECK(TypeTType<std::decay<int>>);
    CHECK(!ValueTType<int>);
    CHECK(ValueTType<value_t<1>>);
    CHECK(ValueTType<std::is_same<int, double>>);
    CHECK(ConstantTType<value_t<int(1)>, int>);
    CHECK(ConstantTType<constant_t<int, 1>, int>);
    CHECK(!ConstantTType<int, int>);

    CHECK(std::is_same_v<take_off<int>::magic, int>);
    CHECK(std::is_same_v<take_off<type_t<int>>::magic, int>);
    CHECK(take_off<value_t<6>>::magic == 6);
    CHECK(take_off<constant_t<int, 6>>::magic == 6);
}

