/**
 * Copyright 2025 NoEvaa
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

#include "aystl/tmp/meta/type.hpp"

namespace iin {
namespace detail {
template <ConstantListType T, ConstantListType... Ts>
struct constant_list_cat : type_t<T> {};
}

template<typename T, T... Vs>
struct constant_list {
    using type       = constant_list;
    using value_type = T;

    static constexpr std::size_t size() noexcept { return sizeof...(Vs); }

    template <typename _Tp>
    using cast = constant_list<_Tp, static_cast<_Tp>(Vs)...>;

    template <template <auto...> class Tmpl>
    using wrapped = Tmpl<Vs...>;

    template <template <auto> class Tmpl>
    requires ConstantTType<Tmpl<value_type{}>, value_type>
    using map = constant_list<value_type, Tmpl<Vs>::value...>;

    template <template <auto> class Tmpl>
    using type_map = type_list<Tmpl<Vs>...>;

    template <ConstantListTType<value_type>... _Ts>
    using concat = typename detail::constant_list_cat<type, _Ts...>::type;
};

namespace detail {
template <typename T, T... Vs1, T... Vs2>
auto _concat_two_constant_list(constant_list<T, Vs1...>, constant_list<T, Vs2...>)
    -> constant_list<T, Vs1..., Vs2...>;

template <ConstantListType T1, ConstantListType T2, ConstantListType... Ts>
struct constant_list_cat<T1, T2, Ts...> {
    using type = typename constant_list_cat<
        decltype(_concat_two_constant_list(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};
}
}

