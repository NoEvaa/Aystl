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

#ifdef _MSC_VER
#define __AY_FUNCSIG__ __FUNCSIG__
#else
#define __AY_FUNCSIG__ __PRETTY_FUNCTION__
#endif

namespace iin {
namespace detail {
template <typename T>
constexpr std::string_view _getFuncSig() {
    return __AY_FUNCSIG__;
}

template <auto _v>
constexpr std::string_view _getFuncSig() {
    return __AY_FUNCSIG__;
}
}
}
