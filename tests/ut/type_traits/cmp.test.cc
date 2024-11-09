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
#include "aystl/type_traits/cmp.hpp"

using namespace iin;

TEST_CASE("ct cmp EQ") {
    constexpr auto op = CmpOp::kEQ;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp NE") {
    constexpr auto op = CmpOp::kNE;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp LT") {
    constexpr auto op = CmpOp::kLT;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp LE") {
    constexpr auto op = CmpOp::kLE;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp GT") {
    constexpr auto op = CmpOp::kGT;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp GE") {
    constexpr auto op = CmpOp::kGE;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

