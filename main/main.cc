#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include "aystl/type_traits.h"
#include "aystl/reflect/type_name.hpp"
#include "aystl/reflect/enum_name.hpp"
#include "aystl/type_traits/int_seq.hpp"
#include "aystl/type_traits/utils.hpp"


using namespace iin;
using namespace iin::detail;

// 4567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

template <typename T, T... Is>
void abcc(std::integer_sequence<T, Is...>)
{
    (..., (std::cout << Is << std::endl));
}

struct dde{
void hha() const volatile && noexcept{}
};

template <auto _func>
std::string_view getFuncName() {
    constexpr auto sample = detail::_getFuncSig<0>();
    constexpr auto entity = detail::_getFuncSig<_func>();

    std::cout << sample << std::endl;
    std::cout << entity << std::endl;
    return entity;

}

int main()
{
    dde().hha();

    getFuncName<getEnumName<CmpOp::kEQ>>();

    std::cout << getEnumName<CmpOp::kEQ>() << std::endl;
    std::cout << getEnumName<CmpOp(0)>() << std::endl;
    std::cout << getEnumName<CmpOp(-1)>() << std::endl;
    std::cout << is_enum_declared_v<CmpOp(-1)> << std::endl;
    std::cout << is_enum_declared_v<CmpOp(0)> << std::endl;

    std::cout << getTypeName<double>() << std::endl;
    std::cout << getTypeName<int_multi_seqs<>>() << std::endl;
    std::cout << getTypeName<std::shared_ptr<int>>() << std::endl;
    std::cout << getTypeName<std::int64_t*>() << std::endl;

    std::cout << ct_cmp<CmpOp::kEQ, 1, 1>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kEQ, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 1>::value << std::endl;
    
    return 0;
}

