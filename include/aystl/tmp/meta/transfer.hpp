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

namespace iin {
template <ValueTType T>
using transfer_value_t = value_t<T::value>;
using transfer_value_tt = ty_tmpl_t<transfer_value_t>;

template <typename T>
using transfer_value_type_t = type_t<typename T::value_type>;
using transfer_value_type_tt = ty_tmpl_t<transfer_value_type_t>;

namespace detail {
template <typename T>
struct transfer_type : type_t<T> {};
template <TypeTType T>
struct transfer_type<T> : type_t<typename T::type> {};
}
template <typename T>
using transfer_type_t = type_t<typename detail::transfer_type<T>::type>;
using transfer_type_tt = ty_tmpl_t<transfer_type_t>;
}

