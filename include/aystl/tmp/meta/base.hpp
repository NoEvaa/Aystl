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

#include "aystl/tmp/meta/type.hpp"
#include "aystl/tmp/meta/template.hpp"
#include "aystl/tmp/type_traits/compare.hpp"

namespace iin {
namespace _tmp_impl {
template <MetaListType, typename...> struct meta_list_push_back;
template <MetaListType, typename> struct meta_list_get;
template <MetaListType> struct meta_list_reverse;

template <MetaListType InT, MetaListType OutT,
    MetaTmplType TmplT, typename... TmplArgs>
struct meta_list_map;
}
template <MetaListType T, typename DefaultT = null_t>
using meta_list_get_tt = typename _tmp_impl::meta_list_get<T, DefaultT>::ttype;

template <MetaListType T>
using meta_list_reverse_t = typename _tmp_impl::meta_list_reverse<T>::type;

template <MetaListType InT, MetaListType OutT,
    MetaTmplType TmplT, typename... TmplArgs>
using meta_list_map_t = typename _tmp_impl::meta_list_map<
    InT, OutT, TmplT, TmplArgs...>::type;
}

