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

#include "aystl/core/arch/base.hpp"

namespace iin {
template <CastOp, typename, typename>
struct AyCast;

template <CastOp _cast_op>
struct AyArch<value_t<_cast_op>> : AyArchBaseVT<CastOp, value_t<_cast_op>, AyCast> {};
template <CastOp _cast_op>
using arch_cast_t = AyArch<value_t<_cast_op>>;
}

