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

#include <functional>

#include "aystl/core/arch/base.hpp"

namespace iin {
template <typename RetT, typename T>
struct AyHash;

template <class T>
struct AyHash<std::size_t, T> {
    std::size_t operator()(T const & _v) const noexcept {
        return std::hash<T>{}(_v);
    }
};

struct arch_hash;
template <typename RetT>
struct AyArch<arch_hash, RetT> : AyArchBaseTT<AyHash, RetT> {};
template <typename RetT>
using arch_hash_t = AyArch<arch_hash, RetT>;
}

