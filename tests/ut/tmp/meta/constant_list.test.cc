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
#include "aystl/tmp/meta.hpp"
#include "aystl/tmp/utils/ct_sorted_array.hpp"

using namespace iin;

using test_seq_0 = constant_list<int>;
using test_seq_1 = constant_list<int, 1, 2, 3>;
using test_seq_2 = constant_list<int, 7, 8, 9>;
using test_seq_3 = constant_list<std::size_t, 7, 8, 9>;

using test_seq_1_2 = constant_list<int, 1, 2, 3, 7, 8, 9>;
using test_seq_3_3 = constant_list<std::size_t, 7, 8, 9, 7, 8, 9>;

TEST_CASE("constant list") {
    CHECK(ConstantListType<test_seq_0>);
    CHECK(ConstantListType<test_seq_1>);
    CHECK(ConstantListType<test_seq_3>);
    CHECK(!ConstantListType<int>);
    CHECK(!ConstantListType<type_list<>>);
    CHECK(!ConstantListType<value_list<>>);

    CHECK(ConstantListTType<test_seq_1, int>);
    CHECK(ConstantListTType<test_seq_3, std::size_t>);
    CHECK(!ConstantListTType<test_seq_3, int>);
}

TEST_CASE("constant list cast") {
    CHECK(std::is_same_v<test_seq_2::cast<std::size_t>, test_seq_3>);
}

template <int _v>
using _test_pow = value_t<_v * _v>;
template <int _v>
using _test_to_char = constant_t<char, _v>;

TEST_CASE("constant list wrapped & map") {
    CHECK(std::is_same_v<test_seq_1::wrapped<value_list>, value_list<1, 2, 3>>);

    CHECK(std::is_same_v<test_seq_1::map<_test_pow>, constant_list<int, 1, 4, 9>>);
    CHECK(std::is_same_v<test_seq_1::type_map<value_t>, type_list<value_t<1>, value_t<2>, value_t<3>>>);
    CHECK(std::is_same_v<test_seq_1::value_map<_test_pow>, value_list<1, 4, 9>>);
    CHECK(std::is_same_v<test_seq_1::constant_map<char, _test_to_char>, constant_list<char, 1, 2, 3>>);
}

TEST_CASE("constant list concat") {
    CHECK(std::is_same_v<test_seq_0::concat<test_seq_1>, test_seq_1>);
    CHECK(std::is_same_v<test_seq_1::concat<test_seq_2>, test_seq_1_2>);
    CHECK(std::is_same_v<test_seq_3::concat<test_seq_3>, test_seq_3_3>);
}

TEST_CASE("constant list sort") {
    using _test_seq_1 = constant_list<int, 2, 5, 1, -1, 0>;

    CHECK(std::is_same_v<_test_seq_1::sort<>, constant_list<int, -1, 0, 1, 2, 5>>);
    CHECK(std::is_same_v<_test_seq_1::sort<std::greater<>>, constant_list<int, 5, 2, 1, 0, -1>>);
}

