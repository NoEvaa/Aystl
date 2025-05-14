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
#include "aystl/tmp/meta/template.hpp"
#include "aystl/tmp/type_traits/compare.hpp"

namespace iin {
namespace _tmp_impl {
template <MetaListType, typename...> struct meta_list_push_back;
template <MetaListType, std::size_t> struct meta_list_at;
template <typename, typename DefaultT> struct meta_list_get;
template <MetaListType, TyListType MaskT> struct meta_list_mask_filter;
template <MetaListType> struct meta_list_reverse;

template <MetaListType InT, MetaListType OutT,
    MetaTmplType TmplT, typename... TmplArgs>
struct meta_list_map;
}
template <typename T, typename DefaultT = null_t>
using meta_list_get_tt = typename _tmp_impl::meta_list_get<T, DefaultT>::ttype;

template <MetaListType T>
using meta_list_reverse_t = typename _tmp_impl::meta_list_reverse<T>::type;

template <MetaListType InT, MetaListType OutT,
    MetaTmplType TmplT, typename... TmplArgs>
using meta_list_map_t = typename _tmp_impl::meta_list_map<
    InT, OutT, TmplT, TmplArgs...>::type;

namespace detail {
template <typename T>
struct basic_meta_list {
    using type = T;

    template <MetaTmplType TmplT>
    using wrapped = meta_wrap_t<TmplT, type>;

    template <MetaTmplType TmplT, typename... _Ts>
    using transform = meta_wrap_t<TmplT, type_list<type, _Ts...>>;
    template <MetaTmplType TmplT, typename... _Ts>
    using transform_t = typename transform<TmplT, _Ts...>::type;
    template <MetaTmplType TmplT, typename... _Ts>
    using transform_tt = typename transform<TmplT, _Ts...>::ttype;

    template <MetaTmplType TmplT>
    using ty_map = meta_list_map_t<type, type_list<>, TmplT>;
    template <MetaTmplType TmplT>
    using va_map = meta_list_map_t<type, value_list<>, TmplT>;
    template <MetaTmplType TmplT, typename _VTp>
    using co_map = meta_list_map_t<type, constant_list<_VTp>, TmplT>;

    template <std::size_t pos>
    requires CtCmp<CmpOp::kLT, pos, type::size()>
    using at = typename _tmp_impl::meta_list_at<type, pos>::type;
    template <std::size_t pos, typename DefaultT = null_t>
    using get = va_wrap_t<meta_list_get_tt<type, DefaultT>, pos>;

    template <typename... _Ts>
    using push_back = typename _tmp_impl::meta_list_push_back<type, _Ts...>::type;

    template <TyListType MaskT>
    using mask_filter = typename _tmp_impl::meta_list_mask_filter<type, MaskT>::type;
    template <MetaTmplType TmplT>
    using filter = mask_filter<ty_map<TmplT>>;
};
}
}

