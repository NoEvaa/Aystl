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
template <MetaTmplType, TyListType> struct currying_template_t;

template <MetaTmplType TmplT, TyListType TmplArgs>
using currying_tmpl_t = currying_template_t<TmplT, TmplArgs>;

namespace detail {
template <typename T>
using is_currying_tmpl = is_spec_of<T, currying_template_t>;
template <typename T> requires is_currying_tmpl<T>::value
struct is_meta_tmpl<T> : bool_constant<true> {};
}
template <typename T>
concept CurryingTmplType = MetaTmplType<T> && detail::is_currying_tmpl<T>::value;

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
template <TyListType OldT, TyListType NewT>
using tmpl_args_bind_t = typename tmpl_args_bind<OldT, NewT>::type;
}

template <MetaTmplType TmplT, TyListType TmplArgs>
struct currying_template_t {
    using tmpl_type = TmplT;
    using args_type = TmplArgs;

    template <TyListType _Tp>
    using wrap = meta_wrap_t<tmpl_type, _tmp_impl::tmpl_args_bind_t<args_type, _Tp>>; 

    template <typename>
    using is_wrapped_to = bool_constant<false>;

    template <MetaTmplType _TmplT>
    using change_args = currying_tmpl_t<_TmplT, args_type>;
    template <TyListType _TmplArgs>
    using change_tmpl = currying_tmpl_t<tmpl_type, _TmplArgs>;

    template <TyListType _TmplArgs>
    using bind = change_args<_tmp_impl::tmpl_args_bind_t<args_type, _TmplArgs>>;
};
}

