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

#include <concepts>
#include <cstddef>

namespace iin {
namespace detail {
constexpr bool _isPow2(auto x) {
    return !(x & (x - 1));
}
}
template <std::unsigned_integral Ty, std::size_t _len>
constexpr Ty ayBitCycle(Ty _bits) {
    static_assert(detail::_isPow2(_len));
    constexpr std::size_t _s = sizeof(Ty) * 8;
    static_assert(_s >= _len);
    if constexpr (_s == _len) {
        return _bits;
    } else {
        return ayBitCycle<Ty, _len << 1>(_bits | (_bits << _len));
    }
}
}

