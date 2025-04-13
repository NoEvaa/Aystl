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

#include "aystl/core/type_traits/common.hpp"

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
struct constant_t : type_t<T> { static constexpr T value = _v; };
template <auto _v>
struct value_t : constant_t<decltype(_v), _v> {};

template <template <typename...> class Tmpl>
struct template_t {
    template <typename... Ts>
    using wrap = Tmpl<Ts...>;
};

template <typename... Ts> struct type_list;
template <auto... Vs> struct value_list;

template <typename T>
concept TypeTType = requires { detail::is_type_v<type_t<typename T::type>>; };
template <typename T>
concept ValueTType = requires { detail::is_type_v<value_t<T::value>>; };
template <typename T, typename VT>
concept ConstantTType = ValueTType<T> && TypeTType<T> && std::is_same_v<typename T::type, VT>;

namespace detail {
template <typename T>
struct is_template_t : std::false_type {};
template <template <typename...> class Tmpl>
struct is_template_t<template_t<Tmpl>> : std::true_type {};
}
template <typename T>
concept TemplateTType = detail::is_template_t<T>::value;

template <typename T>
struct take_off { using magic = T; };
template <TypeTType T> 
requires (!ValueTType<T>)
struct take_off<T> { using magic = typename T::type; };
template <ValueTType T>
struct take_off<T> { static constexpr auto magic = T::value; };
}

