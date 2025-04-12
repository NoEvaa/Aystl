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
#include "aystl/core/arch/base.hpp"

namespace iin {
template <CmpOp, typename T>
struct AyCmp {
    template <typename _Tp>
    bool operator()(_Tp &&, _Tp &&) const noexcept {
        AY_UNREACHABLE("Undefined compare operation.");
        return false;
    }
};

template <CmpOp _cmp_op>
struct AyArch<value_t<_cmp_op>> : AyArchBaseVT<CmpOp, value_t<_cmp_op>, AyCmp> {};
template <CmpOp _cmp_op>
using arch_cmp_t = AyArch<value_t<_cmp_op>>;

#define _AYSTL_DECL_COMPARE_DEF(_op_name, ...)                                                     \
    template <typename T>                                                                          \
    requires requires (T const & lhs, T const & rhs) { lhs __VA_ARGS__ rhs; }                      \
    struct AyCmp<CmpOp::k##_op_name, T> {                                                          \
        bool operator()(T const & lhs, T const & rhs) const noexcept {                             \
            return lhs __VA_ARGS__ rhs;                                                            \
        }                                                                                          \
    };
#include "aystl/global/compare_def.inl"
#undef _AYSTL_DECL_COMPARE_DEF

namespace detail {
template <CmpOp _cmp_op>
inline constexpr bool _has_fuzzy_eq_v = ((_cmp_op & CmpOp::kFuzzyEQ) == CmpOp::kFuzzyEQ); 
}

template <typename T>
struct AyCmp<CmpOp::kFuzzyEQ, T> {
    bool operator()(T const & lhs, T const & rhs) const noexcept {
        return AyCmp<CmpOp::kEQ, T>{}(lhs, rhs);
    }
};

template <CmpOp _cmp_op, typename T>
requires detail::_has_fuzzy_eq_v<_cmp_op>
struct AyCmp<_cmp_op, T> {
    bool operator()(T const & lhs, T const & rhs) const noexcept {
        return AyCmp<_cmp_op & ~CmpOp::kFuzzyEQ, T>{}(lhs, rhs) ||
            AyCmp<CmpOp::kFuzzyEQ, T>{}(lhs, rhs);
    }
};

template <typename T>
bool ayCompare(CmpOp cmp_op, T const & lhs, T const & rhs) noexcept {

#define _AYSTL_DECL_COMPARE_DEF(_op_name, ...)                                                     \
    case CmpOp::k##_op_name: return AyCmp<CmpOp::k##_op_name, T>{}(lhs, rhs);

    switch (cmp_op) {
#include "aystl/global/compare_def.inl"
    _AYSTL_DECL_COMPARE_DEF(FuzzyEQ)
    _AYSTL_DECL_COMPARE_DEF(FuzzyLE)
    _AYSTL_DECL_COMPARE_DEF(FuzzyGE)
    default:
        AY_UNREACHABLE();
        return false;
    }

#undef _AYSTL_DECL_COMPARE_DEF
}
}

