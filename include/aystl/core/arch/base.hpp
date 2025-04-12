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

#include "aystl/core/type_traits/template.hpp"

namespace iin {
template <class T>
struct AyArch;

template <typename ArchT, typename... Ts>
requires is_spec_of_v<ArchT, AyArch>
using build_arch_t = typename ArchT::template build<Ts...>;

template <typename ArchT, typename... Ts>
decltype(auto) ayArchCall(auto &&... _args) {
    return build_arch_t<ArchT, Ts...>{}(std::forward<decltype(_args)>(_args)...);
}

template <template <typename...> typename Tmpl>
struct AyArchBaseT {
    template <typename... Ts>
    using build = Tmpl<Ts...>;
};

template <typename VT, ConstantTType<VT> T, template <VT, typename...> typename Tmpl>
struct AyArchBaseVT {
    template <typename... Ts>
    using build = Tmpl<T::value, Ts...>;
};
}

