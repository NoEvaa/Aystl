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

#include "aystl/tmp/meta.hpp"
#include "aystl/tmp/functional.hpp"

namespace iin {
template <std::size_t _pos>
struct placeholder_t : value_t<_pos> {};
template <std::size_t pos>
using plh_t = placeholder_t<pos>;
template <typename T>
using is_placeholder = is_value_spec_of<T, placeholder_t>;
template <typename T>
concept PlaceholderType = is_placeholder<T>::value;

namespace detail {
template <TyListType T>
struct sorted_placeholders {
    using _mask_type = T::template map<ty_tmpl_t<is_placeholder>>;
    using _plhs_type = T::template filter<_mask_type>;
    using _values_type = _plhs_type::template co_map<transfer_value_tt, std::size_t>;
    using type = _values_type::template unique_sort<>;
};
}
template <TyListType T>
using sorted_placeholders_t = typename detail::sorted_placeholders<T>::type;
}

