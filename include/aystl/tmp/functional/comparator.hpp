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
template <VaTmplType TmplT, std::size_t _max_pos,
    CmpOp _cmp_op = CmpOp::kNE, bool _default = false, bool _first = !_default>
struct ct_pos_forward_comparator;
}

