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
#include "aystl/core/type_traits/compare.hpp"

namespace iin {
template <std::integral T, T... Is>
using int_seq = std::integer_sequence<T, Is...>;

namespace detail {
template <typename T>
struct is_int_seq : std::false_type {};
template <std::integral T, T... Is>
struct is_int_seq<int_seq<T, Is...>> : std::true_type {};
}
template <typename T>
concept IntSeqType = detail::is_int_seq<T>::value;

template <std::size_t... Is>
using index_seq = int_seq<std::size_t, Is...>;

namespace detail {
template <std::integral T, T... Is, std::integral T2, T2... Is2>
auto _concat_two_int_seqs(int_seq<T, Is...>, int_seq<T2, Is2...>)
    -> int_seq<T, Is..., Is2...>;

template <IntSeqType T, IntSeqType... Ts>
struct int_seq_cat : type_t<T> {};

template <IntSeqType T1, IntSeqType T2, IntSeqType... Ts>
struct int_seq_cat<T1, T2, Ts...> {
    using type = typename int_seq_cat<
        decltype(_concat_two_int_seqs(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};

template <std::integral T, T _start, T _stop, T _step, T... Is>
struct monotone_int_seq : type_t<int_seq<T, Is...>> {};

template <std::integral T, T _start, T _stop, T _step, T... Is>
requires (
    (CtCmp<CmpOp::kLT, 0, _step> && CtCmp<CmpOp::kLT, _start, _stop>) ||
    (CtCmp<CmpOp::kLT, _step, 0> && CtCmp<CmpOp::kLT, _stop, _start>))
struct monotone_int_seq<T, _start, _stop, _step, Is...> {
    using type = typename monotone_int_seq<T,
        _start + _step, _stop, _step, Is..., _start>::type;
};
}

template <IntSeqType... Ts>
using int_seq_cat_t = typename detail::int_seq_cat<Ts...>::type;

template <std::integral T, T... Is>
using monotone_int_seq_t = typename detail::monotone_int_seq<T, Is...>::type;

template <int _start, int _stop, int _step = 1>
using ct_range_t = monotone_int_seq_t<int, _start, _stop, _step>;

template <IntSeqType... Ts>
struct int_seq_list : type_list<Ts...> {};
}

