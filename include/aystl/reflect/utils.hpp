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

#include <string_view>

#include "aystl/global/common.h"
#include "aystl/utility/ct_string.hpp"

namespace iin {
namespace detail {
template <typename T>
consteval auto _getFuncSig() noexcept { return ct_str_v<AY_FUNCSIG>; }

constexpr std::size_t _getFuncSigTypePrefixLen() noexcept {
    constexpr auto s = _getFuncSig<void>();
    auto sv = std::string_view(s);
    return sv.find("void");
}
constexpr std::size_t _getFuncSigTypeSuffixLen() noexcept {
    return _getFuncSig<void>().size() - _getFuncSigTypePrefixLen() - 4U;
}

template <auto _v>
consteval auto _getFuncSig() noexcept { return ct_str_v<AY_FUNCSIG>; }

constexpr std::size_t _getFuncSigValuePrefixLen() noexcept {
    constexpr auto s = _getFuncSig<0>();
    auto sv = std::string_view(s);
    return sv.find("0");
}
constexpr std::size_t _getFuncSigValueSuffixLen() noexcept {
    return _getFuncSig<0>().size() - _getFuncSigValuePrefixLen() - 1U;
}
}
}
