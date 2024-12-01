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
#include <type_traits>
#include "aystl/type_traits/utils.hpp"

using namespace iin;

TEST_CASE("type") {
    CHECK(std::is_same_v<decltype(int(any_t{})), int>);

    CHECK(std::is_same_v<type_t<int>::type, int>);
    CHECK(value_t<6>::value == 6);

    CHECK(std::is_same_v<take_off<int>::magic, int>);
    CHECK(std::is_same_v<take_off<type_t<int>>::magic, int>);
    CHECK(take_off<value_t<6>>::magic == 6);
}

TEST_CASE("type list") {
    CHECK(type_list<>::size == 0);
    CHECK(type_list<int, int>::size == 2);
    CHECK(std::is_same_v<value_t_list<1, 2, 3>, type_list<value_t<1>, value_t<2>, value_t<3>>>);
}

TEST_CASE("overload") {
    struct TO1 { int operator()(int) { return 1; } };
    struct TO2 { int operator()(double) { return 2; } };
    struct TO3 { int operator()(bool) { return 3; } };
    auto test_op = overload(TO1{}, TO2{}, TO3{});
    CHECK(test_op(0) == 1);
    CHECK(test_op(0.) == 2);
    CHECK(test_op(true) == 3);
}

TEST_CASE("is any of") {
    CHECK(is_any_of_v<std::true_type, std::true_type>);
    CHECK(is_any_of_v<std::true_type, std::false_type>);
    CHECK(is_any_of_v<std::false_type, std::true_type>);
    CHECK(!is_any_of_v<std::false_type, std::false_type>);
}

TEST_CASE("is all of") {
    CHECK(is_all_of_v<std::true_type, std::true_type>);
    CHECK(!is_all_of_v<std::true_type, std::false_type>);
    CHECK(!is_all_of_v<std::false_type, std::true_type>);
    CHECK(!is_all_of_v<std::false_type, std::false_type>);
}

namespace{
template <typename T, typename T2 = int>
struct TestTmpl {};
template <typename... Args>
using Tmpl1 = TestTmpl<Args...>;
template <typename T>
using Tmpl2 = TestTmpl<T>;
template <typename T, typename T2 = int>
struct TestTmplB {};
template <int _>
struct TestTmplC {};
}

TEST_CASE("wrap tmpl") {
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int>, TestTmpl<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int, float>, Tmpl1<int, float>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, Tmpl1<int>>, Tmpl1<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmplB, Tmpl1<int>>, TestTmplB<Tmpl1<int>>>);
}

TEST_CASE("replace tmpl args") {
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmpl<int>, float, bool>, Tmpl1<float, bool>>);
    CHECK(std::is_same_v<replace_tmpl_args_t<int, float, bool>, int>);
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmplC<1>, int>, TestTmplC<1>>);
}

TEST_CASE("replace tmpl wrapper") {
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<Tmpl1<int, bool>, TestTmplB>, TestTmplB<int, bool>>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<int, TestTmplB>, int>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<TestTmplC<1>, TestTmplB>, TestTmplC<1>>);
}

