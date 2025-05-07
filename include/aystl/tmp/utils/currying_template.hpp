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
#include "aystl/tmp/utils/placeholder.hpp"

namespace iin {
namespace _tmp_impl {
template <TyListType OldT, TyListType NewT>
struct tmpl_args_bind {
    using _plhs_type = sorted_placeholders_t<OldT>
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
}

