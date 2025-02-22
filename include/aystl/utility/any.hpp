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
#include <cstdint>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include "aystl/global/common.h"

namespace iin {
namespace _any_impl {
enum class _Action {
    kTypeInfo = 0,
    kDestroy,
    kCopyTo,
    kMoveTo,
};
template <class, _Action> struct _AyAnyHandler;
}

class AyAny {
    using _Action   = _any_impl::_Action;
    using _act_type = void *(*)(_Action, AyAny const *, AyAny *);
public:
    using size_type = std::size_t;

    static constexpr size_type kPtrSize     = sizeof(void *);
    static constexpr size_type kMaxPtrSize  = sizeof(std::uint64_t);
    static constexpr size_type kBufByteSize = kMaxPtrSize * 3;
    static constexpr size_type kBufSize     = kBufByteSize / kPtrSize;
    static_assert(kPtrSize <= kMaxPtrSize);
    static_assert((kBufByteSize % kPtrSize) == 0);

    AyAny() = default;
    AyAny(AyAny const &) noexcept;
    AyAny(AyAny &&) noexcept;

    ~AyAny() noexcept { this->reset(); }

    AyAny & operator=(AyAny const &) noexcept;
    AyAny & operator=(AyAny &&) noexcept;

    void reset() noexcept;

    void swap(AyAny &) noexcept;

    bool hasValue() const noexcept { return m_act != nullptr; }
    operator bool() const noexcept { return this->hasValue(); }

    std::type_info const & getType() const noexcept {
        if (!this->hasValue()) { return typeid(void); }
        return *static_cast<std::type_info const *>(this->_callAct(_Action::kTypeInfo));
    }
    bool isType(std::type_info const & _tinfo) const noexcept { return this->getType() == _tinfo; }
    template <typename _Tp>
    bool isType() const noexcept { return this->isType(typeid(_Tp)); }

    template <typename _Tp>
    _Tp fastGetValue() const noexcept { return static_cast<_Tp>(*_toPtr<_Tp>()); }

    template <typename _Tp>
    _Tp * getValuePtr() const noexcept {
        if (!this->hasValue()) { return nullptr; }
        if (!isType<std::decay_t<_Tp>>()) { return nullptr; }
        return _toPtr<_Tp>();
    }

    template <typename _Tp>
    _Tp getValue() const {
        auto * __p = getValuePtr<_Tp>();
        if (!__p) {
            throw std::runtime_error{"iin::AyAny can not access to value."};
        }
        return static_cast<_Tp>(*__p);
    }

private:
    template <typename _Tp>
    void * _toVoidPtr() const noexcept {
        if constexpr (sizeof(_Tp) <= kBufByteSize) {
            return detail::_castToVoidPtr(&m_buf);
        } else {
            return detail::_castToVoidPtr(m_buf[0]);
        }
    }

    template <typename _Tp>
    _Tp * _toPtr() const noexcept { return static_cast<_Tp *>(_toVoidPtr<_Tp>()); }

    template <typename _Tp>
    void _initAct() noexcept {
        m_act = [](_Action _action, AyAny const * _this, AyAny * _ot) -> void * {
            using _any_impl::_AyAnyHandler;
            switch (_action) {
            case _Action::kTypeInfo:
                return _AyAnyHandler<_Tp, _Action::kTypeInfo>::call();
            case _Action::kDestroy:
                return _AyAnyHandler<_Tp, _Action::kDestroy>::call(*_this);
            case _Action::kCopyTo:
                return _AyAnyHandler<_Tp, _Action::kCopyTo>::call(*_this, *_ot);
            case _Action::kMoveTo:
                return _AyAnyHandler<_Tp, _Action::kMoveTo>::call(const_cast<AyAny &>(*_this), *_ot);
            default:
                return nullptr;
            }
        };
    }

    void * _callAct(_Action _action, AyAny * _other = nullptr) const noexcept {
        return m_act(_action, this, _other);
    }

private:
    void *    m_buf[kBufSize] = {};
    _act_type m_act = nullptr;

    template <class, _Action>
    friend struct _any_impl::_AyAnyHandler;
};

namespace _any_impl {
template <class T>
struct _AyAnyHandler<T, _Action::kTypeInfo> {
    static void * call() noexcept { return detail::_castToVoidPtr(&typeid(T)); }
};

template <class T>
struct _AyAnyHandler<T, _Action::kDestroy> {
    static void * call(AyAny const & _self) noexcept {
        if constexpr (sizeof(T) <= AyAny::kBufSize) {
            if constexpr (!std::is_trivially_destructible<T>::value) {
                auto * p_v = _self._toPtr<T>();
                p_v->~ValT();
                //std::launder(p_v)->~ValT();
            }
        } else if constexpr (std::is_array_v<T>) {
            auto * p_v = _self._toPtr<T>();
            delete[] p_v;
        } else {
            auto * p_v = _self._toPtr<T>();
            delete p_v;
        }
        return nullptr;
    }
};

template <class T>
struct _AyAnyHandler<T, _Action::kCopyTo> {
    static void * call(AyAny const & _src, AyAny & _dst) noexcept {
        return nullptr;
    }
};

template <class T>
struct _AyAnyHandler<T, _Action::kMoveTo> {
    static void * call(AyAny & _src, AyAny & _dst) noexcept {
        return nullptr;
    }
};
}

inline AyAny::AyAny(AyAny const & _ot) noexcept {
    _ot._callAct(_Action::kCopyTo, this);
    m_act = _ot.m_act;
}

inline AyAny::AyAny(AyAny && _ot) noexcept {
    _ot._callAct(_Action::kMoveTo, this);
    m_act = std::exchange(_ot.m_act, nullptr);
}

inline AyAny & AyAny::operator=(AyAny const & rhs) noexcept {
    if (this != &rhs) [[likely]] {
        rhs._callAct(_Action::kCopyTo, this);
        m_act = rhs.m_act;
    }
    return *this;
}

inline AyAny & AyAny::operator=(AyAny && rhs) noexcept {
    if (this != &rhs) [[likely]] {
        rhs._callAct(_Action::kMoveTo, this);
        m_act = std::exchange(rhs.m_act, nullptr);
    }
    return *this;
}

inline void AyAny::reset() noexcept {
    if (!this->hasValue()) { return; }
    this->_callAct(_Action::kDestroy);
    m_act = nullptr;
}

inline void AyAny::swap(AyAny & rhs) noexcept
{
    if (this == &rhs) [[unlikely]] { return; }
    if (this->hasValue() && rhs.hasValue()) {
        AyAny temp;
        rhs._callAct(_Action::kMoveTo, &temp);
        this->_callAct(_Action::kMoveTo, &rhs);
        temp._callAct(_Action::kMoveTo, this);
    }
    else if (this->hasValue()) {
        rhs = std::move(*this);
    }
    else if (rhs.hasValue()) {
        rhs._callAct(_Action::kMoveTo, this);
        m_act = std::exchange(rhs.m_act, nullptr);
    }
}
}

