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
#include "aystl/core/type_traits/utils/type_list.hpp"

using namespace iin;

TEST_CASE("type list") {
    using _test_list_1 = type_list<int, double, char const &>;
    CHECK(type_list<>::size() == 0);
    CHECK(_test_list_1::size() == 3);
    CHECK(is_type_list_v<type_list<>>);
    CHECK(std::is_same_v<_test_list_1::get<0>, int>);
    CHECK(std::is_same_v<_test_list_1::get<1>, double>);
    CHECK(std::is_same_v<_test_list_1::get<2>, char const &>);
}

TEST_CASE("value list") {
    CHECK(std::is_same_v<value_t_list<1, 2, 3>, type_list<value_t<1>, value_t<2>, value_t<3>>>);
    CHECK(!is_type_list_v<value_list<>>);
    CHECK(!is_value_list_v<type_list<>>);
    CHECK(is_value_list_v<value_list<>>);
}

TEST_CASE("type list cat") {
    using _test_list_1 = type_list<int, double>;
    using _test_list_11 = type_list<int, double, int, double>;
    CHECK(std::is_same_v<type_list_cat_t<_test_list_1, _test_list_1>, _test_list_11>);
}

TEST_CASE("type list slice") {
    using _test_list_1 = type_list<int, double, char, float>;
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<1, 1>>, type_list<>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<0, 1>>, type_list<int>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<0, 2>>, type_list<int, double>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<1, 2>>, type_list<double>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<1, 3>>, type_list<double, char>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<1, 4>>, type_list<double, char, float>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<3, 4>>, type_list<float>>);
    CHECK(std::is_same_v<type_list_slice_t<_test_list_1, ct_range_t<3, 3>>, type_list<>>);
}

