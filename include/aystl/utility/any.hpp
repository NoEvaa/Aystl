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

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include "aystl/global/common.hpp"
#include "aystl/tmp.hpp"
#include "aystl/core/arch/allocator.hpp"
#include "aystl/core/arch/compare.hpp"
#include "aystl/core/arch/copy.hpp"

#define _AY_ANY_METHOD_TEMPLATE() template <typename ATp, ConstantTType<std::size_t> MemSz>
#define _AY_ANY_METHOD_NAME(func_name) AyBasicAny<ATp, MemSz>::func_name
#define _AY_ANY_DECL_METHOD(ret_tp, func_name)                                                     \
    _AY_ANY_METHOD_TEMPLATE()                                                                      \
    ret_tp _AY_ANY_METHOD_NAME(func_name)

namespace iin {
namespace _any_impl {
enum class _Action {
    kTypeInfo = 0,
    kGet,

    kCreate,
    kDestroy,

    kCopyTo,
    kMoveTo,
    
    kEqualTo,
};

template <_Action, typename...> struct AyAnyHandler;
}

template <class AllocT = AyAlloc<void>,
    ConstantTType<std::size_t> MemSizeVT = constant_t<std::size_t, 32U>>
class AyBasicAny {
    using _self_type = AyBasicAny<AllocT, MemSizeVT>;
    using _Action    = _any_impl::_Action;
    using _act_type  = void *(*)(_Action, _self_type const *, _self_type *);

    template <typename _ATp, typename _MemSz>
    using _self_tmpl = AyBasicAny<_ATp, _MemSz>;
    template <_Action _action, typename... _Tps>
    using _hnd_tmpl = _any_impl::AyAnyHandler<_action, _Tps...>;

    template <_Action, typename ...>
    friend struct _any_impl::AyAnyHandler;

public:
    using size_type  = std::size_t;
    using alloc_type = AllocT;

    static constexpr size_type kPtrSize     = sizeof(void *);
    static constexpr size_type kMemSize     = MemSizeVT::value;
    static constexpr size_type kBufByteSize = kMemSize - kPtrSize;
    static constexpr size_type kBufSize     = kBufByteSize / kPtrSize;
    static_assert(sizeof(_act_type) == kPtrSize);
    static_assert(kMemSize > kPtrSize);
    static_assert(kBufByteSize >= kPtrSize);
    static_assert((kBufByteSize % kPtrSize) == 0);

    AyBasicAny() noexcept = default;
    ~AyBasicAny() noexcept { this->reset(); }

    template <typename _ValT, typename _Tp = std::decay_t<_ValT>>
    requires (!is_any_spec_of_v<_Tp, _self_tmpl, std::in_place_type_t>)
    AyBasicAny(_ValT && _v) noexcept { this->setValue<_Tp>(AY_FORWARD(_v)); }

    template <typename _ValT, typename... _Args>
    requires std::is_same_v<_ValT, std::decay_t<_ValT>>
    explicit AyBasicAny(std::in_place_type_t<_ValT>, _Args &&... _args) noexcept {
        this->setValue<_ValT>(AY_FORWARD(_args)...);
    }

    AyBasicAny(_self_type const & _ot) noexcept { _ot._copyTo(this); }
    _self_type & operator=(_self_type const & rhs) noexcept {
        if (this != &rhs) [[likely]] {
            this->reset();
            rhs._copyTo(this);
        }
        return *this;
    }

    AyBasicAny(_self_type && _ot) noexcept { _ot._moveTo(this); }
    _self_type & operator=(_self_type && rhs) noexcept {
        if (this != &rhs) [[likely]] {
            this->reset();
            rhs._moveTo(this);
        }
        return *this;
    }

    bool operator==(_self_type const &) const noexcept;

    void reset() noexcept {
        if (!this->hasValue()) { return; }
        this->__callAct(_Action::kDestroy);
        m_act = nullptr;
    }

    bool hasValue() const noexcept { return m_act != nullptr; }
    operator bool() const noexcept { return this->hasValue(); }
    
    bool isType(std::type_info const & _ti) const noexcept {
        return this->getTypeInfo() == _ti;
    }
    template <typename _Tp>
    bool isType() const noexcept { return this->isType(typeid(_Tp)); }
    std::type_info const & getTypeInfo() const noexcept;

    template <typename _Tp>
    _Tp fastGetValue() const noexcept { return this->__toValue<_Tp>(); }

    template <typename _Tp>
    auto getValuePtr() const noexcept -> std::add_pointer_t<_Tp> {
        if (!this->hasValue()) { return nullptr; }
        if (!this->isType<std::decay_t<_Tp>>()) { return nullptr; }
        return this->__toPtr<_Tp>();
    }

