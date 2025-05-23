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

#include "aystl/reflect/utils.hpp"
#include "aystl/tmp/utils/ct_string.hpp"

namespace iin {
template <typename T>
constexpr auto getTypeName() noexcept {
    constexpr auto entity = detail::_getFuncSig<T>();
    constexpr auto lpos   = detail::_getFuncSigTypePrefixLen();
    constexpr auto rpos   = detail::_getFuncSigTypeSuffixLen();
    constexpr auto cnt    = entity.size() - lpos - rpos;
    static_assert(cnt >= 0);
    return ct_str_substr_v<entity, lpos, cnt>;
}
}
