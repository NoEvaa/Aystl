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
template <TyListType OldT, TyListType NewT>
struct placeholders_bind {
    using _plhs_type = OldT
        ::template filter<ty_tmpl_t<is_placeholder>>
        ::template co_map<transfer_value_tt, std::size_t>
        ::template unique_sort<>
        ::template wrapped<ct_array_tt<detail::ct_std_pass_t>>;

    template <std::size_t pos>
    using _get_from_new = va_wrap_t<meta_list_get_tt<NewT>, pos>;

    template <typename T>
    struct __impl : type_t<T> {};
    template <PlaceholderType T>
    struct __impl<T> {
        static constexpr auto _pos_of_new = _plhs_type::template findFirst<T::value>();
        using type = cond_t<ct_cmp_v<CmpOp::kLT, _pos_of_new, NewT::size()>,
            _get_from_new<_pos_of_new>, T>;
    };
    template <typename T>
    using __impl_t = typename __impl<T>::type;

    using type = OldT::template map<ty_tmpl_t<__impl_t>>;
};
}
template <TyListType OldT, TyListType NewT>
using placeholders_bind_t = typename detail::placeholders_bind<OldT, NewT>::type;
}

