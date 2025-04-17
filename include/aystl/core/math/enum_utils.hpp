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

#include <type_traits>

#include "aystl/core/type_traits/utils/enum.hpp"

namespace iin {
template <EnumType Ty, typename RetTy = Ty>
constexpr RetTy ayEnumAnd(Ty a, Ty b) noexcept {
    using _type = std::underlying_type_t<Ty>;
    return static_cast<RetTy>(static_cast<_type>(a) & static_cast<_type>(b));
}

template <EnumType Ty, typename RetTy = Ty>
constexpr RetTy ayEnumOr(Ty a, Ty b) noexcept {
    using _type = std::underlying_type_t<Ty>;
    return static_cast<RetTy>(static_cast<_type>(a) | static_cast<_type>(b));
}

template <EnumType Ty>
constexpr bool ayEnumFuzzyMatch(Ty _test, Ty _target) noexcept {
    return ayEnumAnd(_test, _target) == _target;
}
}

