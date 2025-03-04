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

#define _AYSTL_DECL_TMPL_AY_CMP(_op_name, ...)                                                     \
    template <typename T>                                                                          \
    requires requires (T const & lhs, T const & rhs) { lhs __VA_ARGS__ rhs; }                      \
    struct AyCmp<CmpOp::k##_op_name, T> {                                                          \
        bool operator()(T const & lhs, T const & rhs) {                                            \
            return lhs __VA_ARGS__ rhs;                                                            \
        }                                                                                          \
    };

namespace iin {
template <CmpOp, typename>
struct AyCmp;

_AYSTL_DECL_CMP_OPS(_AYSTL_DECL_TMPL_AY_CMP)
}

#undef _AYSTL_DECL_TMPL_AY_CMP

