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
#include "aystl/math/bit_utils.hpp"

using namespace iin;

TEST_CASE("bit cycle") {
    CHECK(ayBitCycle<std::uint8_t, 1>(0b0) == 0);
    CHECK(ayBitCycle<std::uint8_t, 1>(0b1) == 0xff);
    CHECK(ayBitCycle<std::uint8_t, 2>(0b01) == 0x55);
    CHECK(ayBitCycle<std::uint8_t, 2>(0b10) == 0xaa);
    CHECK(ayBitCycle<std::uint8_t, 4>(0b0000) == 0);
    CHECK(ayBitCycle<std::uint8_t, 4>(0b0110) == 0x66);
    CHECK(ayBitCycle<std::uint16_t, 4>(0x6) == 0x6666);
    CHECK(ayBitCycle<std::uint32_t, 8>(0x1a) == 0x1a1a1a1a);
}

