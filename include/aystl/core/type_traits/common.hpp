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
#include <utility>

namespace iin {
template <class... Args>
using is_any_of = std::disjunction<Args...>;

template <class... Args>
inline constexpr bool is_any_of_v = is_any_of<Args...>::value;

template <class... Args>
using is_all_of = std::conjunction<Args...>;

template <class... Args>
inline constexpr bool is_all_of_v = is_all_of<Args...>::value;

template <typename T, typename... Ts>
inline constexpr bool is_any_same_of_v = is_any_of_v<std::is_same<T, Ts>...>;

namespace detail {
template <typename T>
constexpr bool is_type_v = std::is_void_v<std::void_t<T>>;
}

template <typename T>
using add_clref_t = std::add_lvalue_reference_t<std::add_const_t<T>>;
}

