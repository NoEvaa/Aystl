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
#include "aystl/tmp/meta/utils.hpp"
#include "aystl/tmp/meta/type_list.hpp"
#include "aystl/tmp/meta/value_list.hpp"
#include "aystl/tmp/meta/constant_list.hpp"
#include "aystl/tmp/meta/int_seq.hpp"
#include "aystl/tmp/functional/comparator.hpp"

namespace iin {
namespace _tmp_impl {
template <TyTmplType T, typename... Ts>
struct ty_wrap<T, Ts...> {
    using type = T::template wrap<Ts...>;
};
template <VaTmplType T, typename... Ts>
requires is_all_of_v<constant_t<bool, ValueTType<Ts>>...>
struct ty_wrap<T, Ts...> {
    using type = T::template wrap<Ts::value...>;
};
/*template <CoTmplType T, typename VT, typename... Ts>
requires is_all_of_v<constant_t<bool, ValueTType<Ts>>...>
struct ty_wrap<T, VT, Ts...> {
    using type = typename T::template wrap<
        VT, static_cast<VT>(Ts::value)...>;
};*/

template <VaTmplType T, auto... Vs>
struct va_wrap<T, Vs...> {
    using type = T::template wrap<Vs...>;
};
template <TyTmplType T, auto... Vs>
struct va_wrap<T, Vs...> {
    using type = T::template wrap<value_t<Vs>...>;
};

template <CoTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = T::template wrap<VT, Vs...>;
};
template <VaTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = T::template wrap<Vs...>;
};
template <TyTmplType T, typename VT, VT... Vs>
struct co_wrap<T, VT, Vs...> {
    using type = T::template wrap<constant_t<VT, Vs>...>;
};

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

template <MetaListType T, typename DefaultT>
struct meta_list_get {
    template <std::size_t pos>
    struct __impl : type_t<DefaultT> {};
    template <std::size_t pos>
    requires CtCmp<CmpOp::kLT, pos, T::size()>
    struct __impl<pos> : type_t<typename T::template at<pos>> {};
    template <std::size_t pos>
    using __impl_t = typename __impl<pos>::type;

    using ttype = va_tmpl_t<__impl_t>;
};

template <MetaListType T>
struct meta_list_iter {
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

template <MetaListType InT, MetaListType OutT, MetaTmplType TmplT, typename... TmplArgs>
requires CtCmp<CmpOp::kEQ, InT::size(), 0>
struct meta_list_map<InT, OutT, TmplT, TmplArgs...> : type_t<OutT> {};

template <typename... Ts, MetaListType OutT, TyTmplType TmplT, typename... TmplArgs>
requires CtCmp<CmpOp::kGT, sizeof...(Ts), 0>
struct meta_list_map<type_list<Ts...>, OutT, TmplT, TmplArgs...> {
    template <typename _Tp>
    using _wrap_tmpl = ty_wrap_t<TmplT, _Tp, TmplArgs...>;

    using type = typename meta_list_push_back<OutT, _wrap_tmpl<Ts>...>::type;
};
template <typename... Ts, MetaListType OutT, VaTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Ts), 0>
struct meta_list_map<type_list<Ts...>, OutT, TmplT> {
    using type = typename meta_list_push_back<OutT, ty_wrap_t<TmplT, Ts>...>::type;
};
template <typename... Ts, MetaListType OutT, CoTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Ts), 0>
struct meta_list_map<type_list<Ts...>, OutT, TmplT> {
    template <typename _Tp>
    using _wrap_tmpl = co_wrap_t<TmplT,
        typename value_t<_Tp::value>::value_type, _Tp::value>;

    using type = typename meta_list_push_back<OutT, _wrap_tmpl<Ts>...>::type;
};

template <auto... Vs, MetaListType OutT, VaTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<value_list<Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, va_wrap_t<TmplT, Vs>...>::type;
};
template <auto... Vs, MetaListType OutT, CoTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<value_list<Vs...>, OutT, TmplT> {
    template <auto __v>
    using _wrap_tmpl = co_wrap_t<TmplT,
        typename value_t<__v>::value_type, __v>;

    using type = typename meta_list_push_back<OutT, _wrap_tmpl<Vs>...>::type;
};
template <auto... Vs, MetaListType OutT, TyTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<value_list<Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, ty_wrap_t<TmplT, value_t<Vs>>...>::type;
};

template <typename VT, VT... Vs, MetaListType OutT, CoTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<constant_list<VT, Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, co_wrap_t<TmplT, VT, Vs>...>::type;
};
template <typename VT, VT... Vs, MetaListType OutT, VaTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<constant_list<VT, Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, va_wrap_t<TmplT, Vs>...>::type;
};
template <typename VT, VT... Vs, MetaListType OutT, TyTmplType TmplT>
requires CtCmp<CmpOp::kGT, sizeof...(Vs), 0>
struct meta_list_map<constant_list<VT, Vs...>, OutT, TmplT> {
    using type = typename meta_list_push_back<
        OutT, ty_wrap_t<TmplT, constant_t<VT, Vs>>...>::type;
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

template <typename T, T... Vs1, T... Vs2>
auto _concat_two_constant_list(constant_list<T, Vs1...>, constant_list<T, Vs2...>)
    -> constant_list<T, Vs1..., Vs2...>;

template <CoListType T1, CoListType T2, CoListType... Ts>
struct constant_list_cat<T1, T2, Ts...> {
    using type = typename constant_list_cat<
        decltype(_concat_two_constant_list(std::declval<T1>(), std::declval<T2>())),
        Ts...
    >::type;
};

template <CoListType T, std::size_t pos>
struct constant_list_at {
    using _tylist_type = T::template wrapped<ty_list_tt>;
    using type = _tylist_type::template at<pos>;
};

template <CoListType InT, TyListType MaskT, CoListType OutT, std::size_t pos>
requires CtCmp<CmpOp::kLT, pos, InT::size()> && CtCmp<CmpOp::kLT, pos, MaskT::size()>
struct constant_list_filter<InT, MaskT, OutT, pos> {
    using _elem_type  = InT::template at<pos>;
    using _filt_type  = MaskT::template at<pos>;
    using _t_out_type = OutT::template push_back<_elem_type::value>;
    using _f_out_type = OutT;
    using _out_type   = cond_t<_filt_type::value, _t_out_type, _f_out_type>;

    using type = typename constant_list_filter<InT, MaskT, _out_type, pos + 1>::type;
};

template <TyListType T, IntSeqType RangeT>
struct type_list_slice {
    using _tmpl_type = va_tmpl_t<T::template at>;
    using type = RangeT::template ty_map<_tmpl_type>;
};

template <TyListType T, TyListType MaskT>
struct type_list_filter {
    using _range_type = make_index_seq<T::size()>::template filter<MaskT>;
    using type = T::template slice<_range_type>;
};

template <CoListType T>
struct constant_list_sorted_unique {
    using _cmp_2v_ttype = ct_pos_value_cmp_tt<va_tmpl_t<T::template at>, CmpOp::kNE>;
    using _cmp_1f_ttype = ct_pos_forward_cmp_tt<_cmp_2v_ttype, T::size()>;
    using _filt_type = make_index_seq<T::size()>::template ty_map<_cmp_1f_ttype>;
    using type = T::template filter<_filt_type>;
};
}
}

