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

#include "aystl/core/type_traits/meta/constant_list.hpp"

namespace iin {
template<typename CharT, CharT... Cs>
struct char_seq : constant_list<CharT, Cs...> {};

namespace detail {
template <typename T>
struct is_char_seq : std::false_type {};
template<typename CharT, CharT... Cs>
struct is_char_seq<char_seq<CharT, Cs...>> : std::true_type {};
}
template <typename T>
concept CharSeqType = detail::is_char_seq<T>::value;
}

