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
#pragma once

#include <cstddef>
#include <cstdint>

namespace iin {
template <typename HashT = std::uint32_t>
HashT ayCharsHash(char const * cs, std::size_t cnt) noexcept {
    std::size_t r = cnt % 2;
    cnt          -= r;
    HashT       v = 0;
    std::size_t i = 0;
    for ( ; i < cnt; ) {
        // v * 31 + c
        v = (v << 5) - v + static_cast<HashT>(cs[i++]);
        v = (v << 5) - v + static_cast<HashT>(cs[i++]);
    }
    if (r) {
        v = (v << 5) - v + static_cast<HashT>(cs[i]);
    }
    return v;
}

template <typename HashT = std::uint32_t, std::size_t N>
HashT ayCharsHash(char const (&cs)[N]) noexcept {
    return ayCharsHash<HashT>(cs, N);
}
}
