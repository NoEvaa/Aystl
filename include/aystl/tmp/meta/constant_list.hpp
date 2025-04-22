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

#include <functional>

#include "aystl/tmp/meta/type.hpp"
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
namespace detail {
template <ConstantListType T, ConstantListType... Ts>
struct constant_list_cat : type_t<T> {};

template <ConstantListType InT, TypeListType MaskT,
    ConstantListType OutT, std::size_t pos = 0>
struct constant_list_filter : type_t<OutT> {};
}

template <typename _Cmp, typename T, T... Vs>
struct ct_sorted_array;

template<typename T, T... Vs>
struct constant_list {
    using type       = constant_list;
    using value_type = T;

    static constexpr index_constant<sizeof...(Vs)> size;

    template <typename _Tp>
    using cast = constant_list<_Tp, static_cast<_Tp>(Vs)...>;

    template <template <typename _Tp, _Tp...> typename Tmpl>
    using wrapped = Tmpl<value_type, Vs...>;
    template <template <auto...> typename Tmpl>
    using value_wrapped = Tmpl<Vs...>;
    template <template <typename...> typename Tmpl>
    using type_wrapped = Tmpl<constant_t<value_type, Vs>...>;

    template <template <auto> class Tmpl>
    requires ConstantTType<Tmpl<value_type{}>, value_type>
    using map = constant_list<value_type, Tmpl<Vs>::value...>;
    template <template <auto> class Tmpl>
    using type_map = type_list<Tmpl<Vs>...>;
    template <template <auto> class Tmpl>
    requires ValueTType<Tmpl<value_type{}>>
    using value_map = value_list<Tmpl<Vs>::value...>;
    template <typename _Tp, template <auto> class Tmpl>
    requires ConstantTType<Tmpl<value_type{}>, _Tp>
    using constant_map = constant_list<_Tp, Tmpl<Vs>::value...>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using at = typename type_wrapped<type_list>::template at<pos>;
    template <std::size_t pos>
    static constexpr auto at_v = at<pos>::value;
    template <std::size_t pos, auto _default = empty_t{}>
    using get = typename type_wrapped<type_list>::template get<pos, value_t<_default>>;
    template <std::size_t pos>
    static constexpr auto get_v = get<pos>::value;

    template <ConstantListTType<value_type>... _Ts>
    using concat = typename detail::constant_list_cat<type, _Ts...>::type;

    template <value_type... _Vs>
    using push_back = constant_list<value_type, Vs..., _Vs...>;
    template <value_type... _Vs>
    using push_front = constant_list<value_type, _Vs..., Vs...>;

    template <TypeListType MaskT>
    using filter = typename detail::constant_list_filter<
        type, MaskT, constant_list<value_type>>::type;

    template <typename _Cmp = std::less<>>
    using sort = typename ct_sorted_array<_Cmp, value_type, Vs...>::to_constant_list;

    //using unique_sort;
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

template <ConstantListType InT, TypeListType MaskT, ConstantListType OutT, std::size_t pos>
requires CtCmp<CmpOp::kLT, pos, InT::size()> && CtCmp<CmpOp::kLT, pos, MaskT::size()>
struct constant_list_filter<InT, MaskT, OutT, pos> {
    using _elem_type  = typename InT::template at<pos>;
    using _filt_type  = typename MaskT::template at<pos>;
    using _t_out_type = typename OutT::template push_back<_elem_type::value>;
    using _f_out_type = OutT;
    using _out_type   = std::conditional_t<
        static_cast<bool>(_filt_type::value), _t_out_type, _f_out_type>;

    using type = typename constant_list_filter<InT, MaskT, _out_type, pos + 1>::type;
};
}
}

