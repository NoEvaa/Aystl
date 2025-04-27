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
#include "aystl/tmp/meta/meta_decl.hpp"
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
namespace _tmp_impl {
template <CoListType T, CoListType... Ts>
struct constant_list_cat : type_t<T> {};

template <CoListType T, std::size_t pos>
struct constant_list_at;
template <CoListType T, std::size_t pos, typename DefaultT>
struct constant_list_get;

template <CoListType InT, TyListType MaskT,
    CoListType OutT, std::size_t pos = 0>
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

    template <MetaTmplType TmplT>
    using wrapped = meta_wrap_t<TmplT, type>;

    template <MetaTmplType TmplT>
    using map = typename _tmp_impl::meta_list_map<
        type, constant_list<value_type>, TmplT>::type;
    template <MetaTmplType TmplT>
    using ty_map = typename _tmp_impl::meta_list_map<
        type, type_list<>, TmplT>::type;
    template <MetaTmplType TmplT>
    using va_map = typename _tmp_impl::meta_list_map<
        type, value_list<>, TmplT>::type;
    template <MetaTmplType TmplT, typename _VTp>
    using co_map = typename _tmp_impl::meta_list_map<
        type, constant_list<_VTp>, TmplT>::type;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using at = typename _tmp_impl::constant_list_at<type, pos>::type;
    template <std::size_t pos>
    static constexpr auto at_v = at<pos>::value;
    template <std::size_t pos, typename DefaultT = null_t>
    using get = typename _tmp_impl::constant_list_get<type, pos, DefaultT>::type;

    template <CoListTType<value_type>... _Ts>
    using concat = typename _tmp_impl::constant_list_cat<type, _Ts...>::type;

    template <value_type... _Vs>
    using push_back  = constant_list<value_type, Vs..., _Vs...>;
    template <value_type... _Vs>
    using push_front = constant_list<value_type, _Vs..., Vs...>;

    template <TyListType MaskT>
    using filter = typename _tmp_impl::constant_list_filter<
        type, MaskT, constant_list<value_type>>::type;

    template <typename _Cmp = std::less<>>
    using sort = typename ct_sorted_array<_Cmp, value_type, Vs...>::to_constant_list;

    //using unique_sort;
};

namespace detail {

}
}

