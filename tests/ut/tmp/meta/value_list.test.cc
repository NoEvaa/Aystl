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

TEST_CASE("value list") {
    CHECK(std::is_same_v<value_t_list<1, 2, 3>, type_list<value_t<1>, value_t<2>, value_t<3>>>);
    CHECK(!TyListType<value_list<>>);
    CHECK(!VaListType<type_list<>>);
    CHECK(VaListType<value_list<>>);
}

