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

#include "aystl/core/memory/allocator.hpp"
#include "aystl/core/type_traits/utils.hpp"
#include "aystl/core/type_traits/is_specialization_of.hpp"
#include "aystl/core/math/compare.hpp"

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

template <_Action, typename...> struct _AyAnyHandler;
}

template <class AllocT = AyAlloc<void>,
    ValueTType MemSizeVT = constant_t<std::size_t, 32U>>
class AyBasicAny {
    using _self_type = AyBasicAny<AllocT, MemSizeVT>;
    using _Action    = _any_impl::_Action;
    using _act_type  = void *(*)(_Action, _self_type const *, _self_type *);

    template <typename _ATp, typename _MemSz>
    using _self_tmpl = AyBasicAny<_ATp, _MemSz>;
    template <_Action _action, typename... _Tps>
    using _hnd_tmpl = _any_impl::_AyAnyHandler<_action, _Tps...>;

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

    AyBasicAny(_self_type const & _ot) noexcept { _ot._copyTo(this); }
    AyBasicAny(_self_type && _ot) noexcept { _ot._moveTo(this); }

    template <typename _ValT, typename _Tp = std::decay_t<_ValT>>
    requires (!is_any_spec_of_v<_Tp, _self_tmpl, std::in_place_type_t>)
    AyBasicAny(_ValT && _v) noexcept { this->setValue<_Tp>(std::forward<_ValT>(_v)); }

    template <typename _ValT, typename... _Args>
    explicit AyBasicAny(std::in_place_type_t<_ValT>, _Args &&... _args) noexcept {
        this->setValue<_ValT>(std::forward<_Args>(_args)...);
    }

    ~AyBasicAny() noexcept { this->reset(); }

    _self_type & operator=(_self_type const & rhs) {
        if (this != &rhs) [[likely]] {
            this->reset();
            rhs._copyTo(this);
        }
        return *this;
    }

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

    std::type_info const & getTypeInfo() const noexcept {
        if (!this->hasValue()) { return typeid(void); }
        return *static_cast<std::type_info const *>(this->__callAct(_Action::kTypeInfo));
    }
    bool isType(std::type_info const & _ti) const noexcept {
        return this->getTypeInfo() == _ti;
    }
    template <typename _Tp>
    bool isType() const noexcept { return this->isType(typeid(_Tp)); }

    template <typename _Tp>
    _Tp fastGetValue() const noexcept { return this->__toValue<_Tp>(); }

