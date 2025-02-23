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
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include "aystl/core/memory/allocator.hpp"
#include "aystl/core/type_traits/utils.hpp"

namespace iin {
namespace _any_impl {
enum class _Action {
    kTypeInfo = 0,
    kGet,
    kCreate,
    kDestroy,
    kCopyTo,
    kMoveTo,
};
template <typename, _Action> struct _AyAnyHandler;
template <typename> struct _AyAnyAlloc;
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

    AyAny() noexcept = default;
    AyAny(AyAny const & _ot) { _ot._copyTo(this); }
    AyAny(AyAny && _ot) noexcept { _ot._moveTo(this); }

    ~AyAny() noexcept { this->reset(); }

    AyAny & operator=(AyAny const &);
    AyAny & operator=(AyAny &&) noexcept;

    void reset() noexcept;

    void swap(AyAny &) noexcept;

    bool hasValue() const noexcept { return m_act != nullptr; }
    operator bool() const noexcept { return this->hasValue(); }

    std::type_info const & getType() const noexcept {
        if (!this->hasValue()) { return typeid(void); }
        return *static_cast<std::type_info const *>(this->_callAct(_Action::kTypeInfo));
    }
    bool isType(std::type_info const & _ti) const noexcept { return this->getType() == _ti; }
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
        auto * _p = getValuePtr<_Tp>();
        if (!_p) {
            throw std::runtime_error{"iin::AyAny can not access to value."};
        }
        return static_cast<_Tp>(*_p);
    }

private:
    void * _toVoidPtr() const noexcept { return this->_callAct(_Action::kGet); }

    template <typename _Tp>
    _Tp * _toPtr() const noexcept { return static_cast<_Tp *>(_toVoidPtr()); }

    template <typename _Tp>
    void _initAct() noexcept;

    void * _callAct(_Action _action, AyAny * _other = nullptr) const {
        return m_act(_action, this, _other);
    }

    void _copyTo(AyAny * _dst) const {
        this->_callAct(_Action::kCopyTo, _dst);
        _dst->m_act = this->m_act;
    }

    void _moveTo(AyAny * _dst) noexcept {
        this->_callAct(_Action::kMoveTo, _dst);
        _dst->m_act = std::exchange(m_act, nullptr);
    }

private:
    void *    m_buf[kBufSize] = {};
    _act_type m_act = nullptr;

    template <typename, _Action>
    friend struct _any_impl::_AyAnyHandler;
};

namespace _any_impl {
template <typename T>
inline constexpr bool _is_inp_value_v = value_t<(sizeof(T) <= AyAny::kBufByteSize)>::value;

template <typename T>
struct _AyAnyHandler<T, _Action::kTypeInfo> {
    static void * call() noexcept {
        return const_cast<void *>(static_cast<void const *>(&typeid(T)));
    }
};

template <typename T>
struct _AyAnyHandler<T, _Action::kGet> {
    static void * call(AyAny & _self) noexcept { 
        if constexpr (_is_inp_value_v<T>) {
            return static_cast<void *>(&_self.m_buf);
        } else {
            return static_cast<void *>(_self.m_buf[0]);
        }
    }
};

template <typename T>
struct _AyAnyHandler<T, _Action::kCreate> {
    static void * call(AyAny & _self) {
        T * _p = nullptr;
        if constexpr (_is_inp_value_v<T>) {
            _p = static_cast<T *>(static_cast<void *>(&_self.m_buf));
            //std::construct_at(_p, );
        } else {
            using _alloc_type = _AyAnyAlloc<T>;
            _p = _alloc_type::allocate();
            _self.m_buf[0] = _p;
        }
        return nullptr;
    }
};

template <typename T>
struct _AyAnyHandler<T, _Action::kDestroy> {
    static void * call(AyAny const & _self) noexcept {
        if constexpr (_is_inp_value_v<T>) {
            if constexpr (!std::is_trivially_destructible<T>::value) {
                std::destroy_at(_self._toPtr<T>());
            }
        } else {
            using _alloc_type = _AyAnyAlloc<T>;
            auto * p_v = _self._toPtr<T>();
            _alloc_type::destroy(p_v);
        }
        return nullptr;
    }
};

template <typename T>
struct _AyAnyHandler<T, _Action::kCopyTo> {
    static void * call(AyAny const & _src, AyAny & _dst) {
        return nullptr;
    }
};

template <typename T>
struct _AyAnyHandler<T, _Action::kMoveTo> {
    static void * call(AyAny & _src, AyAny & _dst) noexcept {
        if constexpr (_is_inp_value_v<T>) {
            std::memcpy(&_dst.m_buf, &_src.m_buf, sizeof(T));
        } else {
            std::memcpy(&_dst.m_buf, &_src.m_buf, AyAny::kPtrSize);
        }
        return nullptr;
    }
};

template <typename T>
struct _AyAnyAlloc {
    static void construct() noexcept {}

    static void destroy(T * __p) noexcept {
        if constexpr (std::is_array_v<T>) {
            delete[] __p;
        } else {
            delete __p;
        }
    }
};
}

inline AyAny & AyAny::operator=(AyAny const & rhs) {
    if (this != &rhs) [[likely]] {
        this->reset();
        rhs._copyTo(this);
    }
    return *this;
}

inline AyAny & AyAny::operator=(AyAny && rhs) noexcept {
    if (this != &rhs) [[likely]] {
        this->reset();
        rhs._moveTo(this);
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
        this->_moveTo(&temp);
        rhs._moveTo(this);
        temp._moveTo(&rhs);
    } else if (this->hasValue()) {
        this->_moveTo(&rhs);
    } else if (rhs.hasValue()) {
        rhs._moveTo(this);
    }
}

template <typename _Tp>
void AyAny::_initAct() noexcept {
    m_act = [](_Action _action, AyAny const * _this, AyAny * _ot) -> void * {
        using _any_impl::_AyAnyHandler;
        switch (_action) {
        case _Action::kTypeInfo:
            return _AyAnyHandler<_Tp, _Action::kTypeInfo>::call();
        case _Action::kGet:
            return _AyAnyHandler<_Tp, _Action::kGet>::call(const_cast<AyAny &>(*_this));
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
}

