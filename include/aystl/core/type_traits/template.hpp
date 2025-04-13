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

#include "aystl/core/type_traits/meta.hpp"

namespace iin {
template <typename T, template <typename...> typename Tmpl>
struct is_spec_of : std::false_type {};

template <
    template <typename...> typename Tmpl1,
    template <typename...> typename Tmpl2,
    typename... Args>
requires std::is_same_v<Tmpl1<Args...>, Tmpl2<Args...>>
struct is_spec_of<Tmpl1<Args...>, Tmpl2> : std::true_type {};

template <typename T, template <typename...> typename Tmpl>
inline constexpr bool is_spec_of_v = is_spec_of<T, Tmpl>::value;

template <typename T, template <typename...> typename... Tmpls>
inline constexpr bool is_any_spec_of_v = is_any_of_v<is_spec_of<T, Tmpls>...>;

template <typename T, template <auto...> typename Tmpl>
struct is_value_spec_of : std::false_type {};

template <
    template <auto...> typename Tmpl1,
    template <auto...> typename Tmpl2,
    auto... args>
requires std::is_same_v<Tmpl1<args...>, Tmpl2<args...>>
struct is_value_spec_of<Tmpl1<args...>, Tmpl2> : std::true_type {};

template <typename T, template <auto...> typename Tmpl>
inline constexpr bool is_value_spec_of_v = is_value_spec_of<T, Tmpl>::value;

namespace detail {
template <template <typename...> class Tmpl, typename... Args>
struct wrap_tmpl : type_t<Tmpl<Args...>> {};

template <template <typename...> class Tmpl, typename... TmplArgs, typename... Args>
struct wrap_tmpl<Tmpl, Tmpl<TmplArgs...>, Args...> : type_t<Tmpl<TmplArgs...>> {};

template <typename T>
struct unwrap_tmpl : type_t<T> {};

template <template <typename...> class Tmpl, typename T, typename... Args>
struct unwrap_tmpl<Tmpl<T, Args...>> : type_t<T> {};

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

template <typename T>
using unwrap_tmpl_t = typename detail::unwrap_tmpl<T>::type;

template <typename T, typename... Args>
using replace_tmpl_args_t = typename detail::replace_tmpl_args<T, Args...>::type;

template <typename T, template <typename...> class Tmpl>
using replace_tmpl_wrapper_t = typename detail::replace_tmpl_wrapper<T, Tmpl>::type;
}

