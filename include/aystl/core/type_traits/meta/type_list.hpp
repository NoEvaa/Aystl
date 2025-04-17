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

#include "aystl/core/type_traits/common.hpp"
#include "aystl/core/type_traits/meta/meta.hpp"
#include "aystl/core/type_traits/meta/template.hpp"
#include "aystl/core/type_traits/utils/compare.hpp"
#include "aystl/core/type_traits/utils/int_seq.hpp"

namespace iin {
namespace detail {
template <TypeListType T, TypeListType... Ts>
struct type_list_cat : type_t<T> {};

template <TypeListType OutT, TypeListType InT,
    template <typename...> typename Tmpl, typename... TmplArgs>
struct type_list_map;
}
template <TypeListType... Ts>
using type_list_cat_t = typename detail::type_list_cat<Ts...>::type;

template <typename... Ts>
struct type_list {
    static constexpr std::size_t size() noexcept { return sizeof...(Ts); }

    using type = type_list<Ts...>;

    template <template <typename...> class Tmpl>
    using wrapped = wrap_tmpl_t<Tmpl, Ts...>;

    template <template <typename...> class Tmpl, typename... TmplArgs>
    using map = typename detail::type_list_map<
        type_list<>, type, Tmpl, TmplArgs...>::type;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using get = std::tuple_element_t<pos, wrapped<std::tuple>>;

    template <std::integral IntT, IntT... Is>
    static auto __sliceImpl(int_seq<IntT, Is...>)
        -> type_list<get<static_cast<std::size_t>(Is)>...>; 

    template <IntSeqType RangeT>
    using slice = decltype(__sliceImpl(std::declval<RangeT>()));
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
    using erase = type_list_cat_t<
        slice_range<0, pos>, slice_range<pos + 1>>;

    template <typename OldT, typename NewT>
    using replace = map<replace_if_same_as_t, OldT, NewT>;
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
}
}

