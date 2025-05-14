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
#include "aystl/tmp/utils/compose_template.hpp"

using namespace iin;

namespace {
template <typename...> struct TestC1 {};
template <typename...> struct TestC2 {};
template <typename...> struct TestC3 {};
template <auto...> struct TestC4 {};
}

TEST_CASE("compose_template_t") {
    using _test_tt1 = compose_tmpl_t<
            ty_tmpl_t<TestC3>,
            ty_tmpl_t<TestC2>,
            ty_tmpl_t<TestC1>
        >;
    using _test_tt2 = compose_tmpl_t<
            ty_list_tt,
            ty_list_tt,
            ty_tmpl_t<TestC3>,
            ty_list_tt,
            ty_list_tt,
            ty_tmpl_t<TestC2>,
            ty_tmpl_t<TestC1>,
            ty_list_tt
        >;
    using _test_tt3 = compose_tmpl_t<
            ty_list_tt,
            ty_list_tt,
            ty_tmpl_t<TestC3>,
            ty_list_tt
        >;
    using _test_tt4 = compose_tmpl_t<
            ty_list_tt,
            ty_tmpl_t<TestC2>,
            ty_tmpl_t<TestC1>,
            ty_list_tt
        >;
    using _test_args_t1 = type_list<int, bool>;
    using _test_args_t2 = value_list<1, 2, 3>;
    using _test_args_t3 = value_t_list<1, 2, 3>;
    using _test_res_t1 = TestC3<TestC2<TestC1<int, bool>>>;
    CHECK(std::is_same_v<meta_wrap_t<_test_tt1, _test_args_t1>, _test_res_t1>);
    CHECK(std::is_same_v<meta_wrap_t<_test_tt2, _test_args_t1>, type_list<_test_res_t1>>);
    CHECK(std::is_same_v<meta_wrap_t<compose_tmpl_t<_test_tt3, _test_tt4>, _test_args_t1>, type_list<_test_res_t1>>);
    CHECK(std::is_same_v<meta_wrap_t<_test_tt1, _test_args_t2>,
        TestC3<TestC2<TestC1<value_t<1>, value_t<2>, value_t<3>>>>>);
    CHECK(std::is_same_v<meta_wrap_t<compose_tmpl_t<ty_tmpl_t<TestC1>, va_tmpl_t<TestC4>>, _test_args_t3>,
        TestC1<TestC4<1, 2, 3>>>);
}

