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

#include <tuple>
#include <utility>

#include "aystl/type_traits/is_specialization_of.hpp"
#include "aystl/type_traits/utils.hpp"

namespace iin {
template <typename... Ts>
struct type_list {
    static constexpr auto size = sizeof...(Ts);

    template <template <typename...> class Tmpl>
    using wrapped = wrap_tmpl_t<Tmpl, Ts...>;

    template <std::size_t pos>
    using get = decltype(std::get<pos>(std::declval<wrapped<std::tuple>>()));
};

template <typename T>
constexpr bool is_type_list_v = is_spec_of_v<T, type_list>;
template <typename T>
concept TypeListType = is_type_list_v<T>;

template <auto... Vs>
struct value_list {
    template <template <auto...> class Tmpl>
    using wrapped = Tmpl<Vs...>;

    template <template <typename...> class Tmpl>
    using type_wrapped = Tmpl<value_t<Vs>...>;
};

template <typename T>
constexpr bool is_value_list_v = is_value_spec_of_v<T, value_list>;
template <typename T>
concept ValueListType = is_value_list_v<T>;

template <auto... Vs>
using value_t_list = value_list<Vs...>::template type_wrapped<type_list>;
}

