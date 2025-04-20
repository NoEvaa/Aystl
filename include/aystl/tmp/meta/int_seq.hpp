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
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
template <std::integral T, T... Is>
using int_seq = constant_list<T, Is...>;

template <typename T>
concept IntSeqType = is_constant_spec_of_v<T, int_seq>;

template <std::size_t... Is>
using index_seq = int_seq<std::size_t, Is...>;

template <typename T>
concept IndexSeqType = ConstantListTType<T, std::size_t>;

namespace detail {
template <typename T, T... Is>
auto _toIntSeq(std::integer_sequence<T, Is...>) -> int_seq<T, Is...>;

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
template<size_t N>
using make_index_seq = decltype(detail::_toIntSeq(
    std::declval<std::make_index_sequence<N>>()));

template <std::integral T, T... Is>
using monotone_int_seq_t = typename detail::monotone_int_seq<T, Is...>::type;

template <int _start, int _stop, int _step = 1>
using ct_range_t = monotone_int_seq_t<int, _start, _stop, _step>;
}

