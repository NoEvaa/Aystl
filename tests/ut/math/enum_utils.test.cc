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
#include "aystl/core/math/enum_utils.hpp"

using namespace testlib;
using namespace iin;

TEST_CASE("enum and") {
    CHECK(ayEnumAnd(TestEnum::k0, TestEnum::k1) == TestEnum::k0);
    CHECK(ayEnumAnd(TestEnum::k1, TestEnum::k2) == TestEnum::k0);
    CHECK(ayEnumAnd(TestEnum::k2, TestEnum::k4) == TestEnum::k0);
    CHECK(ayEnumAnd(TestEnum::k1, TestEnum::k3) == TestEnum::k1);
    CHECK(ayEnumAnd(TestEnum::k3, TestEnum::k1) == TestEnum::k1);
}

TEST_CASE("enum or") {
    CHECK(ayEnumOr(TestEnum::k0, TestEnum::k1) == TestEnum::k1);
    CHECK(ayEnumOr(TestEnum::k1, TestEnum::k2) == TestEnum::k3);
    CHECK(ayEnumOr(TestEnum::k2, TestEnum::k4) == TestEnum::k6);
    CHECK(ayEnumOr(TestEnum::k1, TestEnum::k3) == TestEnum::k3);
    CHECK(ayEnumOr(TestEnum::k3, TestEnum::k1) == TestEnum::k3);
    CHECK(ayEnumOr(TestEnum::k1, TestEnum::k1) == TestEnum::k1);
}

TEST_CASE("enum fuzzy match") {
    CHECK(ayEnumFuzzyMatch(TestEnum::k0, TestEnum::k0));
    CHECK(ayEnumFuzzyMatch(TestEnum::k1, TestEnum::k1));
    CHECK(ayEnumFuzzyMatch(TestEnum::k3, TestEnum::k1));
    CHECK(!ayEnumFuzzyMatch(TestEnum::k1, TestEnum::k3));
    CHECK(!ayEnumFuzzyMatch(TestEnum::k1, TestEnum::k2));
    CHECK(!ayEnumFuzzyMatch(TestEnum::k2, TestEnum::k1));
}

