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

#include "aystl/tmp/meta/base.hpp"

namespace iin {
template <auto... Vs>
struct value_list : detail::basic_meta_list<value_list<Vs...>> {
    using type       = value_list;
    using empty_type = value_list<>;

    static constexpr index_constant<sizeof...(Vs)> size;

    template <MetaTmplType TmplT>
    using map = meta_list_map_t<type, value_list<>, TmplT>;

    template <auto... _Vs> using push_back  = value_list<Vs..., _Vs...>;
    template <auto... _Vs> using push_front = value_list<_Vs..., Vs...>;
};
}

