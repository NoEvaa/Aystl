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
#include <type_traits>

#include "aystl/type_traits/utils.hpp"

namespace iin {
template <typename T>
struct remove_smart_ptr {
    using type = T;
};

template <typename T>
struct remove_smart_ptr<std::shared_ptr<T>> {
    using type = T;
};

template <typename T>
struct remove_smart_ptr<std::weak_ptr<T>> {
    using type = T;
};

template <typename T>
struct remove_smart_ptr<std::unique_ptr<T>> {
    using type = T;
};

template <typename T>
using remove_smart_ptr_t = remove_smart_ptr<T>;

template <typename T>
using wrap_sptr_t = wrap_tmpl_t<std::shared_ptr, T>;
template <typename T>
using wrap_wptr_t = wrap_tmpl_t<std::weak_ptr, T>;
template <typename T>
using wrap_uptr_t = wrap_tmpl_t<std::unique_ptr, T>;
}

