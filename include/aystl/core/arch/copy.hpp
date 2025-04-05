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

#include <type_traits>
#include <memory>

#include "aystl/global/common.hpp"
#include "aystl/core/type_traits/utils.hpp"
#include "aystl/core/arch/base.hpp"

namespace iin {
template <CpyOp, typename T>
struct AyCpy : value_t<false> {
    template <typename _Tp>
    void operator()(_Tp &&, _Tp &&) noexcept {
        AY_UNREACHABLE("Undefined copy operation.");
    }
};

template <CpyOp _cpy_op>
struct AyArch<value_t<_cpy_op>> : AyArchBaseVT<CpyOp, value_t<_cpy_op>, AyCpy> {};
template <CpyOp _cpy_op>
using arch_cpy_t = AyArch<value_t<_cpy_op>>;

template <typename T>
requires requires (T & dst, T const & src) { dst = src; }
struct AyCpy<CpyOp::kCopy, T> : value_t<true> {
    void operator()(T dst, T const & src) noexcept {
        dst = src;
    }
};

template <typename T>
requires requires (T & dst, T && src) { dst = std::move(src); }
struct AyCpy<CpyOp::kMove, T> : value_t<true> {
    void operator()(T dst, T && src) noexcept {
        dst = std::move(src);
    }
};

template <typename T>
requires std::is_trivially_copyable_v<T>
struct AyCpy<CpyOp::kMemory, T> : value_t<true> {
    void operator()(T dst, T const & src) noexcept {
        std::memcpy(std::addressof(dst), std::addressof(src), sizeof(T));
    }
};

template <typename T>
struct AyCpy<CpyOp::kDeep, T> : value_t<true> {
    void operator()(T dst, T const & src) noexcept {
        AyCpy<CpyOp::kCopy, T>{}(dst, src);
    }
};

template <typename T>
struct AyCpy<CpyOp::kShallow, T> : value_t<true> {
    void operator()(T dst, T const & src) noexcept {
        AyCpy<CpyOp::kCopy, T>{}(dst, src);
    }
};
}

