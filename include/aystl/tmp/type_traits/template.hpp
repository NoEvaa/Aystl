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
#include "aystl/tmp/type_traits/is_specialization_of.hpp"

namespace iin {
namespace detail {
template <template <typename...> class Tmpl, typename... Ts>
struct wrap_tmpl : type_t<Tmpl<Ts...>> {};
template <template <typename...> class Tmpl, typename... TmplArgs, typename... Args>
struct wrap_tmpl<Tmpl, Tmpl<TmplArgs...>, Args...> : type_t<Tmpl<TmplArgs...>> {};

template <typename T>
struct unwrap_tmpl : type_t<T> {};
template <template <typename...> class Tmpl, typename T, typename... Args>
struct unwrap_tmpl<Tmpl<T, Args...>> : type_t<T> {};

template <typename T, typename... Args>
struct replace_tmpl_args : type_t<T> {};
template <template <typename...> class Tmpl, typename... OldTs, typename... NewTs>
requires is_type_v<Tmpl<NewTs...>>
struct replace_tmpl_args<Tmpl<OldTs...>, NewTs...> : type_t<Tmpl<NewTs...>> {};

template <typename T, auto... _args>
struct replace_va_tmpl_args : type_t<T> {};
template <template <auto...> class Tmpl, auto... _old_args, auto... _new_args>
requires is_type_v<Tmpl<_new_args...>>
struct replace_va_tmpl_args<Tmpl<_old_args...>, _new_args...>
    : type_t<Tmpl<_new_args...>> {};

template <typename T, typename VT, VT... _args>
struct replace_co_tmpl_args : type_t<T> {};
template <template <typename _Tp, _Tp...> class Tmpl,
    typename OldT, OldT... _old_args,
    typename NewT, NewT... _new_args>
requires is_type_v<Tmpl<NewT, _new_args...>>
struct replace_co_tmpl_args<Tmpl<OldT, _old_args...>, NewT, _new_args...>
    : type_t<Tmpl<NewT, _new_args...>> {};

template <typename T, template <typename...> class Tmpl>
struct replace_tmpl_wrapper : type_t<T> {};
template <template <typename...> class OldTmpl,
    template <typename...> class NewTmpl, typename... Ts>
requires is_type_v<NewTmpl<Ts...>>
struct replace_tmpl_wrapper<OldTmpl<Ts...>, NewTmpl>
    : type_t<NewTmpl<Ts...>> {};

template <typename T, template <auto...> class Tmpl>
struct replace_va_tmpl_wrapper : type_t<T> {};
template <template <auto...> class OldTmpl,
    template <auto...> class NewTmpl, auto... _args>
requires is_type_v<NewTmpl<_args...>>
struct replace_va_tmpl_wrapper<OldTmpl<_args...>, NewTmpl>
    : type_t<NewTmpl<_args...>> {};

template <typename T, template <typename _Tp, _Tp...> class Tmpl>
struct replace_co_tmpl_wrapper : type_t<T> {};
template <template <typename _Tp, _Tp...> class OldTmpl,
    template <typename _Tp, _Tp...> class NewTmpl,
    typename VT, VT... _args>
requires is_type_v<NewTmpl<VT, _args...>>
struct replace_co_tmpl_wrapper<OldTmpl<VT, _args...>, NewTmpl>
    : type_t<NewTmpl<VT, _args...>> {};
}

template <template <typename...> class Tmpl, typename... Args>
using wrap_tmpl_t = typename detail::wrap_tmpl<Tmpl, Args...>::type;

template <typename T>
using unwrap_tmpl_t = typename detail::unwrap_tmpl<T>::type;

template <typename T, typename... Args>
using replace_tmpl_args_t = typename detail::replace_tmpl_args<T, Args...>::type;
template <typename T, auto... _args>
using replace_va_tmpl_args_t = typename detail::replace_va_tmpl_args<T, _args...>::type;
template <typename T, typename VT, VT... _args>
using replace_co_tmpl_args_t = typename detail::replace_co_tmpl_args<T, VT, _args...>::type;

template <typename T, template <typename...> class Tmpl>
using replace_tmpl_wrapper_t = typename detail::replace_tmpl_wrapper<T, Tmpl>::type;
template <typename T, template <auto...> class Tmpl>
using replace_va_tmpl_wrapper_t = typename detail::replace_va_tmpl_wrapper<T, Tmpl>::type;
template <typename T, template <typename _Tp, _Tp...> class Tmpl>
using replace_co_tmpl_wrapper_t = typename detail::replace_co_tmpl_wrapper<T, Tmpl>::type;
}

