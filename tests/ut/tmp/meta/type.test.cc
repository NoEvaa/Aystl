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
#include "aystl/tmp/meta.hpp"

using namespace iin;

TEST_CASE("base") {
    CHECK(std::is_same_v<decltype(int(any_t{})), int>);
}

TEST_CASE("type_t") {
    CHECK(std::is_same_v<type_t<int>::type, int>);

    CHECK(!TypeTType<int>);
    CHECK(TypeTType<type_t<int>>);
    CHECK(TypeTType<std::decay<int>>);
}

TEST_CASE("value_t") {
    CHECK(value_t<6>::value == 6);

    CHECK(!ValueTType<int>);
    CHECK(ValueTType<value_t<1>>);
    CHECK(ValueTType<std::is_same<int, double>>);
}

TEST_CASE("constant_t") {
    using _test_ca1 = constant_t<int, 6>;
    CHECK(std::is_same_v<_test_ca1::value_type, int>);
    CHECK(_test_ca1::value == 6);

    CHECK(ConstantTType<value_t<int(1)>, int>);
    CHECK(ConstantTType<constant_t<int, 1>, int>);
    CHECK(!ConstantTType<int, int>);

    CHECK(std::is_same_v<_test_ca1::cast<char>, constant_t<char, 6>>);

    constexpr _test_ca1 v;
    CHECK(v() == 6);
    CHECK(std::is_same_v<decltype(v()), int>);
    CHECK(char(v) == 6);
}

TEST_CASE("take off") {
    CHECK(std::is_same_v<take_off<int>::magic, int>);
    CHECK(std::is_same_v<take_off<type_t<int>>::magic, int>);
    CHECK(take_off<value_t<6>>::magic == 6);
    CHECK(take_off<constant_t<int, 6>>::magic == 6);
}

