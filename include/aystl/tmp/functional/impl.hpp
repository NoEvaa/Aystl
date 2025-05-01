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

#include <array>
#include <algorithm>

#include "aystl/tmp/functional/ct_sorted_array.hpp"
#include "aystl/tmp/functional/comparator.hpp"

#include "aystl/tmp/type_traits.hpp"
#include "aystl/tmp/meta.hpp"

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

    static constexpr index_constant<sizeof...(Vs)> size;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    static constexpr element_type at_v = std::get<pos>(value);
    template <std::size_t pos>
    using at = constant_t<element_type, at_v<pos>>;

    using to_constant_list = typename make_index_seq<size()>
        ::template co_map<va_tmpl_t<at>, element_type>;
};

namespace _tmp_impl {
template <VaTmplType TmplT, CmpOp _cmp_op>
struct ct_pos_value_comparator {
    template <std::size_t lpos, std::size_t rpos>
    using __impl = ct_cmp<_cmp_op, 
        va_wrap_t<TmplT, lpos>::value, va_wrap_t<TmplT, rpos>::value>;
    using ttype = va_tmpl_t<__impl>;
};

template <VaTmplType TmplT, std::size_t _max_pos,
    bool _default, bool _first>
struct ct_pos_forward_comparator {
    template <std::size_t pos>
    struct __impl : constant_t<bool, _default> {};
    template <std::size_t pos>
    requires CtCmp<CmpOp::kEQ, pos, 0> && CtCmp<CmpOp::kLT, 0, _max_pos>
    struct __impl<pos> : constant_t<bool, _first> {};
    template <std::size_t pos>
    requires CtCmp<CmpOp::kLT, 0, pos> && CtCmp<CmpOp::kLT, pos, _max_pos>
    struct __impl<pos> : constant_t<bool, va_wrap_t<TmplT, pos, pos - 1>::value> {};

    using ttype = va_tmpl_t<__impl>;
};
}
}

