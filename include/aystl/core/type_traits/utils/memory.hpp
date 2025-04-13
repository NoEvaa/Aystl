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

#include <memory>

#include "aystl/core/type_traits/template.hpp"

namespace iin {
template <typename T>
inline constexpr bool is_std_smart_ptr_v = is_any_spec_of_v<T,
    std::unique_ptr, std::shared_ptr, std::weak_ptr>;

template <typename T>
struct remove_smart_ptr : type_t<T> {};

template <typename T>
requires is_std_smart_ptr_v<T>
struct remove_smart_ptr<T> : type_t<unwrap_tmpl_t<T>> {};

template <typename T>
using remove_smart_ptr_t = typename remove_smart_ptr<T>::type;

template <typename T>
using wrap_sptr_t = wrap_tmpl_t<std::shared_ptr, T>;
template <typename T>
using wrap_wptr_t = wrap_tmpl_t<std::weak_ptr, T>;
template <typename T>
using wrap_uptr_t = wrap_tmpl_t<std::unique_ptr, T>;
}

