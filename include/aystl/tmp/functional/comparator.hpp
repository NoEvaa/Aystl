/**
 * Copyright 2025 NoEvaa
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
#include "aystl/global/common.hpp"

namespace iin {
namespace _tmp_impl {
template <VaTmplType, CmpOp>
struct ct_pos_value_comparator;

template <VaTmplType, std::size_t, bool, bool>
struct ct_pos_forward_comparator;
}
/**
 * TmplT<lpos>::value ${_cmp_op} TmplT<rpos>::value
 */
template <VaTmplType TmplT, CmpOp _cmp_op>
using ct_pos_value_cmp_tt = typename _tmp_impl::ct_pos_value_comparator<
    TmplT, _cmp_op>::ttype;

/**
 * TmplT<pos, pos - 1>::value
 */
template <VaTmplType TmplT, std::size_t _max_pos,
    bool _default = false, bool _first = !_default>
using ct_pos_forward_cmp_tt = typename _tmp_impl::ct_pos_forward_comparator<
    TmplT, _max_pos, _default, _first>::ttype;
}

