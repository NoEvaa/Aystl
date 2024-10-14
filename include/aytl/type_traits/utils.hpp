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
template <class... Args>
using is_any_of = std::disjunction<Args...>;

template <class... Args>
inline constexpr bool is_any_of_v = is_any_of<Args...>::value;

template <class... Args>
using is_all_of = std::conjunction<Args...>;

template <class... Args>
inline constexpr bool is_all_of_v = is_all_of<Args...>::value;

template <typename T, template <typename...> typename Tmpl, typename = void>
struct is_spec_of : std::false_type {};

template <
    template <typename...> typename Tmpl1,
    template <typename...> typename Tmpl2,
    typename... Args>
struct is_spec_of<Tmpl1<Args...>, Tmpl2,
    std::enable_if_t<std::is_same_v<Tmpl1<Args...>, Tmpl2<Args...>>>> : std::true_type {};

template <typename T, template <typename...> typename Tmpl, typename = void>
inline constexpr bool is_spec_of_v = is_spec_of<T, Tmpl>::value;

namespace detail {
template <template <typename...> class Tmpl, typename... Args>
struct wrap_tmpl {
    using type = Tmpl<Args...>;
};

template <template <typename...> class Tmpl, typename... TmplArgs, typename... Args>
struct wrap_tmpl<Tmpl, Tmpl<TmplArgs...>, Args...> {
    using type = Tmpl<TmplArgs...>;
};
}

template <template <typename...> class Tmpl, typename... Args>
using wrap_tmpl_t = typename detail::wrap_tmpl<Tmpl, Args...>::type;

}
