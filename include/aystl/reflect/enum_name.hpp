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

#include "aystl/reflect/utils.hpp"

namespace iin {
template <auto _enum>
requires std::is_enum_v<decltype(_enum)>
constexpr std::string_view getEnumName() {
    constexpr auto sample = detail::_getFuncSig<0>();
    constexpr auto entity = detail::_getFuncSig<_enum>();

    constexpr std::size_t lpos = sample.find('0');
    static_assert(lpos + 1 <= sample.size());
    constexpr std::size_t rpos = sample.size() - lpos - 1;
    static_assert(lpos + rpos <= entity.size());
    constexpr std::size_t cnt = entity.size() - lpos - rpos;

    return entity.substr(lpos, cnt);
}

constexpr bool isDeclaredEnum(std::string_view enum_name) {
    return (enum_name.find('(') == std::string_view::npos);
}

template <auto _enum>
requires std::is_enum_v<decltype(_enum)>
constexpr bool is_enum_declared_v = isDeclaredEnum(getEnumName<_enum>());
}
