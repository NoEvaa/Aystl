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

#include <functional>

#include "aystl/tmp/meta/base.hpp"

namespace iin {
namespace _tmp_impl {
template <CoListType T, CoListType... Ts>
struct constant_list_cat : type_t<T> {};

template <CoListType, typename AlgoT> struct constant_list_apply_algo;
template <CoListType, typename CmpT> struct constant_list_sort;
template <CoListType> struct constant_list_sorted_unique;
}

template<typename T, T... Vs>
struct constant_list : detail::basic_meta_list<constant_list<T, Vs...>> {
    using type       = constant_list;
    using value_type = T;
    using empty_type = constant_list<value_type>;

    static constexpr index_constant<sizeof...(Vs)> size;

    template <typename _Tp>
    using cast = constant_list<_Tp, static_cast<_Tp>(Vs)...>;

    template <MetaTmplType TmplT>
    using map = meta_list_map_t<type, constant_list<value_type>, TmplT>;

    template <CoListTType<value_type>... _Ts>
    using concat = typename _tmp_impl::constant_list_cat<type, _Ts...>::type;

    template <value_type... _Vs>
    using xpush_back  = constant_list<value_type, Vs..., _Vs...>;
    template <value_type... _Vs>
    using xpush_front = constant_list<value_type, _Vs..., Vs...>;

    template <typename _AlgoT>
    using apply_algo = typename _tmp_impl::constant_list_apply_algo<type, _AlgoT>::type;

    template <typename _CmpT = std::less<>>
    using sort = typename _tmp_impl::constant_list_sort<type, _CmpT>::type;

    template <typename _Cmp = std::less<>>
    using unique_sort = typename _tmp_impl::constant_list_sorted_unique<sort<_Cmp>>::type;
};
}

