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

template <MetaTmplType TmplT, TyListType ArgsT>
struct currying_template_t {
    using type = currying_template_t;

    using tmpl_type = TmplT;
    using args_type = ArgsT;

    template <TyListType _ArgsT = type_list<>>
    using wrap = meta_wrap_t<tmpl_type, placeholders_bind_t<args_type, _ArgsT>>; 
    template <typename... _ArgTs>
    using xwrap = wrap<type_list<_ArgTs...>>; 

    template <typename>
    using is_wrapped_to = bool_constant<false>;

    template <MetaTmplType _TmplT>
    using change_tmpl = currying_tmpl_t<_TmplT, args_type>;
    template <TyListType _ArgsT>
    using change_args = currying_tmpl_t<tmpl_type, _ArgsT>;

    template <TyListType _ArgsT>
    using bind = change_args<placeholders_bind_t<args_type, _ArgsT>>;
    template <typename... _ArgTs>
    using xbind = bind<type_list<_ArgTs...>>; 
};
}

