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

#include "aystl/tmp/type_traits/common.hpp"
#include "aystl/tmp/type_traits/is_specialization_of.hpp"

namespace iin {
struct null_t;
struct empty_t {};
struct any_t {
    template <typename U>
    constexpr operator U();
};

template <typename T>
struct type_t { using type = T; };
template <typename T, T _v>
struct constant_t {
    using value_type = T;
    static constexpr value_type value = _v;

    template <typename _Tp>
    using cast = constant_t<_Tp, static_cast<_Tp>(_v)>;

    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator ()() const noexcept { return value; }
};
template <auto _v>
struct value_t : constant_t<decltype(_v), _v> {};

template <typename T>
concept TypeTType = requires { detail::is_type_v<type_t<typename T::type>>; };
template <typename T>
concept ValueTType = requires { detail::is_type_v<value_t<T::value>>; };
template <typename T, typename VT>
concept ConstantTType = ValueTType<T>
    && std::is_same_v<typename T::value_type, VT>;

template <auto _i>
using index_constant = constant_t<std::size_t, static_cast<std::size_t>(_i)>;
template <auto _b>
using bool_constant = constant_t<bool, static_cast<bool>(_b)>;

using invalid_index_t = index_constant<-1>;

template <template <typename...> class Tmpl>      struct template_t;
template <template <auto...> class Tmpl>          struct value_template_t;
template <template <typename T, T...> class Tmpl> struct constant_template_t;

namespace detail {
template <typename>
struct is_meta_tmpl : bool_constant<false> {};
template <typename>
struct is_meta_primitive_tmpl : bool_constant<false> {};
template <typename T> requires is_meta_primitive_tmpl<T>::value
struct is_meta_tmpl<T> : bool_constant<true> {};

template <typename T>
struct is_ty_tmpl : std::false_type {};
template <template <typename...> class Tmpl>
struct is_ty_tmpl<template_t<Tmpl>> : std::true_type {};
template <typename T> requires is_ty_tmpl<T>::value
struct is_meta_primitive_tmpl<T> : bool_constant<true> {};

template <typename T>
struct is_va_tmpl : std::false_type {};
template <template <auto...> class Tmpl>
struct is_va_tmpl<value_template_t<Tmpl>> : std::true_type {};
template <typename T> requires is_va_tmpl<T>::value
struct is_meta_primitive_tmpl<T> : bool_constant<true> {};

template <typename T>
struct is_co_tmpl : std::false_type {};
template <template <typename T, T...> class Tmpl>
struct is_co_tmpl<constant_template_t<Tmpl>> : std::true_type {};
template <typename T> requires is_co_tmpl<T>::value
struct is_meta_primitive_tmpl<T> : bool_constant<true> {};
}
template <typename T>
concept MetaTmplType = detail::is_meta_tmpl<T>::value;
template <typename T>
concept MetaPrimTmplType = MetaTmplType<T> && detail::is_meta_primitive_tmpl<T>::value;
template <typename T>
concept TyTmplType = MetaPrimTmplType<T> && detail::is_ty_tmpl<T>::value;
template <typename T>
concept VaTmplType = MetaPrimTmplType<T> && detail::is_va_tmpl<T>::value;
template <typename T>
concept CoTmplType = MetaPrimTmplType<T> && detail::is_co_tmpl<T>::value;

template <template <typename...> class Tmpl>
using ty_tmpl_t = template_t<Tmpl>;
template <template <auto...> class Tmpl>
using va_tmpl_t = value_template_t<Tmpl>;
template <template <typename T, T...> class Tmpl>
using co_tmpl_t = constant_template_t<Tmpl>;

template <typename T1, typename T2>
struct type_pair {
    using first_type  = T1;
    using second_type = T2;
};

template <typename T>
concept TypePairType = is_spec_of_v<T, type_pair>;

template <typename... Ts>      struct type_list;
template <auto... Vs>          struct value_list;
template <typename T, T... Vs> struct constant_list;

namespace detail {
template <typename>
struct is_meta_list : bool_constant<false> {};
template <typename T> requires is_spec_of_v<T, type_list>
struct is_meta_list<T> : bool_constant<true> {};
template <typename T> requires is_value_spec_of_v<T, value_list>
struct is_meta_list<T> : bool_constant<true> {};
template <typename T> requires is_constant_spec_of_v<T, constant_list>
struct is_meta_list<T> : bool_constant<true> {};
}
template <typename T>
concept MetaListType = detail::is_meta_list<T>::value;
template <typename T>
concept TyListType = MetaListType<T> && is_spec_of_v<T, type_list>;
template <typename T>
concept VaListType = MetaListType<T> && is_value_spec_of_v<T, value_list>;
template <typename T>
concept CoListType = MetaListType<T> && is_constant_spec_of_v<T, constant_list>;
template <typename T, typename VT>
concept CoListTType = CoListType<T> && std::is_same_v<typename T::value_type, VT>;

template <auto... Vs>
using value_t_list = type_list<value_t<Vs>...>;

template <std::integral T, T... Is>
using int_seq = constant_list<T, Is...>;
template <typename T>
concept IntSeqType = is_constant_spec_of_v<T, int_seq>;
template <std::size_t... Is>
using index_seq = int_seq<std::size_t, Is...>;
template <typename T>
concept IndexSeqType = CoListTType<T, std::size_t>;

template <typename T>
struct take_off { using magic = T; };
template <TypeTType T> 
requires (!ValueTType<T>)
struct take_off<T> { using magic = typename T::type; };
template <ValueTType T>
struct take_off<T> { static constexpr auto magic = T::value; };
}

