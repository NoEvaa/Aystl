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
#include "aystl/core/type_traits/utils/optional.hpp"

using namespace iin;

TEST_CASE("wrap optional") {
    CHECK(std::is_same_v<wrap_optional_t<int>, std::optional<int>>);
    CHECK(std::is_same_v<wrap_optional_t<std::optional<int>>, std::optional<int>>);
}

