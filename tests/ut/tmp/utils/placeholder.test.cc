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
#include "aystl/tmp/utils/placeholder.hpp"

using namespace iin;

TEST_CASE("placeholder_t") {
    CHECK(is_placeholder<plh_t<0>>::value);
    CHECK(!is_placeholder<int>::value);
    CHECK(!is_placeholder<value_t<0>>::value);

    CHECK(PlaceholderType<plh_t<0>>);
    CHECK(!PlaceholderType<value_t<0>>);

    CHECK(std::is_same_v<plh_t_list<1, 3, 5>, type_list<plh_t<1>, plh_t<3>, plh_t<5>>>);
    CHECK(std::is_same_v<make_plh_seq_t<3>, type_list<plh_t<0>, plh_t<1>, plh_t<2>>>);
}

TEST_CASE("placeholders_bind") {
    CHECK(std::is_same_v<placeholders_bind_t<type_list<>, type_list<>>, type_list<>>);
    CHECK(std::is_same_v<placeholders_bind_t<type_list<>, type_list<int>>, type_list<>>);
    CHECK(std::is_same_v<placeholders_bind_t<type_list<int>, type_list<>>, type_list<int>>);

    using _test_c1 = placeholders_bind_t<plh_t_list<1, 5, 3>, type_list<int ,char, bool>>;
    CHECK(std::is_same_v<_test_c1, type_list<int, bool, char>>);
    using _test_c2 = placeholders_bind_t<plh_t_list<1, 3, 1>, type_list<int ,char>>;
    CHECK(std::is_same_v<_test_c2, type_list<int, char, int>>);
    using _test_c3 = placeholders_bind_t<type_list<plh_t<0>, bool, plh_t<10>>, type_list<int ,char>>;
    CHECK(std::is_same_v<_test_c3, type_list<int, bool, char>>);
}

