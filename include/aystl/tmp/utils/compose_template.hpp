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
template <MetaTmplType...> struct compose_template_t;

template <MetaTmplType... TmplTs>
using compose_tmpl_t = compose_template_t<TmplTs...>;
using compose_tmpl_tt = ty_tmpl_t<compose_tmpl_t>;

namespace detail {
template <typename T>
using is_compose_tmpl = is_spec_of<T, compose_template_t>;
template <typename T> requires is_compose_tmpl<T>::value
struct is_meta_tmpl<T> : bool_constant<true> {};
}
template <typename T>
concept ComposeTmplType = MetaTmplType<T> && detail::is_compose_tmpl<T>::value;

namespace _tmp_impl {
template <typename T, MetaTmplType...>
struct compose_tmpl_wrap_impl : type_t<T> {};

template <MetaListType T, MetaTmplType TmplT, MetaTmplType... TmplTs>
struct compose_tmpl_wrap_impl<T, TmplT, TmplTs...> {
    using _a1_type = meta_wrap_t<TmplT, T>;
    using _a2_type = cond_t<MetaListType<_a1_type>, _a1_type, type_list<_a1_type>>;
    using _a3_type = cond_t<ct_cmp_v<CmpOp::kEQ, sizeof...(TmplTs), 0>, _a1_type, _a2_type>;
    using type = typename compose_tmpl_wrap_impl<_a3_type, TmplTs...>::type;
};

template <typename T, TyListType TmplsT>
struct compose_tmpl_wrap {
    template <MetaTmplType... _TmplTs>
    using __impl_t = typename compose_tmpl_wrap_impl<T, _TmplTs...>::type;
    using _tmpls_type = meta_list_reverse_t<TmplsT>;
    using type = _tmpls_type::template wrapped<ty_tmpl_t<__impl_t>>;
};
}

template <MetaTmplType... TmplTs>
struct compose_template_t {
    using type = compose_template_t;
    
    using tmpls_type = type_list<TmplTs...>;

    template <MetaListType _Tp>
    using wrap = typename _tmp_impl::compose_tmpl_wrap<_Tp, tmpls_type>::type; 

    template <typename>
    using is_wrapped_to = bool_constant<false>;
};
}

