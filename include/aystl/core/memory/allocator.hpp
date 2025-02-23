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
#pragma once

#include <cstddef>
#include <memory>

namespace iin {
template <class T>
class AyAlloc;

#if 1
template <class T>
class AyAlloc {
public:
    using value_type = T;
    using size_type  = std::size_t;
    using alloc_type = std::allocator<T>;
    using difference_type = typename alloc_type::difference_type;

    constexpr AyAlloc() noexcept = default;

    template <class U>
    constexpr AyAlloc(AyAlloc<U> const &) noexcept { }

    constexpr T * allocate(size_type _n) {
        alloc_type _a;
        return _a.allocate(_n);
    }

    constexpr void deallocate(T * _p, size_type _n) noexcept {
        alloc_type _a;
        _a.deallocate(_p, _n);
    }
};
#endif

template <class T>
using AyAllocTraits = std::allocator_traits<T>;
}

