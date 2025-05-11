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
#include "aystl/tmp.hpp"

using namespace iin;

TEST_CASE("type list") {
    using _test_list_1 = type_list<int, double, char>;
    CHECK(type_list<>::size() == 0);
    CHECK(_test_list_1::size() == 3);
    CHECK(TyListType<type_list<>>);
    CHECK(!TyListType<int>);
}

TEST_CASE("type list concat") {
    using _test_list_1 = type_list<int, double>;
    using _test_list_11 = type_list<int, double, int, double>;
    using _test_list_111 = type_list<int, double, int, double, int, double>;
    CHECK(std::is_same_v<type_list_cat_t<_test_list_1, _test_list_1>, _test_list_11>);
    CHECK(std::is_same_v<_test_list_1::concat<_test_list_1>, _test_list_11>);
    CHECK(std::is_same_v<_test_list_1::concat<_test_list_1, _test_list_1>, _test_list_111>);
}

TEST_CASE("type list push front & back") {
    using _test_list_1 = type_list<float, double>;

    CHECK(std::is_same_v<_test_list_1::push_front<int, char>, type_list<int, char, float, double>>);
    CHECK(std::is_same_v<_test_list_1::push_back<int, char>, type_list<float, double, int, char>>);
}

TEST_CASE("type list at") {
    using _test_list_1 = type_list<int, double, char const &>;
    CHECK(std::is_same_v<_test_list_1::at<0>, int>);
    CHECK(std::is_same_v<_test_list_1::at<1>, double>);
    CHECK(std::is_same_v<_test_list_1::at<2>, char const &>);
}

TEST_CASE("type list wrapped & map") {
    using _test_list_1 = type_list<int, double, char>;
    using _test_list_2 = type_list<value_t<1>, value_t<2>, value_t<3>>;

    CHECK(std::is_same_v<_test_list_1::wrapped<ty_tmpl_t<std::tuple>>, std::tuple<int, double, char>>);

    CHECK(std::is_same_v<_test_list_1::map<ty_tmpl_t<type_t>>,
          type_list<type_t<int>, type_t<double>, type_t<char>>>);
    CHECK(std::is_same_v<_test_list_1::map<ty_tmpl_t<std::tuple>, float, bool>,
          type_list<std::tuple<int, float, bool>, std::tuple<double, float, bool>, std::tuple<char, float, bool>>>);

    CHECK(std::is_same_v<_test_list_1::ty_map<ty_tmpl_t<type_t>>,
          type_list<type_t<int>, type_t<double>, type_t<char>>>);
    CHECK(std::is_same_v<_test_list_2::ty_map<va_tmpl_t<value_t>>,
          type_list<value_t<1>, value_t<2>, value_t<3>>>);
    CHECK(std::is_same_v<_test_list_2::ty_map<co_tmpl_t<constant_t>>,
          type_list<constant_t<int, 1>, constant_t<int, 2>, constant_t<int, 3>>>);

    CHECK(std::is_same_v<_test_list_2::va_map<ty_tmpl_t<transfer_value_t>>, value_list<1, 2, 3>>);
    CHECK(std::is_same_v<_test_list_2::co_map<ty_tmpl_t<transfer_value_t>, int>, constant_list<int, 1, 2, 3>>);
}

TEST_CASE("type list slice") {
    using _test_list_1 = type_list<int, double, char, float>;
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<1, 1>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<0, 1>>, type_list<int>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<0, 2>>, type_list<int, double>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<1, 2>>, type_list<double>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<1, 3>>, type_list<double, char>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<1, 4>>, type_list<double, char, float>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<3, 4>>, type_list<float>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<3, 3>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::slice<ct_range_t<5, 9>>, type_list<>>);

    CHECK(std::is_same_v<_test_list_1::slice_range<1, 1>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<0, 1>, type_list<int>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<0, 2>, type_list<int, double>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<1, 2>, type_list<double>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<1, 4>, type_list<double, char, float>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<3, 4>, type_list<float>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<3, 3>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<0>, _test_list_1>);
    CHECK(std::is_same_v<_test_list_1::slice_range<2>, type_list<char, float>>);
    CHECK(std::is_same_v<_test_list_1::slice_range<4>, type_list<>>);
}

TEST_CASE("type list insert") {
    using _test_list_1 = type_list<float, double>;

    CHECK(std::is_same_v<_test_list_1::insert<0, int, char>, type_list<int, char, float, double>>);
    CHECK(std::is_same_v<_test_list_1::insert<1, int, char>, type_list<float, int, char, double>>);
    CHECK(std::is_same_v<_test_list_1::insert<2, int, char>, type_list<float, double, int, char>>);
}

TEST_CASE("type list erase") {
    using _test_list_1 = type_list<int, double, char>;

    CHECK(std::is_same_v<_test_list_1::erase<0>, type_list<double, char>>);
    CHECK(std::is_same_v<_test_list_1::erase<1>, type_list<int, char>>);
    CHECK(std::is_same_v<_test_list_1::erase<2>, type_list<int, double>>);
}

TEST_CASE("type list replace") {
    using _test_list_1 = type_list<int, double, int, char>;

    CHECK(std::is_same_v<_test_list_1::replace<double, float>, type_list<int, float, int, char>>);
    CHECK(std::is_same_v<_test_list_1::replace<int, bool>, type_list<bool, double, bool, char>>);

    CHECK(std::is_same_v<type_list<int, char, int>::replace<int, bool>,
          type_list<bool, char, bool>>);
}

TEST_CASE("type list filter") {
    using _test_list_1 = type_list<int, bool, char>;

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true>>, type_list<int>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, false>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false>>, type_list<int>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true>>, type_list<int, bool>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true>>, type_list<bool>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, false, false>>, type_list<>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false, false>>, type_list<int>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true, false>>, type_list<bool>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, false, true>>, type_list<char>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, false>>, type_list<int, bool>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, false, true>>, type_list<int, char>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<false, true, true>>, type_list<bool, char>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, true>>, type_list<int, bool, char>>);

    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, true, true>>, type_list<int, bool, char>>);
    CHECK(std::is_same_v<_test_list_1::mask_filter<value_t_list<true, true, false, true>>, type_list<int, bool>>);
}

