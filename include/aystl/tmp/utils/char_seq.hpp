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

#include "aystl/tmp/meta.hpp"

namespace iin {
template<typename CharT, CharT... Cs>
requires is_any_same_of_v<CharT, char, wchar_t,
    char8_t, char16_t, char32_t>
using char_seq = constant_list<CharT, Cs...>;

template <typename T>
concept CharSeqType = is_constant_spec_of_v<T, char_seq>;
}

