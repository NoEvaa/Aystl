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

namespace iin {
enum class CmpOp {
    kEQ = 0,
    kNE,
    kLT,
    kLE,
    kGT,
    kGE
};

template <CmpOp _op, auto _left, auto _right>
struct ct_cmp : std::false_type {};

template <auto _left, auto _right>
requires (_left == _right)
struct ct_cmp<CmpOp::kEQ, _left, _right> : std::true_type {};

template <auto _left, auto _right>
requires (_left != _right)
struct ct_cmp<CmpOp::kNE, _left, _right> : std::true_type {};

template <auto _left, auto _right>
requires (_left < _right)
struct ct_cmp<CmpOp::kLT, _left, _right> : std::true_type {};

template <auto _left, auto _right>
requires (_left <= _right)
struct ct_cmp<CmpOp::kLE, _left, _right> : std::true_type {};

template <auto _left, auto _right>
requires (_left > _right)
struct ct_cmp<CmpOp::kGT, _left, _right> : std::true_type {};

template <auto _left, auto _right>
requires (_left >= _right)
struct ct_cmp<CmpOp::kGE, _left, _right> : std::true_type {};

template <CmpOp _op, auto _left, auto _right>
constexpr bool ct_cmp_v = ct_cmp<_op, _left, _right>::value;

template <CmpOp _op, auto _left, auto _right>
concept CtCmp = ct_cmp_v<_op, _left, _right>;
}