    template <typename _Tp>
    _Tp * getValuePtr() const noexcept {
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

    void swap(_self_type &) noexcept;
    
private:
    _self_type & _getSelf() const noexcept { return const_cast<_self_type &>(*this); }

    template <typename _Tp> void _initAct() noexcept;
    void * _callAct(_Action, _self_type * _other = nullptr) const;
    void * __callAct(_Action, _self_type * _other = nullptr) const;

    template <typename _Tp> _Tp * __toPtr() const noexcept;
    template <typename _Tp> _Tp __toValue() const noexcept;

    void _copyTo(_self_type *) const;
    void __copyTo(_self_type *) const;
    void _moveTo(_self_type *) noexcept;
    void __moveTo(_self_type *) noexcept;

private:
    _act_type m_act = nullptr;
    void *    m_buf[kBufSize] = {};

    template <_Action, typename ...>
    friend struct _any_impl::_AyAnyHandler;
};
using AyAny = AyBasicAny<>;

namespace _any_impl {
template <typename T>
inline constexpr bool _is_inp_value_v = value_t<(sizeof(T) <= AyAny::kBufByteSize)>::value;

template <typename T>
struct _AyAnyHandler<_Action::kTypeInfo, T> {
    static void * call() noexcept {
        return const_cast<void *>(static_cast<void const *>(&typeid(T)));
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kGet, T, AnyT> {
    static void * call(AnyT & _self) noexcept { 
        if constexpr (_is_inp_value_v<T>) {
            return static_cast<void *>(&_self.m_buf);
        } else {
            return static_cast<void *>(_self.m_buf[0]);
        }
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kCreate, T, AnyT> {
    using alloc_type = replace_tmpl_args_t<typename AnyT::alloc_type, T>;
    using atraits_type = AyAllocTraits<alloc_type>;

    template <typename... _Args>
    static T & call(AnyT & _self, _Args &&... _args) {
        T * _p = nullptr;
        if constexpr (_is_inp_value_v<T>) {
            _p = static_cast<T *>(static_cast<void *>(&_self.m_buf));
            std::construct_at(_p, std::forward<_Args>(_args)...);
        } else {
            alloc_type _a;
            _p = atraits_type::allocate(_a, 1);
            atraits_type::construct(_a, _p, std::forward<_Args>(_args)...);
            _self.m_buf[0] = _p;
        }
        return *_p;
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kDestroy, T, AnyT> {
    using alloc_type = replace_tmpl_args_t<typename AnyT::alloc_type, T>;
    using atraits_type = AyAllocTraits<alloc_type>;

    static void * call(AnyT const & _self) noexcept {
        if constexpr (_is_inp_value_v<T>) {
            if constexpr (!std::is_trivially_destructible<T>::value) {
                std::destroy_at(_self.template __toPtr<T>());
            }
        } else {
            alloc_type _a;
            auto * _p = _self.template __toPtr<T>();
            atraits_type::destroy(_a, _p);
            atraits_type::deallocate(_a, _p, 1);
        }
        return nullptr;
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kCopyTo, T, AnyT> {
    static void * call(AnyT const & _src, AnyT & _dst) {
        using _lref_type = std::add_lvalue_reference_t<T>;
        _AyAnyHandler<_Action::kCreate, T, AnyT>::call(
            _dst, _src.template __toValue<_lref_type>());
        return nullptr;
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kMoveTo, T, AnyT> {
    static void * call(AnyT & _src, AnyT & _dst) noexcept {
        if constexpr (_is_inp_value_v<T>) {
            std::memcpy(&_dst.m_buf, &_src.m_buf, sizeof(T));
        } else {
            std::memcpy(&_dst.m_buf, &_src.m_buf, AnyT::kPtrSize);
        }
        return nullptr;
    }
};

template <typename T, typename AnyT>
struct _AyAnyHandler<_Action::kEqualTo, T, AnyT> {
    static void * call(AnyT const & lhs, AnyT const & rhs) {
        if (AyCmp<CmpOp::kEQ, T>{}(
            lhs.template __toValue<T const &>(),
            rhs.template __toValue<T const &>())) {
            return static_cast<void *>((bool *)1);
        }
        return nullptr;
    }
};
}

template <typename ATp, ValueTType MemSz>
bool AyBasicAny<ATp, MemSz>::operator==(_self_type const & rhs) const noexcept {
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

template <typename ATp, ValueTType MemSz>
template <typename _Tp, typename... _Args>
_Tp & AyBasicAny<ATp, MemSz>::setValue(_Args &&... _args) {
    using _type = std::decay_t<_Tp>;
    this->reset();
    this->_initAct<_type>();
    return _hnd_tmpl<_Action::kCreate, _type, _self_type>::call(
        *this, std::forward<_Args>(_args)...);
}

template <typename ATp, ValueTType MemSz>
void AyBasicAny<ATp, MemSz>::swap(_self_type & _ot) noexcept
{
    if (this == &_ot) [[unlikely]] { return; }
    if (this->hasValue() && _ot.hasValue()) {
        _self_type temp;
        this->__moveTo(&temp);
        _ot.__moveTo(this);
        temp.__moveTo(&_ot);
    } else if (this->hasValue()) {
        this->__moveTo(&_ot);
    } else if (_ot.hasValue()) {
        _ot.__moveTo(this);
    }
}

template <typename ATp, ValueTType MemSz>
template <typename _Tp>
void AyBasicAny<ATp, MemSz>::_initAct() noexcept {
    m_act = [](_Action _a, _self_type const * _this, _self_type * _ot) -> void * {
        switch (_a) {
        case _Action::kTypeInfo:
            return _hnd_tmpl<_Action::kTypeInfo, _Tp>::call();
        case _Action::kGet:
            return _hnd_tmpl<_Action::kGet, _Tp, _self_type>::call(_this->_getSelf());
        case _Action::kDestroy:
            return _hnd_tmpl<_Action::kDestroy, _Tp, _self_type>::call(*_this);
        case _Action::kCopyTo:
            return _hnd_tmpl<_Action::kCopyTo, _Tp, _self_type>::call(*_this, *_ot);
        case _Action::kMoveTo:
            return _hnd_tmpl<_Action::kMoveTo, _Tp, _self_type>::call(_this->_getSelf(), *_ot);
        case _Action::kEqualTo:
            return _hnd_tmpl<_Action::kEqualTo, _Tp, _self_type>::call(*_this, *_ot);
        default:
            return nullptr;
        }
    };
}

template <typename ATp, ValueTType MemSz>
void * AyBasicAny<ATp, MemSz>::_callAct(_Action _action, _self_type * _other) const {
    if (this->hasValue()) {
        return this->__callAct(_action, _other);
    }
    return nullptr;
}

template <typename ATp, ValueTType MemSz>
void * AyBasicAny<ATp, MemSz>::__callAct(_Action _action, _self_type * _other) const {
    return this->m_act(_action, this, _other);
}

template <typename ATp, ValueTType MemSz>
template <typename _Tp>
_Tp * AyBasicAny<ATp, MemSz>::__toPtr() const noexcept {
    return static_cast<_Tp *>(this->__callAct(_Action::kGet));
}

template <typename ATp, ValueTType MemSz>
template <typename _Tp>
_Tp AyBasicAny<ATp, MemSz>::__toValue() const noexcept {
    return static_cast<_Tp>(*__toPtr<std::remove_reference_t<_Tp>>());
}

template <typename ATp, ValueTType MemSz>
void AyBasicAny<ATp, MemSz>::_copyTo(_self_type * _dst) const {
    if (this->hasValue()) {
        this->__copyTo(_dst);
    } else {
        _dst->reset();
    }
}

template <typename ATp, ValueTType MemSz>
void AyBasicAny<ATp, MemSz>::__copyTo(_self_type * _dst) const {
    this->__callAct(_Action::kCopyTo, _dst);
    _dst->m_act = this->m_act;
}

template <typename ATp, ValueTType MemSz>
void AyBasicAny<ATp, MemSz>::_moveTo(_self_type * _dst) noexcept {
    if (this->hasValue()) {
        this->__moveTo(_dst);
    } else {
        _dst->reset();
    }
}

template <typename ATp, ValueTType MemSz>
void AyBasicAny<ATp, MemSz>::__moveTo(_self_type * _dst) noexcept {
    this->__callAct(_Action::kMoveTo, _dst);
    _dst->m_act = std::exchange(m_act, nullptr);
}
}

