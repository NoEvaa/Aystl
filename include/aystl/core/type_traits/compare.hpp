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

#include <functional>
#include <type_traits>

#include "aystl/global/common.hpp"
#include "aystl/core/type_traits/utils.hpp"

#define _AYSTL_DECL_CONCEPT_CMPABLE(_op_name, ...)                                                 \
    template <class T>                                                                             \
    requires requires (add_clref<T> _v) {                                                          \
        { _v __VA_ARGS__ _v } -> std::convertible_to<bool>;                                        \
    }                                                                                              \
    struct is_comparable<CmpOp::k##_op_name, T> : std::true_type {};

#define _AYSTL_DECL_TMPL_CT_CMP(_op_name, ...)                                                     \
    template <auto _left, auto _right> requires (_left __VA_ARGS__ _right)                         \
    struct ct_cmp<CmpOp::k##_op_name, _left, _right> : std::true_type {};

namespace iin {
template <CmpOp, class T>
struct is_comparable : std::false_type {};

_AYSTL_DECL_CMP_OPS(_AYSTL_DECL_CONCEPT_CMPABLE)

template <CmpOp _op, class T>
inline constexpr bool is_comparable_v = is_comparable<_op, T>::value;

template <CmpOp _op, auto _left, auto _right>
struct ct_cmp : std::false_type {};

_AYSTL_DECL_CMP_OPS(_AYSTL_DECL_TMPL_CT_CMP)

template <CmpOp _op, auto _left, auto _right>
inline constexpr bool ct_cmp_v = ct_cmp<_op, _left, _right>::value;

template <CmpOp _op, auto _left, auto _right>
concept CtCmp = ct_cmp_v<_op, _left, _right>;
}

#undef _AYSTL_DECL_CONCEPT_CMPABLE
#undef _AYSTL_DECL_TMPL_CT_CMP

