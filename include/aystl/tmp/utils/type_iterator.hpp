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
template <MetaListType T>
struct type_iterator {
    template <std::size_t pos>
    struct __impl {
        static constexpr index_constant<pos> current_pos;

        using type = va_wrap_t<meta_list_get_tt<T>, pos>;

        using is_begin = ct_cmp<CmpOp::kEQ, pos, 0>;
        using is_end   = ct_cmp<CmpOp::kGE, pos, T::size()>;

        template <std::size_t _offset = 1>
        using next = __impl<(pos + _offset)>;
        template <std::size_t _offset>
        using prev = __impl<(pos < _offset ? 0 : pos - _offset)>;
        template <int _offset = 1>
        using advance = cond_t<ct_cmp_v<CmpOp::kLT, _offset, 0>,
            prev<static_cast<std::size_t>(-_offset)>,
            next<static_cast<std::size_t>(_offset)>>;
    };
    using ttype = va_tmpl_t<__impl>;
};

template <MetaListType T>
using ty_iter_tt = typename type_iterator<T>::ttype;
template <MetaListType T, std::size_t pos>
using ty_iter_t = va_wrap_t<ty_iter_tt<T>, pos>;
}

