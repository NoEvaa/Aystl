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
#include "aystl/tmp/functional/ct_range.hpp"

namespace iin {
namespace _tmp_impl {
template <TyListType T, TyListType... Ts>
struct type_list_cat : type_t<T> {};

template <TyListType T, IntSeqType RangeT>
struct type_list_slice;
}
template <TyListType... Ts>
using type_list_cat_t = typename _tmp_impl::type_list_cat<Ts...>::type;

template <typename... Ts>
struct type_list : detail::basic_meta_list<type_list<Ts...>> {
    using type       = type_list;
    using empty_type = type_list<>;

    static constexpr index_constant<sizeof...(Ts)> size;

    template <TyTmplType TmplT, typename... TmplArgs>
    using map = meta_list_map_t<type, type_list<>, TmplT, TmplArgs...>;

    template <TyListType... _Ts>
    using concat = type_list_cat_t<type, _Ts...>;

    template <typename... _Ts> using xpush_back  = type_list<Ts..., _Ts...>;
    template <typename... _Ts> using xpush_front = type_list<_Ts..., Ts...>;

    template <IntSeqType RangeT>
    using slice = typename _tmp_impl::type_list_slice<type, RangeT>::type;
    template <auto lpos, auto rpos = size()>
    using slice_range = slice<ct_range_t<
        static_cast<int>(lpos), static_cast<int>(rpos)>>;

    template <std::size_t pos, typename... _Ts>
    requires CtCmp<CmpOp::kLE, pos, size()>
    using insert = type_list_cat_t<
        slice_range<0, pos>, type_list<_Ts...>, slice_range<pos>>;

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    using erase = type_list_cat_t<slice_range<0, pos>, slice_range<pos + 1>>;

    template <typename OldT, typename NewT>
    using replace = map<ty_tmpl_t<replace_if_same_as_t>, OldT, NewT>;
};
}

