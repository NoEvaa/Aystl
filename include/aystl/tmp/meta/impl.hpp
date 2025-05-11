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

#include <tuple>

#include "aystl/tmp/meta/base.hpp"
#include "aystl/tmp/meta/type_list.hpp"
#include "aystl/tmp/meta/value_list.hpp"
#include "aystl/tmp/meta/constant_list.hpp"
#include "aystl/tmp/meta/int_seq.hpp"
#include "aystl/tmp/functional/comparator.hpp"
#include "aystl/tmp/functional/ct_array.hpp"
#include "aystl/tmp/functional/ct_std_algo.hpp"

namespace iin {
namespace _tmp_impl {
template <TyTmplType T, typename... Ts>
struct ty_wrap<T, Ts...> {
    using type = T::template wrap<Ts...>;
};
template <VaTmplType T, typename... Ts>
requires is_all_of_v<bool_constant<ValueTType<Ts>>...>
struct ty_wrap<T, Ts...> {
    using type = T::template wrap<Ts::value...>;
};
/*template <CoTmplType T, typename VT, typename... Ts>
requires is_all_of_v<bool_constant<ValueTType<Ts>>...>
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

template <MetaTmplType TmplT, MetaListType T>
struct meta_wrap {
    using type = TmplT::template wrap<T>;
};
template <MetaPrimTmplType TmplT, typename... Ts>
struct meta_wrap<TmplT, type_list<Ts...>> {
    using type = typename ty_wrap<TmplT, Ts...>::type;
};
template <MetaPrimTmplType TmplT, auto... Vs>
struct meta_wrap<TmplT, value_list<Vs...>> {
    using type = typename va_wrap<TmplT, Vs...>::type;
};
template <MetaPrimTmplType TmplT, typename VT, VT... Vs>
struct meta_wrap<TmplT, constant_list<VT, Vs...>> {
    using type = typename co_wrap<TmplT, VT, Vs...>::type;
};

template <typename T, TyTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_tmpl_wrapper_t<T, TmplT::template wrap>;
};
template <typename T, VaTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_va_tmpl_wrapper_t<T, TmplT::template wrap>;
};
template <typename T, CoTmplType TmplT>
struct meta_rewrapped<T, TmplT> {
    using type = replace_co_tmpl_wrapper_t<T, TmplT::template wrap>;
};

template <typename T, typename... Ts>
struct meta_rewrap<T, type_list<Ts...>> {
    using type = replace_tmpl_args_t<T, Ts...>;
};
template <typename T, auto... Vs>
struct meta_rewrap<T, value_list<Vs...>> {
    using type = replace_va_tmpl_args_t<T, Vs...>;
};
template <typename T, typename VT, VT... Vs>
struct meta_rewrap<T, constant_list<VT, Vs...>> {
    using type = replace_co_tmpl_args_t<T, VT, Vs...>;
};
}

namespace _tmp_impl {
template <TyListType T, typename... _Ts>
struct meta_list_push_back<T, _Ts...> {
    using type = T::template push_back<_Ts...>;
};
template <VaListType T, ValueTType... _Ts>
struct meta_list_push_back<T, _Ts...> {
    using type = T::template push_back<_Ts::value...>;
};
template <CoListType T, ValueTType... _Ts>
struct meta_list_push_back<T, _Ts...> {
    using value_type = typename T::value_type;
    using type = T::template push_back<static_cast<value_type>(_Ts::value)...>;
};

template <MetaListType T, std::size_t pos>
struct meta_list_at {
    using _list_type = T::template wrapped<ty_list_tt>;
    using type = _list_type::template at<pos>;
};
template <TyListType T, std::size_t pos>
struct meta_list_at<T, pos> {
    using _tuple_type = T::template wrapped<ty_tmpl_t<std::tuple>>;
    using type = std::tuple_element_t<pos, _tuple_type>;
};

template <typename T, typename DefaultT>
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

template <CoListType T, TyListType MaskT>
struct meta_list_mask_filter<T, MaskT> {
    template <CoListType OutT, std::size_t pos>
    struct __impl : type_t<OutT> {};
    template <CoListType OutT, std::size_t pos>
    requires CtCmp<CmpOp::kLT, pos, T::size()> && CtCmp<CmpOp::kLT, pos, MaskT::size()>
    struct __impl<OutT, pos> {
        using _elem_type  = T::template at<pos>;
        using _mask_type  = MaskT::template at<pos>;
        using _t_out_type = OutT::template push_back<_elem_type::value>;
        using _f_out_type = OutT;
        using _out_type   = cond_t<_mask_type::value, _t_out_type, _f_out_type>;

        using type = typename __impl<_out_type, pos + 1>::type;
    };

    using type = typename __impl<constant_list<typename T::value_type>, 0>::type;
};
template <TyListType T, TyListType MaskT>
struct meta_list_mask_filter<T, MaskT> {
    using _idxes_type = make_index_seq_t<T::size()>::template mask_filter<MaskT>;
    using type = T::template slice<_idxes_type>;
};

template <CoListType T>
struct meta_list_reverse<T> {
    using type = T::template apply_algo<detail::ct_std_reverse_t>;
};

template <TyListType T>
struct meta_list_reverse<T> {
    using _idxes_type = meta_list_reverse_t<make_index_seq_t<T::size()>>;
    using type = _idxes_type::template ty_map<va_tmpl_t<T::template at>>;
};

template <VaListType T>
struct meta_list_reverse<T> {
    using _idxes_type = meta_list_reverse_t<make_index_seq_t<T::size()>>;
    using type = _idxes_type::template va_map<va_tmpl_t<T::template at>>;
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
}

namespace _tmp_impl {
template <TyListType T1, TyListType T2, TyListType... Ts>
struct type_list_cat<T1, T2, Ts...> {
    using _first_type = T2::template wrapped<ty_tmpl_t<T1::template push_back>>;
    using type = typename type_list_cat<_first_type, Ts...>::type;
};

template <CoListType T1, CoListType T2, CoListType... Ts>
struct constant_list_cat<T1, T2, Ts...> {
    using _first_type = T2::template wrapped<va_tmpl_t<T1::template push_back>>;
    using type = typename constant_list_cat<_first_type, Ts...>::type;
};

template <TyListType T, IntSeqType RangeT>
struct type_list_slice {
    template <std::size_t pos>
    using _check_pos = ct_cmp<CmpOp::kLT, pos, T::size()>;

    using type = RangeT
        ::template filter<va_tmpl_t<_check_pos>>
        ::template ty_map<va_tmpl_t<T::template at>>;
};

template <CoListType T, typename AlgoT>
struct constant_list_apply_algo {
    using type = typename T::template wrapped<ct_array_tt<AlgoT>>::to_constant_list;
};

template <CoListType T, typename CmpT>
struct constant_list_sort {
    using type = T::template apply_algo<detail::ct_std_sort_t<CmpT>>;
};

template <CoListType T>
struct constant_list_sorted_unique {
    using _cmp_2v_ttype = ct_pos_value_cmp_tt<va_tmpl_t<T::template at>, CmpOp::kNE>;
    using _cmp_1f_ttype = ct_pos_forward_cmp_tt<_cmp_2v_ttype, T::size()>;
    using _mask_type    = make_index_seq_t<T::size()>::template ty_map<_cmp_1f_ttype>;
    using type = T::template mask_filter<_mask_type>;
};
}
}

