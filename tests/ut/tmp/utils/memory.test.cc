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
#include "aystl/tmp/utils/memory.hpp"

using namespace iin;

TEST_CASE("remove_smart_ptr") {
    CHECK(std::is_same_v<remove_smart_ptr_t<int>, int>);
    CHECK(std::is_same_v<remove_smart_ptr_t<std::shared_ptr<int>>, int>);
    CHECK(std::is_same_v<remove_smart_ptr_t<std::unique_ptr<int>>, int>);
    CHECK(std::is_same_v<remove_smart_ptr_t<std::weak_ptr<int const>>, int const>);
}

