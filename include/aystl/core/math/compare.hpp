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

#include "aystl/global/common.hpp"
#include "aystl/global/enum.hpp"

#define _AYSTL_DECL_TMPL_AY_CMP(_op_name, ...)                                                     \
    template <typename T>                                                                          \
    requires requires (T const & lhs, T const & rhs) { lhs __VA_ARGS__ rhs; }                      \
    struct AyCmp<CmpOp::k##_op_name, T> {                                                          \
        bool operator()(T const & lhs, T const & rhs) noexcept {                                   \
            return lhs __VA_ARGS__ rhs;                                                            \
        }                                                                                          \
    };

namespace iin {
template <CmpOp, typename>
struct AyCmp;

_AYSTL_DECL_CMP_OPS(_AYSTL_DECL_TMPL_AY_CMP)

namespace detail {
template <CmpOp _cmp_op>
inline constexpr bool _has_fuzzy_eq_v = ((_cmp_op & CmpOp::kFuzzyEQ) == CmpOp::kFuzzyEQ); 
}

template <typename T>
struct AyCmp<CmpOp::kFuzzyEQ, T> {
    bool operator()(T const & lhs, T const & rhs) noexcept {
        return AyCmp<CmpOp::kEQ, T>{}(lhs, rhs);
    }
};

template <CmpOp _cmp_op, typename T>
requires detail::_has_fuzzy_eq_v<_cmp_op>
struct AyCmp<_cmp_op, T> {
    bool operator()(T const & lhs, T const & rhs) {
        return AyCmp<_cmp_op & ~CmpOp::kFuzzyEQ, T>{}(lhs, rhs) ||
            AyCmp<CmpOp::kFuzzyEQ, T>{}(lhs, rhs);
    }
};
}

#undef _AYSTL_DECL_TMPL_AY_CMP

