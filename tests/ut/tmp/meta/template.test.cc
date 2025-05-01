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

using namespace iin;

namespace{
template <typename... Ts>
struct Tmpl1 {};

template <auto...>
struct TmplV1 {};

template <typename T, T...>
struct TmplC1 {};
}

TEST_CASE("template_t wrap") {
    CHECK(std::is_same_v<ty_wrap_t<ty_list_tt, int, bool>, type_list<int, bool>>);
    CHECK(std::is_same_v<ty_wrap_t<va_list_tt, value_t<1>, value_t<2>>, value_list<1, 2>>);
    //CHECK(std::is_same_v<ty_wrap_t<co_list_tt, int, value_t<1>, value_t<2>>, constant_list<int, 1, 2>>);

    CHECK(std::is_same_v<va_wrap_t<ty_list_tt, 1, 2>, type_list<value_t<1>, value_t<2>>>);
    CHECK(std::is_same_v<va_wrap_t<va_list_tt, 1, 2>, value_list<1, 2>>);

    CHECK(std::is_same_v<co_wrap_t<ty_list_tt, int, 1, 2>, type_list<constant_t<int, 1>, constant_t<int, 2>>>);
    CHECK(std::is_same_v<co_wrap_t<va_list_tt, int, 1, 2>, value_list<1, 2>>);
    CHECK(std::is_same_v<co_wrap_t<co_list_tt, int, 1, 2>, constant_list<int, 1, 2>>);

    CHECK(std::is_same_v<meta_wrap_t<ty_list_tt, type_list<int, bool>>, type_list<int, bool>>);
    CHECK(std::is_same_v<meta_wrap_t<va_list_tt, value_list<1, 2>>, value_list<1, 2>>);
    CHECK(std::is_same_v<meta_wrap_t<co_list_tt, constant_list<int, 1, 2>>, constant_list<int, 1, 2>>);
    //CHECK(std::is_same_v<meta_wrap_t<co_list_tt, type_list<int, value_t<1>, value_t<2>>>, constant_list<int, 1, 2>>);
}

TEST_CASE("template_t rewrapped") {
    CHECK(std::is_same_v<meta_rewrapped_t<Tmpl1<int, bool>, ty_list_tt>, type_list<int, bool>>);
    CHECK(std::is_same_v<meta_rewrapped_t<TmplV1<1, 2>, va_list_tt>, value_list<1, 2>>);
    CHECK(std::is_same_v<meta_rewrapped_t<TmplC1<int, 1, 2>, co_list_tt>, constant_list<int, 1, 2>>);
}

TEST_CASE("template_t rewrap") {
    CHECK(std::is_same_v<meta_rewrap_t<Tmpl1<int, bool>, type_list<bool, int>>, Tmpl1<bool, int>>);
    CHECK(std::is_same_v<meta_rewrap_t<TmplV1<1, 2>, value_list<9, 6>>, TmplV1<9, 6>>);
    CHECK(std::is_same_v<meta_rewrap_t<TmplC1<int, 1, 2>, constant_list<bool, true, false>>, TmplC1<bool, true, false>>);
}

