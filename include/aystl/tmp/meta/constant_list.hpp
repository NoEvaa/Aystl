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
#include "aystl/tmp/meta/utils.hpp"
#include "aystl/tmp/type_traits/compare.hpp"

namespace iin {
namespace _tmp_impl {
template <CoListType T, CoListType... Ts>
struct constant_list_cat : type_t<T> {};

template <CoListType T, std::size_t pos>
struct constant_list_at;

template <CoListType InT, TyListType MaskT,
    CoListType OutT, std::size_t pos = 0>
struct constant_list_filter : type_t<OutT> {};

template <CoListType, typename AlgoT> struct constant_list_apply_algo;
template <CoListType, typename CmpT> struct constant_list_sort;
template <CoListType> struct constant_list_sorted_unique;
}

template<typename T, T... Vs>
struct constant_list {
    using type       = constant_list;
    using value_type = T;

    static constexpr index_constant<sizeof...(Vs)> size;

    template <typename _Tp>
    using cast = constant_list<_Tp, static_cast<_Tp>(Vs)...>;

    template <MetaTmplType TmplT>
    using wrapped = meta_wrap_t<TmplT, type>;

    template <TyTmplType TmplT>
    using transform = ty_wrap_t<TmplT, type>;
    template <TyTmplType TmplT>
    using transform_t = typename transform<TmplT>::type;
    template <TyTmplType TmplT>
    using transform_tt = typename transform<TmplT>::ttype;

    template <MetaTmplType TmplT>
    using map = meta_list_map_t<type, constant_list<value_type>, TmplT>;
    template <MetaTmplType TmplT>
    using ty_map = meta_list_map_t<type, type_list<>, TmplT>;
    template <MetaTmplType TmplT>
    using va_map = meta_list_map_t<type, value_list<>, TmplT>;
    template <MetaTmplType TmplT, typename _VTp>
    using co_map = meta_list_map_t<type, constant_list<_VTp>, TmplT>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using at = typename _tmp_impl::constant_list_at<type, pos>::type;
    template <std::size_t pos>
    static constexpr auto at_v = at<pos>::value;

    template <CoListTType<value_type>... _Ts>
    using concat = typename _tmp_impl::constant_list_cat<type, _Ts...>::type;

    template <value_type... _Vs>
    using push_back  = constant_list<value_type, Vs..., _Vs...>;
    template <value_type... _Vs>
    using push_front = constant_list<value_type, _Vs..., Vs...>;

    template <TyListType MaskT>
    using filter = typename _tmp_impl::constant_list_filter<
        type, MaskT, constant_list<value_type>>::type;

    template <typename _AlgoT>
    using apply_algo = typename _tmp_impl::constant_list_apply_algo<type, _AlgoT>::type;

    template <typename _CmpT = std::less<>>
    using sort = typename _tmp_impl::constant_list_sort<type, _CmpT>::type;

    template <typename _Cmp = std::less<>>
    using unique_sort = typename _tmp_impl::constant_list_sorted_unique<sort<_Cmp>>::type;
};
}

