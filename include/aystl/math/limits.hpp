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

#include <cstdlib>
#include <limits>
#include <concepts>

#include "aystl/math/common.hpp"

namespace iin {
template <typename T>
class num_limits : public std::numeric_limits<T> {};

template <std::signed_integral _Ty>
class num_limits<_Ty> : public std::numeric_limits<_Ty> {
    using _type = _Ty;
    using _base = std::numeric_limits<_type>;
public:
    static constexpr _type vMax() noexcept { return (_base::max)() - 1; }
    static constexpr _type vMin() noexcept { return (_base::min)() + 2; }
    static constexpr _type vInf() noexcept { return (_base::max)(); }
    static constexpr _type vNan() noexcept { return (_base::min)(); }

    static constexpr bool isMax(_type v) noexcept { return v == vMax(); }
    static constexpr bool isMin(_type v) noexcept { return v == vMin(); }
    static constexpr bool isNan(_type v) noexcept { return v == vNan(); }
    static constexpr bool isInf(_type v) noexcept { return std::abs(v) == vInf(); }
};

template <std::unsigned_integral _Ty>
class num_limits<_Ty> : public std::numeric_limits<_Ty> {
    using _type = _Ty;
    using _base = std::numeric_limits<_type>;
public:
    static constexpr _type vMax() noexcept { return (_base::max)(); }
    static constexpr _type vMin() noexcept { return (_base::min)(); }
    static constexpr _type vInf() noexcept { return (_base::max)() - 1; }
    static constexpr _type vNan() noexcept { return (_base::max)(); }

    static constexpr bool isMax(_type v) noexcept { return v == vMax(); }
    static constexpr bool isMin(_type v) noexcept { return v == vMin(); }
    static constexpr bool isNan(_type v) noexcept { return v == vNan(); }
    static constexpr bool isInf(_type v) noexcept { return std::abs(v) == vInf(); }
};
}

