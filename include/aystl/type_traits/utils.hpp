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

#include <type_traits>

namespace iin {
struct null_t;
struct empty_t {};
struct any_t {
    template <typename U>
    constexpr operator U();
};

template <typename T>
struct type_t { using type = T; };
template <auto _v>
struct value_t { static constexpr auto value = _v; };

template <typename T>
struct take_off { using thing = T; };
template <typename T>
struct take_off<type_t<T>> { using thing = T; };
template <auto _v>
struct take_off<value_t<_v>> { static constexpr auto thing = _v; };

template <typename... Ts>
struct type_list {
    template <template <typename...> class Tmpl>
    using wrapped = Tmpl<Ts...>;
};

template <auto... Vs>
struct value_list {
    template <template <auto...> class Tmpl>
    using wrapped = Tmpl<Vs...>;

    template <template <typename...> class Tmpl>
    using type_wrapped = Tmpl<value_t<Vs>...>;
};

template <auto... Vs>
using value_t_list = value_list<Vs...>::template type_wrapped<type_list>;

template <typename... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
overload(Ts...) -> overload<Ts...>;

template <class... Args>
using is_any_of = std::disjunction<Args...>;

template <class... Args>
inline constexpr bool is_any_of_v = is_any_of<Args...>::value;

template <class... Args>
using is_all_of = std::conjunction<Args...>;

template <class... Args>
inline constexpr bool is_all_of_v = is_all_of<Args...>::value;

namespace detail {
template <typename T>
constexpr bool is_type_v = std::is_void_v<std::void_t<T>>;

template <template <typename...> class Tmpl, typename... Args>
struct wrap_tmpl : type_t<Tmpl<Args...>> {};

template <template <typename...> class Tmpl, typename... TmplArgs, typename... Args>
struct wrap_tmpl<Tmpl, Tmpl<TmplArgs...>, Args...> : type_t<Tmpl<TmplArgs...>> {};

template <typename T, typename... Args>
struct replace_tmpl_args : type_t<T> {};

template <template <typename...> class Tmpl, typename... TmplArgs, typename... Args>
requires is_type_v<Tmpl<Args...>>
struct replace_tmpl_args<Tmpl<TmplArgs...>, Args...> : type_t<Tmpl<Args...>> {};

template <typename T, template <typename...> class Tmpl>
struct replace_tmpl_wrapper : type_t<T> {};

template <template <typename...> class Tmpl1,
    template <typename...> class Tmpl2, typename... Args>
requires is_type_v<Tmpl2<Args...>>
struct replace_tmpl_wrapper<Tmpl1<Args...>, Tmpl2> : type_t<Tmpl2<Args...>> {};
}

template <template <typename...> class Tmpl, typename... Args>
using wrap_tmpl_t = typename detail::wrap_tmpl<Tmpl, Args...>::type;

template <typename T, typename... Args>
using replace_tmpl_args_t = typename detail::replace_tmpl_args<T, Args...>::type;

template <typename T, template <typename...> class Tmpl>
using replace_tmpl_wrapper_t = typename detail::replace_tmpl_wrapper<T, Tmpl>::type;
}

