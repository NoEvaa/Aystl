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
#include "aystl/tmp/utils/currying_template.hpp"

using namespace iin;

TEST_CASE("currying_template_t") {
    using _test_c1 = currying_tmpl_t<ty_list_tt, type_list<>>;
    CHECK(CurryingTmplType<_test_c1>);
    CHECK(!CurryingTmplType<int>);
    CHECK(MetaTmplType<_test_c1>);
    CHECK(!MetaPrimTmplType<_test_c1>);

    using _test_c2 = currying_tmpl_t<ty_list_tt, type_list<int, char>>;
    CHECK(std::is_same_v<_test_c1::wrap<>, type_list<>>);
    CHECK(std::is_same_v<_test_c2::wrap<>, type_list<int, char>>);

    using _test_c3 = currying_tmpl_t<ty_list_tt, make_plh_seq_t<3>>;
    CHECK(std::is_same_v<_test_c3::xbind<int>::xwrap<char, bool>, type_list<int, char, bool>>);
    CHECK(std::is_same_v<_test_c3::xbind<int, char>::xwrap<bool>, type_list<int, char, bool>>);
    CHECK(std::is_same_v<_test_c3::xbind<plh_t<10>, char>::xwrap<bool, int>, type_list<int, char, bool>>);
    CHECK(std::is_same_v<_test_c3::change_args<type_list<plh_t<10>, char>>::xwrap<bool, int>, type_list<bool, char>>);
    CHECK(std::is_same_v<_test_c3::change_tmpl<va_list_tt>::wrap<value_t_list<1, 3, 5>>, value_list<1, 3, 5>>);
}

