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
#include "aystl/tmp.hpp"

using namespace iin;

using test_seq_0 = constant_list<int>;
using test_seq_1 = constant_list<int, 1, 2, 3>;
using test_seq_2 = constant_list<int, 7, 8, 9>;
using test_seq_3 = constant_list<std::size_t, 7, 8, 9>;

using test_seq_1_2 = constant_list<int, 1, 2, 3, 7, 8, 9>;
using test_seq_3_3 = constant_list<std::size_t, 7, 8, 9, 7, 8, 9>;

TEST_CASE("constant list") {
    CHECK(CoListType<test_seq_0>);
    CHECK(CoListType<test_seq_1>);
    CHECK(CoListType<test_seq_3>);
    CHECK(!CoListType<int>);
    CHECK(!CoListType<type_list<>>);
    CHECK(!CoListType<value_list<>>);

    CHECK(CoListTType<test_seq_1, int>);
    CHECK(CoListTType<test_seq_3, std::size_t>);
    CHECK(!CoListTType<test_seq_3, int>);
}

TEST_CASE("constant list cast") {
    CHECK(std::is_same_v<test_seq_2::cast<std::size_t>, test_seq_3>);
}

template <int _v>
struct _test_pow : value_t<_v * _v> {};
template <int _v>
struct _test_to_char : constant_t<char, _v> {};

TEST_CASE("constant list wrapped & map") {
    CHECK(std::is_same_v<test_seq_1::wrapped<va_list_tt>, value_list<1, 2, 3>>);
    
    CHECK(std::is_same_v<test_seq_1::map<va_tmpl_t<_test_pow>>, constant_list<int, 1, 4, 9>>);
    CHECK(std::is_same_v<test_seq_1::co_map<va_tmpl_t<_test_to_char>, char>, constant_list<char, 1, 2, 3>>);
    CHECK(std::is_same_v<test_seq_1::ty_map<co_tmpl_t<constant_t>>,
          type_list<constant_t<int, 1>, constant_t<int, 2>, constant_t<int, 3>>>);
    CHECK(std::is_same_v<test_seq_1::va_map<va_tmpl_t<_test_pow>>, value_list<1, 4, 9>>);
}

TEST_CASE("constant list at & get") {
    CHECK(std::is_same_v<test_seq_2::at<0>, constant_t<int, 7>>);
    CHECK(std::is_same_v<test_seq_2::at<1>, constant_t<int, 8>>);
    CHECK(std::is_same_v<test_seq_2::at<2>, constant_t<int, 9>>);
    
    CHECK(std::is_same_v<test_seq_2::get<0>, constant_t<int, 7>>);
    CHECK(std::is_same_v<test_seq_2::get<2>, constant_t<int, 9>>);
    CHECK(std::is_same_v<test_seq_2::get<3>, null_t>);
    CHECK(std::is_same_v<test_seq_2::get<10>, null_t>);
}

TEST_CASE("constant list concat") {
    CHECK(std::is_same_v<test_seq_0::concat<test_seq_1>, test_seq_1>);
    CHECK(std::is_same_v<test_seq_1::concat<test_seq_2>, test_seq_1_2>);
    CHECK(std::is_same_v<test_seq_3::concat<test_seq_3>, test_seq_3_3>);
}

TEST_CASE("constant list filter") {
    using _test_list_1 = constant_list<int, 1, 2, 3>;

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<>>, constant_list<int>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false>>, constant_list<int>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true>>, constant_list<int, 1>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false>>, constant_list<int, 1>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true>>, constant_list<int, 2>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true>>, constant_list<int, 1, 2>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, false, false>>, constant_list<int>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false, false>>, constant_list<int, 1>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true, false>>, constant_list<int, 2>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, false, true>>, constant_list<int, 3>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, false>>, constant_list<int, 1, 2>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false, true>>, constant_list<int, 1, 3>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true, true>>, constant_list<int, 2, 3>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, true>>, constant_list<int, 1, 2, 3>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, true, true>>, constant_list<int, 1, 2, 3>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, false, true>>, constant_list<int, 1, 2>>);
}

TEST_CASE("constant apply algo") {
    using _test_seq_1 = constant_list<int, 3, 1, 5, 7>;

    CHECK(std::is_same_v<_test_seq_1::apply_algo<detail::ct_std_reverse_t>, constant_list<int, 7, 5, 1, 3>>);
    CHECK(std::is_same_v<_test_seq_1::apply_algo<detail::ct_std_sort_t<std::less<>>>, constant_list<int, 1, 3, 5, 7>>);
}

TEST_CASE("constant list sort") {
    using _test_seq_1 = constant_list<int, 2, 5, 1, -1, 0>;
    using _test_seq_2 = constant_list<int, 2, 1, 2, 0, 0, 1, -1>;

    CHECK(std::is_same_v<_test_seq_1::sort<>, constant_list<int, -1, 0, 1, 2, 5>>);
    CHECK(std::is_same_v<_test_seq_1::sort<std::greater<>>, constant_list<int, 5, 2, 1, 0, -1>>);
    CHECK(std::is_same_v<_test_seq_2::unique_sort<>, constant_list<int, -1, 0, 1, 2>>);
    CHECK(std::is_same_v<_test_seq_2::unique_sort<std::greater<>>, constant_list<int, 2, 1, 0, -1>>);
}

