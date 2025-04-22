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

#include "aystl/tmp/meta/type.hpp"
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
template <auto... Vs>
struct value_list {
    using type = value_list;

    static constexpr index_constant<sizeof...(Vs)> size;

    template <template <auto...> class Tmpl>
    using wrapped = Tmpl<Vs...>;
    template <template <typename...> class Tmpl>
    using type_wrapped = Tmpl<value_t<Vs>...>;

    template <template <auto> class Tmpl>
    requires is_all_of_v<constant_t<bool, ValueTType<Tmpl<Vs>>>...>
    using map = value_list<Tmpl<Vs>::value...>;
    template <template <auto> class Tmpl>
    using type_map = type_list<Tmpl<Vs>...>;
    template <typename _Tp, template <auto> class Tmpl>
    requires is_all_of_v<constant_t<bool, ConstantTType<Tmpl<Vs>, _Tp>>...>
    using constant_map = constant_list<_Tp, Tmpl<Vs>::value...>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using at = typename type_wrapped<type_list>::template at<pos>;
    template <std::size_t pos>
    static constexpr auto at_v = at<pos>::value;
    template <std::size_t pos, auto _default = empty_t{}>
    using get = typename type_wrapped<type_list>::template get<pos, value_t<_default>>;
    template <std::size_t pos>
    static constexpr auto get_v = get<pos>::value;
};

template <auto... Vs>
using value_t_list = value_list<Vs...>::template type_wrapped<type_list>;
}

