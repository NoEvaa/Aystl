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
#include "aystl/tmp/type_traits.hpp"

using namespace iin;

namespace{
template <typename T, typename T2 = int>
struct TestTmpl {};
template <typename... Ts>
using Tmpl1 = TestTmpl<Ts...>;
template <typename T>
using Tmpl2 = TestTmpl<T>;
template <typename... Ts>
struct Tmpl3 {};
template <typename T, typename T2 = int>
struct TestTmplB {};
template <int>
struct TestTmplC {};

template <auto...>
struct TestVTmpl {};
template <auto... Vs>
using TmplV1 = TestVTmpl<Vs...>;
template <auto...>
struct TmplV2 {};

template <typename T, T...>
struct TestCTmpl {};
template <typename T, T... Vs>
using TmplC1 = TestCTmpl<T, Vs...>;
template <typename T, T...>
struct TmplC2 {};
}

TEST_CASE("is spec of") {
    CHECK(is_spec_of_v<TestTmpl<int>, TestTmpl>);
    CHECK(is_spec_of_v<TestTmpl<int>, Tmpl1>);
    CHECK(!is_spec_of_v<TestTmpl<int>, Tmpl2>);

    CHECK(is_spec_of_v<Tmpl1<int>, TestTmpl>);
    CHECK(is_spec_of_v<Tmpl1<int>, Tmpl1>);
    CHECK(!is_spec_of_v<Tmpl1<int>, Tmpl2>);

    CHECK(is_spec_of_v<Tmpl2<int>, TestTmpl>);
    CHECK(is_spec_of_v<Tmpl2<int>, Tmpl1>);
    CHECK(!is_spec_of_v<Tmpl2<int>, Tmpl2>);

    CHECK(!is_spec_of_v<int, TestTmpl>);
    CHECK(!is_spec_of_v<Tmpl1<int>, TestTmplB>);
}

TEST_CASE("is value spec of") {
    CHECK(is_value_spec_of_v<TestVTmpl<1, 2, 3>, TestVTmpl>);
    CHECK(is_value_spec_of_v<TestVTmpl<1, 2, 3>, TmplV1>);
    CHECK(is_value_spec_of_v<TmplV1<1, 2, 3>, TestVTmpl>);
    CHECK(is_value_spec_of_v<TmplV1<1, 2, 3>, TmplV1>);

    CHECK(!is_value_spec_of_v<int, TestVTmpl>);
}

TEST_CASE("is constant spec of") {
    CHECK(is_constant_spec_of_v<TestCTmpl<int, 1, 2, 3>, TestCTmpl>);
    CHECK(is_constant_spec_of_v<TestCTmpl<int, 1, 2, 3>, TmplC1>);
    CHECK(is_constant_spec_of_v<TmplC1<int, 1, 2, 3>, TestCTmpl>);
    CHECK(is_constant_spec_of_v<TmplC1<int, 1, 2, 3>, TmplC1>);

    CHECK(!is_constant_spec_of_v<int, TestCTmpl>);
}

TEST_CASE("wrap tmpl") {
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int>, TestTmpl<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int, float>, Tmpl1<int, float>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, Tmpl1<int>>, Tmpl1<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmplB, Tmpl1<int>>, TestTmplB<Tmpl1<int>>>);
}

TEST_CASE("unwrap tmpl") {
    CHECK(std::is_same_v<unwrap_tmpl_t<TestTmpl<int>>, int>);
    CHECK(std::is_same_v<unwrap_tmpl_t<TestTmpl<TestTmpl<int>>>, TestTmpl<int>>);
    CHECK(std::is_same_v<unwrap_tmpl_t<int>, int>);
}

TEST_CASE("replace tmpl args") {
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmpl<int>, float, bool>, Tmpl1<float, bool>>);
    CHECK(std::is_same_v<replace_tmpl_args_t<int, float, bool>, int>);
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmplC<1>, int>, TestTmplC<1>>);

    CHECK(std::is_same_v<replace_va_tmpl_args_t<TmplV2<1>, 5, 6>, TmplV2<5, 6>>);
    CHECK(std::is_same_v<replace_co_tmpl_args_t<TmplC2<bool, false>, int, 5, 6>, TmplC2<int, 5, 6>>);
}

TEST_CASE("replace tmpl wrapper") {
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<Tmpl1<int, bool>, TestTmplB>, TestTmplB<int, bool>>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<int, TestTmplB>, int>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<TestTmplC<1>, TestTmplB>, TestTmplC<1>>);

    CHECK(std::is_same_v<replace_va_tmpl_wrapper_t<TmplV1<1, 2>, TmplV2>, TmplV2<1, 2>>);
    CHECK(std::is_same_v<replace_co_tmpl_wrapper_t<TmplC1<int, 1, 2>, TmplC2>, TmplC2<int, 1, 2>>);
}

