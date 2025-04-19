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

#include "aystl/tmp/meta/type.hpp"
#include "aystl/tmp/type_traits/common.hpp"
#include "aystl/tmp/type_traits/template.hpp"
#include "aystl/tmp/meta/int_seq.hpp"
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
namespace detail {
template <TypeListType T, TypeListType... Ts>
struct type_list_cat : type_t<T> {};

template <TypeListType OutT, TypeListType InT,
    template <typename...> typename Tmpl, typename... TmplArgs>
struct type_list_map;

template <TypeListType T, std::size_t pos, typename DefaultT>
struct type_list_get : type_t<DefaultT> {};

template <TypeListType InT, TypeListType MaskT,
    TypeListType OutT = type_list<>, std::size_t pos = 0>
struct type_list_filter : type_t<OutT> {};
}
template <TypeListType... Ts>
using type_list_cat_t = typename detail::type_list_cat<Ts...>::type;

template <typename... Ts>
struct type_list {
    using type = type_list;

    static constexpr std::size_t size() noexcept { return sizeof...(Ts); }

    template <template <typename...> class Tmpl>
    using wrapped = wrap_tmpl_t<Tmpl, Ts...>;

    template <template <typename...> class Tmpl, typename... TmplArgs>
    using map = typename detail::type_list_map<
        type_list<>, type, Tmpl, TmplArgs...>::type;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using at = std::tuple_element_t<pos, wrapped<std::tuple>>;
    template <std::size_t pos, typename DefaultT = null_t>
    using get = typename detail::type_list_get<type, pos, DefaultT>::type;

    template <TypeListType... _Ts>
    using concat = type_list_cat_t<type, _Ts...>;

    template <IntSeqType RangeT>
    using slice = typename RangeT::template map<at>;
    template <auto lpos, auto rpos = size()>
    using slice_range = slice<ct_range_t<
        static_cast<int>(lpos), static_cast<int>(rpos)>>;

    template <typename... _Ts> using push_back = type_list<Ts..., _Ts...>;
    template <typename... _Ts> using push_front = type_list<_Ts..., Ts...>;

    template <std::size_t pos, typename... _Ts>
    requires CtCmp<CmpOp::kLE, pos, size()>
    using insert = type_list_cat_t<
        slice_range<0, pos>, type_list<_Ts...>, slice_range<pos>>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using erase = type_list_cat_t<slice_range<0, pos>, slice_range<pos + 1>>;

    template <typename OldT, typename NewT>
    using replace = map<replace_if_same_as_t, OldT, NewT>;

    template <TypeListType MaskT>
    using filter = typename detail::type_list_filter<type, MaskT>::type;
};

namespace detail {
template <typename... Ts, typename... Ts2>
auto _concat_two_type_list(type_list<Ts...>, type_list<Ts2...>)
    -> type_list<Ts..., Ts2...>;

template <TypeListType T1, TypeListType T2, TypeListType... Ts>
struct type_list_cat<T1, T2, Ts...> {
    using type = typename type_list_cat<
        decltype(_concat_two_type_list(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};

template <typename... Ts, template <typename...> typename Tmpl>
struct type_list_map<type_list<>, type_list<Ts...>, Tmpl>
    : type_t<type_list<Tmpl<Ts>...>> {};

template <TypeListType OutT, template <typename...> typename Tmpl, typename... TmplArgs>
requires CtCmp<CmpOp::kGT, sizeof...(TmplArgs), 0>
struct type_list_map<OutT, type_list<>, Tmpl, TmplArgs...> : type_t<OutT> {};

template <typename FirstT, typename... RestTs, typename... OutTs,
    template <typename...> typename Tmpl, typename... TmplArgs>
requires CtCmp<CmpOp::kGT, sizeof...(TmplArgs), 0>
struct type_list_map<type_list<OutTs...>,
    type_list<FirstT, RestTs...>, Tmpl, TmplArgs...> {
    using _in_type  = type_list<RestTs...>;
    using _out_type = type_list<OutTs..., Tmpl<FirstT, TmplArgs...>>;

    using type = typename type_list_map<_out_type, _in_type, Tmpl, TmplArgs...>::type;
};

template <TypeListType T, std::size_t pos, typename DefaultT>
requires CtCmp<CmpOp::kLT, pos, T::size()>
struct type_list_get<T, pos, DefaultT> : type_t<typename T::template at<pos>> {};

template <TypeListType InT, TypeListType MaskT, TypeListType OutT, std::size_t pos>
requires CtCmp<CmpOp::kLT, pos, InT::size()> && CtCmp<CmpOp::kLT, pos, MaskT::size()>
struct type_list_filter<InT, MaskT, OutT, pos> {
    using _elem_type  = typename InT::template at<pos>;
    using _filt_type  = typename MaskT::template at<pos>;
    using _t_out_type = typename OutT::template push_back<_elem_type>;
    using _f_out_type = OutT;
    using _out_type   = std::conditional_t<
        static_cast<bool>(_filt_type::value), _t_out_type, _f_out_type>;

    using type = typename type_list_filter<InT, MaskT, _out_type, pos + 1>::type;
};
}
}

