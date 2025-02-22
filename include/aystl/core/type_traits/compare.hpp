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

#include "aystl/global/common.hpp"

#define _AYSTL_DECL_TMPL_CT_CMP(_op_name, ...)                                                     \
    template <auto _left, auto _right> requires (_left __VA_ARGS__ _right)                         \
    struct ct_cmp<CmpOp::k##_op_name, _left, _right> : std::true_type {};

namespace iin {
template <CmpOp _op, auto _left, auto _right>
struct ct_cmp : std::false_type {};

_AYSTL_DECL_TMPL_CT_CMP(EQ, ==)
_AYSTL_DECL_TMPL_CT_CMP(NE, !=)
_AYSTL_DECL_TMPL_CT_CMP(LT, <)
_AYSTL_DECL_TMPL_CT_CMP(LE, <=)
_AYSTL_DECL_TMPL_CT_CMP(GT, >)
_AYSTL_DECL_TMPL_CT_CMP(GE, >=)

template <CmpOp _op, auto _left, auto _right>
inline constexpr bool ct_cmp_v = ct_cmp<_op, _left, _right>::value;

template <CmpOp _op, auto _left, auto _right>
concept CtCmp = ct_cmp_v<_op, _left, _right>;
}

#undef _AYSTL_DECL_TMPL_CT_CMP

