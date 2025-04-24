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
namespace detail {
template <std::integral T, T _start, T _stop, T _step, T... Is>
struct ct_range_impl : type_t<int_seq<T, Is...>> {};

template <std::integral T, T _start, T _stop, T _step, T... Is>
requires (
    (CtCmp<CmpOp::kLT, 0, _step> && CtCmp<CmpOp::kLT, _start, _stop>) ||
    (CtCmp<CmpOp::kLT, _step, 0> && CtCmp<CmpOp::kLT, _stop, _start>))
struct ct_range_impl<T, _start, _stop, _step, Is...> {
    using type = typename ct_range_impl<T,
        _start + _step, _stop, _step, Is..., _start>::type;
};
}
template <std::integral T, T _start, T _stop, T _step>
struct ct_range {
    using type = typename detail::ct_range_impl<T, _start, _stop, _step>::type;
};
}