    template <typename _Tp>
    _Tp getValue() const {
        auto * _p = this->getValuePtr<std::remove_reference_t<_Tp>>();
        if (!_p) {
            throw std::runtime_error{"`iin::AyBasicAny` cannot access the value."};
        }
        return static_cast<_Tp>(*_p);
    }

    template <typename _Tp, typename... _Args>
    _Tp & setValue(_Args &&...);

    void swap(_self_type &&) noexcept;
    
private:
    _self_type & _getSelf() const noexcept { return const_cast<_self_type &>(*this); }

    template <typename _Tp> void _initAct() noexcept;
    void * _callAct(_Action, _self_type * _other = nullptr) const;
    void * __callAct(_Action, _self_type * _other = nullptr) const;

    template <typename _Tp> std::add_pointer_t<_Tp> __toPtr() const noexcept;
    template <typename _Tp> _Tp __toValue() const noexcept {
        return static_cast<_Tp>(*__toPtr<_Tp>());
    }

    void _copyTo(_self_type *) const;
    void _moveTo(_self_type *) noexcept;

private:
    _act_type m_act = nullptr;
    void *    m_buf[kBufSize] = {};
};
using AyAny = AyBasicAny<>;

namespace _any_impl {
template <typename T, typename AnyT>
inline constexpr bool _is_inp_value_v = value_t<(sizeof(T) <= AnyT::kBufByteSize)>::value;

template <typename T>
struct AyAnyHandler<_Action::kTypeInfo, T> {
    static void * call() noexcept {
        return const_cast<void *>(static_cast<void const *>(&typeid(T)));
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kGet, T, AnyT> {
    static void * call(AnyT & _self) noexcept { 
        if constexpr (_is_inp_value_v<T, AnyT>) {
            return static_cast<void *>(&_self.m_buf);
        } else {
            return static_cast<void *>(_self.m_buf[0]);
        }
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kCreate, T, AnyT> {
    using alloc_type   = replace_tmpl_args_t<typename AnyT::alloc_type, T>;
    using atraits_type = AyAllocTraits<alloc_type>;

    template <typename... _Args>
    static T & call(AnyT & _self, _Args &&... _args) {
        T * _p = nullptr;
        if constexpr (_is_inp_value_v<T, AnyT>) {
            _p = static_cast<T *>(static_cast<void *>(&_self.m_buf));
            std::construct_at(_p, AY_FORWARD(_args)...);
        } else {
            alloc_type _a;
            _p = atraits_type::allocate(_a, 1);
            atraits_type::construct(_a, _p, AY_FORWARD(_args)...);
            _self.m_buf[0] = _p;
        }
        return *_p;
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kDestroy, T, AnyT> {
    using alloc_type   = replace_tmpl_args_t<typename AnyT::alloc_type, T>;
    using atraits_type = AyAllocTraits<alloc_type>;

    static void call(AnyT & _self) noexcept {
        if constexpr (_is_inp_value_v<T, AnyT>) {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy_at(_self.template __toPtr<T>());
            }
        } else {
            alloc_type _a;
            auto * _p = _self.template __toPtr<T>();
            atraits_type::destroy(_a, _p);
            atraits_type::deallocate(_a, _p, 1);
        }
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kCopyTo, T, AnyT> {
    using create_type = AyAnyHandler<_Action::kCreate, T, AnyT>;
    using clref_type  = add_clref_t<T>;

    static void call(AnyT const & _src, AnyT & _dst) {
        create_type::call(_dst, _src.template __toValue<clref_type>());
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kMoveTo, T, AnyT> {
    using memcpy_type  = AyCpy<CpyOp::kMemory, T>;
    using create_type  = AyAnyHandler<_Action::kCreate, T, AnyT>;
    using destroy_type = AyAnyHandler<_Action::kDestroy, T, AnyT>;
    using clref_type   = add_clref_t<T>;
    using rref_type    = std::add_rvalue_reference_t<T>;

    static void call(AnyT & _src, AnyT & _dst) noexcept {
        if constexpr (_is_inp_value_v<T, AnyT>) {
            if constexpr (memcpy_type::value) {
                // memory copy
                memcpy_type{}(
                    _dst.template __toValue<T &>(), 
                    _src.template __toValue<T const &>()
                );
            } else if constexpr (std::is_move_constructible_v<T>) {
                // move construct
                create_type::call(_dst, _src.template __toValue<rref_type>());
            } else {
                // copy construct & destroy
                create_type::call(_dst, _src.template __toValue<clref_type>());
                destroy_type::call(_src);
            }
        } else {
            // only memory copy the pointer
            std::memcpy(&_dst.m_buf, &_src.m_buf, AnyT::kPtrSize);
        }
    }
};

template <typename T, typename AnyT>
struct AyAnyHandler<_Action::kEqualTo, T, AnyT> {
    static void * call(AnyT const & lhs, AnyT const & rhs) noexcept {
        if (AyCmp<CmpOp::kEQ, T>{}(
            lhs.template __toValue<T const &>(),
            rhs.template __toValue<T const &>())) {
            return static_cast<void *>((bool *)1);
        }
        return nullptr;
    }
};
}

_AY_ANY_DECL_METHOD(bool, operator==)(_self_type const & rhs) const noexcept {
    if (this == &rhs) [[unlikely]] { return true; }
    if (!this->hasValue()) {
        return !rhs.hasValue();
    } else if (!rhs.hasValue()) {
        return false;
    }
    if (!this->isType(rhs.getTypeInfo())) {
        return false;
    }
    return bool(this->__callAct(_Action::kEqualTo, &rhs._getSelf()));
}

_AY_ANY_DECL_METHOD(std::type_info const &, getTypeInfo)() const noexcept {
    if (!this->hasValue()) { return typeid(void); }
    return *static_cast<std::type_info const *>(this->__callAct(_Action::kTypeInfo));
}

_AY_ANY_METHOD_TEMPLATE()
template <typename _Tp, typename... _Args>
_Tp & _AY_ANY_METHOD_NAME(setValue)(_Args &&... _args) {
    using _type = std::decay_t<_Tp>;
    using _create_type = _hnd_tmpl<_Action::kCreate, _type, _self_type>;
    this->reset();
    this->_initAct<_type>();
    return _create_type::call(*this, AY_FORWARD(_args)...);
}

_AY_ANY_DECL_METHOD(void, swap)(_self_type && _ot) noexcept {
    if (this == &_ot) [[unlikely]] { return; }
    _ot = std::exchange(*this, std::move(_ot));
}

_AY_ANY_METHOD_TEMPLATE()
template <typename _Tp>
void _AY_ANY_METHOD_NAME(_initAct)() noexcept {
    m_act = [](_Action _a, _self_type const * _this, _self_type * _ot) -> void * {
        switch (_a) {
        case _Action::kTypeInfo:
            return _hnd_tmpl<_Action::kTypeInfo, _Tp>::call();
        case _Action::kGet:
            return _hnd_tmpl<_Action::kGet, _Tp, _self_type>::call(_this->_getSelf());
        case _Action::kDestroy:
            _hnd_tmpl<_Action::kDestroy, _Tp, _self_type>::call(_this->_getSelf());
            break;
        case _Action::kCopyTo:
            _hnd_tmpl<_Action::kCopyTo, _Tp, _self_type>::call(*_this, *_ot);
            break;
        case _Action::kMoveTo:
            _hnd_tmpl<_Action::kMoveTo, _Tp, _self_type>::call(_this->_getSelf(), *_ot);
            break;
        case _Action::kEqualTo:
            return _hnd_tmpl<_Action::kEqualTo, _Tp, _self_type>::call(*_this, *_ot);
        default:
            AY_UNREACHABLE();
            break;
        }
        return nullptr;
    };
}

_AY_ANY_DECL_METHOD(void *, _callAct)(_Action _action, _self_type * _other) const {
    if (this->hasValue()) {
        return this->__callAct(_action, _other);
    }
    return nullptr;
}

_AY_ANY_DECL_METHOD(void *, __callAct)(_Action _action, _self_type * _other) const {
    return this->m_act(_action, this, _other);
}

_AY_ANY_METHOD_TEMPLATE()
template <typename _Tp>
std::add_pointer_t<_Tp> _AY_ANY_METHOD_NAME(__toPtr)() const noexcept {
    return static_cast<std::add_pointer_t<_Tp>>(this->__callAct(_Action::kGet));
}

_AY_ANY_DECL_METHOD(void, _copyTo)(_self_type * _dst) const {
    AY_ASSERT(!_dst->hasValue());
    if (this->hasValue()) {
        _dst->m_act = this->m_act;
        this->__callAct(_Action::kCopyTo, _dst);
    }
}

_AY_ANY_DECL_METHOD(void, _moveTo)(_self_type * _dst) noexcept {
    AY_ASSERT(!_dst->hasValue());
    if (this->hasValue()) {
        _dst->m_act = this->m_act;
        this->__callAct(_Action::kMoveTo, _dst);
        this->m_act = nullptr;
    }
}
}

#undef _AY_ANY_DECL_METHOD
#undef _AY_ANY_METHOD_TEMPLATE
#undef _AY_ANY_METHOD_NAME

