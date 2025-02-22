#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>
#include "aystl/math/bit_utils.hpp"
#include "aystl/type_traits.h"
#include "aystl/math.h"
#include "aystl/reflect/type_name.hpp"
#include "aystl/reflect/enum_name.hpp"
#include "aystl/type_traits/utils.hpp"
#include "aystl/utility.h"
#include "aystl/math/limits.hpp"
#include "aystl/utility/function.hpp"
#include "aystl/utility/hash.hpp"
#include "aystl/utility/int_seq.hpp"
#include "aystl/utility/type_list.hpp"


using namespace iin;
using namespace iin::detail;

// 4567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

template <typename T, std::size_t N>
std::ostream & operator<<(std::ostream & ost, ct_str<T, N> const & s) {
    return (ost << std::string_view(s));
}

template <typename T, T... Is>
void abcc(std::integer_sequence<T, Is...>)
{
    (..., (std::cout << Is << std::endl));
}

template <auto _func>
constexpr auto getFuncName() {
    constexpr auto sample = detail::_getFuncSig<0>();
    constexpr auto entity = detail::_getFuncSig<_func>();

    std::cout << sample << std::endl;
    std::cout << entity << std::endl;
    return entity;

}

enum class ABC {
    k11 = 1,
    k1 = 1,
};

namespace _any_impl {
enum class _Action {
    kTypeInfo = 0,
    kDestroy,
    kCopy,
    kMove,
};
template <class, _Action> struct _AyAnyHandler;
}

class AyAny {
    using _Action   = _any_impl::_Action;
    using _act_type = void * (*)(_Action, AyAny const *);
public:
    using size_type = std::size_t;

    static constexpr size_type kPtrSize     = sizeof(void *);
    static constexpr size_type kMaxPtrSize  = sizeof(uint64_t);
    static constexpr size_type kBufByteSize = kMaxPtrSize * 3;
    static constexpr size_type kBufSize     = kBufByteSize / kPtrSize;
    static_assert(kPtrSize <= kMaxPtrSize);
    static_assert((kBufByteSize % kPtrSize) == 0);

    bool hasValue() const noexcept { return m_act; }
    operator bool() const noexcept { return hasValue(); }

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
        m_act = [](_Action _action, AyAny const * _self) -> void * {
            switch (_action) {
            case _Action::kTypeInfo:
                return _any_impl::_AyAnyHandler<_Tp, _Action::kTypeInfo>::call();
            case _Action::kDestroy:
                return _any_impl::_AyAnyHandler<_Tp, _Action::kDestroy>::call(_self);
            case _Action::kCopy:
            case _Action::kMove:
            default:
                return nullptr;
            }
        };
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
    static void * call() { return detail::_castToVoidPtr(&typeid(T)); }
};

template <class T>
struct _AyAnyHandler<T, _Action::kDestroy> {
    static void * call(AyAny const * _self) {
        if constexpr (sizeof(T) <= AyAny::kBufSize) {
            if constexpr (!std::is_trivially_destructible<T>::value) {
                auto * p_v = _self->_toPtr<T>();
                p_v->~ValT();
                //std::launder(p_v)->~ValT();
            }
        } else if constexpr (std::is_array_v<T>) {
            auto * p_v = _self->_toPtr<T>();
            delete[] p_v;
        } else {
            auto * p_v = _self->_toPtr<T>();
            delete p_v;
        }
        return nullptr;
    }
};
}

#include <any>
int main()
{
    std::any aa = int(1);
    auto ddd =  std::any_cast<const int>(aa);
    std::cout << ddd << std::endl;
    std::cout << sizeof(std::any) << std::endl;
    std::cout << sizeof(AyAny) << std::endl;
    std::cout << sizeof(void*) << std::endl;
    std::cout << sizeof(std::shared_ptr<int>) << std::endl;
    std::cout << sizeof(std::weak_ptr<int>) << std::endl;
    std::cout << sizeof(std::unique_ptr<int>) << std::endl;
    std::cout << alignof(std::max_align_t) << std::endl;

    std::cout << getTypeName<std::shared_ptr<std::string const &>>() << std::endl;

    auto xxx = []<int i>(int a, int b, int c){
        std::cout << i << a << b << c << std::endl;
    };
    //invokeTmplFunc<value_t<1>>(xxx, 1, 2, 3);

    auto xxx2 = []<typename T>(int a, int b, int c){
        std::cout << getTypeName<T>() << std::endl;
        std::cout << a << b << c << std::endl;
    };
    //invokeTmplFunc<int>(xxx2, 1, 2, 3);

    getFuncName<getEnumName<CmpOp::kEQ>>();

    std::cout << getEnumName<ABC::k1>() << std::endl;
    std::cout << getEnumName<ABC::k11>() << std::endl;
    std::cout << getEnumName<ABC(1)>() << std::endl;

    std::cout << getEnumName<CmpOp::kEQ>() << std::endl;
    std::cout << getEnumName<CmpOp(0)>() << std::endl;
    std::cout << getEnumName<CmpOp(-1)>() << std::endl;
    std::cout << is_enum_declared_v<CmpOp(-1)> << std::endl;
    std::cout << is_enum_declared_v<CmpOp(0)> << std::endl;

    return 0;
}

