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
#include "aystl/tmp/meta/meta_decl.hpp"
#include "aystl/tmp/meta/type_list.hpp"
#include "aystl/tmp/meta/value_list.hpp"
#include "aystl/tmp/meta/constant_list.hpp"

namespace iin {
namespace _tmp_impl {
template <TyListType T, typename... NextT>
struct meta_list_push_back<T, NextT...> {
    using type = T::template push_back<NextT...>;
};
template <VaListType T, ValueTType... NextT>
struct meta_list_push_back<T, NextT...> {
    using type = T::template push_back<NextT::value...>;
};
template <CoListType T, ValueTType... NextT>
struct meta_list_push_back<T, NextT...> {
    using value_type = typename T::value_type;
    using type = T::template push_back<static_cast<value_type>(NextT::value)...>;
};

template <MetaListType InT, MetaListType OutT, TyTmplType TmplT, typename... TmplArgs>
requires CtCmp<CmpOp::kEQ, InT::size(), 0>
struct meta_list_map<InT, OutT, TmplT, TmplArgs...> : type_t<OutT> {};

template <typename... Ts, MetaListType OutT, TyTmplType TmplT, typename... TmplArgs>
requires CtCmp<CmpOp::kGT, sizeof...(Ts), 0>
struct meta_list_map<type_list<Ts...>, OutT, TmplT, TmplArgs...> {
    using type = typename meta_list_push_back<
        OutT, ty_wrap_t<TmplT, Ts, TmplArgs...>...>::type;
};

template <auto... Vs, MetaListType OutT, VaTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<value_list<Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, va_wrap_t<TmplT, Vs>...>::type;
};

template <typename VT, VT... Vs, MetaListType OutT, CoTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<constant_list<VT, Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, co_wrap_t<TmplT, VT, Vs>...>::type;
};

template <typename... Ts, typename... Ts2>
auto _concat_two_type_list(type_list<Ts...>, type_list<Ts2...>)
    -> type_list<Ts..., Ts2...>;

template <TyListType T1, TyListType T2, TyListType... Ts>
struct type_list_cat<T1, T2, Ts...> {
    using type = typename type_list_cat<
        decltype(_concat_two_type_list(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};

template <TyListType T, std::size_t pos, typename DefaultT>
requires CtCmp<CmpOp::kLT, pos, T::size()>
struct type_list_get<T, pos, DefaultT> : type_t<typename T::template at<pos>> {};

template <TyListType InT, TyListType MaskT, TyListType OutT, std::size_t pos>
requires CtCmp<CmpOp::kLT, pos, InT::size()> && CtCmp<CmpOp::kLT, pos, MaskT::size()>
struct type_list_filter<InT, MaskT, OutT, pos> {
    using _elem_type  = typename InT::template at<pos>;
    using _filt_type  = typename MaskT::template at<pos>;
    using _t_out_type = typename OutT::template push_back<_elem_type>;
    using _f_out_type = OutT;
    using _out_type   = std::conditional_t<
        static_cast<bool>(_filt_type::value), _t_out_type, _f_out_type>;

    using type = typename type_list_filter<InT, MaskT, _out_type, pos + 1>::type;
};
}
}

