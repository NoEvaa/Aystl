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
namespace _tmp_impl {
template <MetaPrimTmplType T, typename... Ts>        struct ty_wrap;
template <MetaPrimTmplType T, auto... Vs>            struct va_wrap;
template <MetaPrimTmplType T, typename VT, VT... Vs> struct co_wrap;
template <MetaTmplType TmplT, MetaListType T>        struct meta_wrap;
}
template <MetaPrimTmplType T, typename... Ts>
using ty_wrap_t = typename _tmp_impl::ty_wrap<T, Ts...>::type;
template <MetaPrimTmplType T, auto... Vs>
using va_wrap_t = typename _tmp_impl::va_wrap<T, Vs...>::type;
template <MetaPrimTmplType T, typename VT, VT... Vs>
using co_wrap_t = typename _tmp_impl::co_wrap<T, VT, Vs...>::type;
template <MetaTmplType TmplT, MetaListType T>
using meta_wrap_t = typename _tmp_impl::meta_wrap<TmplT, T>::type;

template <template <typename...> class Tmpl>
struct template_t {
    template <typename... _Ts>
    struct __tmpl_pkg : type_t<Tmpl<_Ts...>> {};

    template <typename... _Ts>
    using wrap = typename __tmpl_pkg<_Ts...>::type;

    template <typename _Tp>
    using is_wrapped_to = is_spec_of<_Tp, Tmpl>;
};

template <template <auto...> class Tmpl>
struct value_template_t {
    template <auto... _Vs>
    struct __tmpl_pkg : type_t<Tmpl<_Vs...>> {};

    template <auto... _Vs>
    using wrap = typename __tmpl_pkg<_Vs...>::type;

    template <typename _Tp>
    using is_wrapped_to = is_value_spec_of<_Tp, Tmpl>;
};

template <template <typename T, T...> class Tmpl>
struct constant_template_t {
    template <typename _Tp, _Tp... _Vs>
    struct __tmpl_pkg : type_t<Tmpl<_Tp, _Vs...>> {};

    template <typename _Tp, _Tp... _Vs>
    using wrap = typename __tmpl_pkg<_Tp, _Vs...>::type;

    template <typename _Tp>
    using is_wrapped_to = is_constant_spec_of<_Tp, Tmpl>;
};

namespace _tmp_impl {
template <typename T, MetaPrimTmplType TmplT> struct meta_rewrapped;
template <typename T, MetaListType ArgsT> struct meta_rewrap;
}

template <typename T, MetaPrimTmplType TmplT>
using meta_rewrapped_t = typename _tmp_impl::meta_rewrapped<T, TmplT>::type;
template <typename T, MetaListType ArgsT>
using meta_rewrap_t = typename _tmp_impl::meta_rewrap<T, ArgsT>::type;

using ty_list_tt = ty_tmpl_t<type_list>;
using va_list_tt = va_tmpl_t<value_list>;
using co_list_tt = co_tmpl_t<constant_list>;
}

