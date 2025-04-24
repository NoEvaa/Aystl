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

namespace iin {
template <std::size_t pos>
struct placeholder_t : value_t<pos> {};
template <std::size_t pos>
using plh_t = placeholder_t<pos>;
template <typename T>
using is_placeholder = is_value_spec_of<T, placeholder_t>;
template <typename T>
concept PlaceholderType = is_placeholder<T>::value;

namespace detail {
template <TypeListType T>
struct sorted_placeholders {
    using _mask_type = typename T::template map<is_placeholder>;
    using _plhs_type = typename T::template filter<_mask_type>;
    using _values_type = typename _plhs_type
        ::template constant_map<std::size_t, transfer_value_t>;
    using _sorted_values_type = typename _values_type::template sort<>;
    using _sorted_plhs_type = typename _sorted_values_type
        ::template type_map<plh_t>;

    using type = _sorted_plhs_type;
};
}
template <TypeListType T>
using sorted_placeholders_t = typename detail::sorted_placeholders<T>::type;
}

