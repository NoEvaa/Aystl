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

#include <algorithm>

#include "aystl/tmp/meta/constant_list.hpp"
#include "aystl/tmp/meta/int_seq.hpp"
#include "aystl/tmp/utils/compare.hpp"

namespace iin {
template <typename _Cmp, typename T, T... Vs>
struct ct_sorted_array {
    using element_type = T;
    using value_type   = std::array<element_type, sizeof...(Vs)>;

    static constexpr value_type value = [] {
        value_type temp = { Vs... };
        std::sort(temp.begin(), temp.end(), _Cmp{});
        return temp;
    }();

    static constexpr std::size_t size() noexcept { return sizeof...(Vs); }

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    static constexpr element_type at = std::get<pos>(value);
    template <std::size_t pos>
    using at_t = constant_t<element_type, at<pos>>;

    using to_constant_list = typename make_index_seq<size()>
        ::template constant_map<element_type, at_t>;
};
}

