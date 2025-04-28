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
#include "aystl/tmp/functional.hpp"

using namespace iin;

TEST_CASE("ct_sorted_array") {
    CHECK(std::is_same_v<ct_sorted_array<std::less<>, int, 1, 5, 2, 0>::to_constant_list,
          int_seq<int, 0, 1, 2, 5>>);
    CHECK(std::is_same_v<ct_sorted_array<std::greater<>, int, 1, 5, 2, 0>::to_constant_list,
          int_seq<int, 5, 2, 1, 0>>);
}

