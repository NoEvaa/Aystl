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
#include <concepts>
#include <utility>

namespace iin {
template <std::integral T, T... Is>
using int_seq = std::integer_sequence<T, Is...>;

namespace detail {
template <typename T>
struct is_int_seq : std::true_type {};

template <std::integral T, T... Is>
struct is_int_seq<int_seq<T, Is...>> : std::true_type {};
}

template <typename T>
concept IntSeqType = detail::is_int_seq<T>::value;

namespace detail {
template <std::integral T, T... Is, std::integral T2, T... Is2>
constexpr auto _concat_two_int_seqs_impl(
    int_seq<T, Is...>, int_seq<T2, Is2...>) {
    return int_seq<T, Is..., Is2...>();
};

template <IntSeqType T1, IntSeqType T2>
struct concat_two_int_seqs {
    using type = decltype(_concat_two_int_seqs_impl(T1{}, T2{}));
};
}

template <IntSeqType T, IntSeqType... Ts>
struct concat_int_seqs {
    using type = T;
};

template <IntSeqType T1, IntSeqType T2, IntSeqType... Ts>
struct concat_int_seqs<T1, T2, Ts...> {
    using type = typename concat_int_seqs<
        typename detail::concat_two_int_seqs<T1, T2>::type,
        Ts...
    >::type;
};

template <IntSeqType... Ts>
struct int_multi_seqs {
    template <template <IntSeqType...> class Tmpl>
    using wrapped = Tmpl<Ts...>;
};
}
