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
#include "aystl/tmp/type_traits/template.hpp"

namespace iin {
namespace detail {
template <MetaTmplType T, typename... Ts> struct ty_wrap;
template <TyTmplType T, typename... Ts>
struct ty_wrap<T, Ts...> {
    using type = typename T::template wrap<Ts...>;
};
template <VaTmplType T, typename... Ts>
requires is_all_of_v<constant_t<bool, ValueTType<Ts>>...>
struct ty_wrap<T, Ts...> {
    using type = typename T::template wrap<Ts::value...>;
};
/*template <CoTmplType T, typename VT, typename... Ts>
requires is_all_of_v<constant_t<bool, ValueTType<Ts>>...>
struct ty_wrap<T, VT, Ts...> {
    using type = typename T::template wrap<
        VT, static_cast<VT>(Ts::value)...>;
};*/

template <MetaTmplType T, auto... Vs> struct va_wrap;
template <VaTmplType T, auto... Vs>
struct va_wrap<T, Vs...> {
    using type = typename T::template wrap<Vs...>;
};
template <TyTmplType T, auto... Vs>
struct va_wrap<T, Vs...> {
    using type = typename T::template wrap<value_t<Vs>...>;
};

template <MetaTmplType T, typename VT, VT... Vs> struct co_wrap;
template <CoTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = typename T::template wrap<VT, Vs...>;
};
template <VaTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = typename T::template wrap<Vs...>;
};
template <TyTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = typename T::template wrap<constant_t<VT, Vs>...>;
};

template <MetaTmplType TmplT, MetaListType T> struct meta_wrap;
template <MetaTmplType TmplT, typename... Ts>
struct meta_wrap<TmplT, type_list<Ts...>> {
    using type = typename ty_wrap<TmplT, Ts...>::type;
};
template <MetaTmplType TmplT, auto... Vs>
struct meta_wrap<TmplT, value_list<Vs...>> {
    using type = typename va_wrap<TmplT, Vs...>::type;
};
template <MetaTmplType TmplT, typename VT, VT... Vs>
struct meta_wrap<TmplT, constant_list<VT, Vs...>> {
    using type = typename co_wrap<TmplT, VT, Vs...>::type;
};

template <typename T, MetaTmplType TmplT> struct meta_rewrapped;
template <typename T, TyTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_tmpl_wrapper_t<T, TmplT::template wrap>;
};
template <typename T, VaTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_va_tmpl_wrapper_t<T, TmplT::template wrap>;
};
template <typename T, CoTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_co_tmpl_wrapper_t<T, TmplT::template wrap>;
};

template <typename T, MetaListType ArgsT> struct meta_rewrap;
template <typename T, typename... Ts>
struct meta_rewrap<T, type_list<Ts...>> {
    using type = replace_tmpl_args_t<T, Ts...>;
};
template <typename T, auto... Vs>
struct meta_rewrap<T, value_list<Vs...>> {
    using type = replace_va_tmpl_args_t<T, Vs...>;
};
template <typename T, typename VT, VT... Vs>
struct meta_rewrap<T, constant_list<VT, Vs...>> {
    using type = replace_co_tmpl_args_t<T, VT, Vs...>;
};
}

template <MetaTmplType T, typename... Ts>
using ty_wrap_t = typename detail::ty_wrap<T, Ts...>::type;
template <MetaTmplType T, auto... Vs>
using va_wrap_t = typename detail::va_wrap<T, Vs...>::type;
template <MetaTmplType T, typename VT, VT... Vs>
using co_wrap_t = typename detail::co_wrap<T, VT, Vs...>::type;
template <MetaTmplType TmplT, MetaListType T>
using meta_wrap_t = typename detail::meta_wrap<TmplT, T>::type;

template <typename T, MetaTmplType TmplT>
using meta_rewrapped_t = typename detail::meta_rewrapped<T, TmplT>::type;
template <typename T, MetaListType ArgsT>
using meta_rewrap_t = typename detail::meta_rewrap<T, ArgsT>::type;

using ty_list_tmpl_t = ty_tmpl_t<type_list>;
using va_list_tmpl_t = va_tmpl_t<value_list>;
using co_list_tmpl_t = co_tmpl_t<constant_list>;
}

