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

#include "aystl/type_traits/compare.hpp"
#include "aystl/type_traits/utils.hpp"
#include "aystl/type_traits/is_specialization_of.hpp"

namespace iin {
template <typename... Ts>
struct type_list {
    static constexpr std::size_t size() noexcept { return sizeof...(Ts); }

    template <template <typename...> class Tmpl>
    using wrapped = wrap_tmpl_t<Tmpl, Ts...>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using get = std::tuple_element_t<pos, wrapped<std::tuple>>;
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

namespace detail {
template <typename... Ts, typename... Ts2>
auto _concat_two_type_list(type_list<Ts...>, type_list<Ts2...>)
    -> type_list<Ts..., Ts2...>;

template <TypeListType T, TypeListType... Ts>
struct type_list_cat : type_t<T> {};

template <TypeListType T1, TypeListType T2, TypeListType... Ts>
struct type_list_cat<T1, T2, Ts...> {
    using type = typename type_list_cat<
        decltype(_concat_two_type_list(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};

template <TypeListType T, std::integral IntT, IntT... Is>
auto _type_list_slice(int_seq<IntT, Is...>)
    -> type_list<typename T::template get<static_cast<std::size_t>(Is)>...>; 
}
template <TypeListType... Ts>
using type_list_cat_t = typename detail::type_list_cat<Ts...>::type;

template <TypeListType T, IntSeqType RangeT>
using type_list_slice_t = decltype(detail::_type_list_slice<T>(std::declval<RangeT>()));
}

