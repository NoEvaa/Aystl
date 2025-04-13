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

#include "aystl/core/type_traits/meta.hpp"
#include "aystl/core/type_traits/template.hpp"
#include "aystl/core/type_traits/utils/type_list.hpp"

namespace iin {
template <auto... Vs>
struct value_list {
    static constexpr std::size_t size() noexcept { return sizeof...(Vs); }

    template <template <auto...> class Tmpl>
    using wrapped = Tmpl<Vs...>;

    template <template <typename...> class Tmpl>
    using type_wrapped = Tmpl<value_t<Vs>...>;
};

template <typename T>
inline constexpr bool is_value_list_v = is_value_spec_of_v<T, value_list>;
template <typename T>
concept ValueListType = is_value_list_v<T>;

template <auto... Vs>
using value_t_list = value_list<Vs...>::template type_wrapped<type_list>;
}

