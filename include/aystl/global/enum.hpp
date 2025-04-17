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

#include "aystl/core/type_traits/utils/enum.hpp"

namespace iin {
template <EnumClassType T>
inline constexpr T operator|(T lhs, T rhs) {
    using _utype = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<_utype>(lhs) | static_cast<_utype>(rhs));
}

template <EnumClassType T>
inline constexpr T operator&(T lhs, T rhs) {
    using _utype = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<_utype>(lhs) & static_cast<_utype>(rhs));
}

template <EnumClassType T>
inline constexpr T operator^(T lhs, T rhs) {                                  \
    using _utype = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<_utype>(lhs) ^ static_cast<_utype>(rhs));
}                                                                                                  \

template <EnumClassType T>
inline constexpr T operator~(T e) {
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(e));
}
}

