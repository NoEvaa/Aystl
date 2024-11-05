#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>
#include "aystl/type_traits.h"
#include "aystl/reflect/type_name.hpp"
#include "aystl/reflect/enum_name.hpp"
#include "aystl/type_traits/utils.hpp"


using namespace iin;
using namespace iin::detail;


template <typename T, T... Is>
void abcc(std::integer_sequence<T, Is...>)
{
    (..., (std::cout << Is << std::endl));
}

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

    using p1 = wrap_sptr_t<int>;
    using p2 = wrap_sptr_t<p1>;
    std::cout << typeid(p1).name() << std::endl;
    std::cout << typeid(p2).name() << std::endl;

    std::cout << ct_cmp<CmpOp::kEQ, 1, 1>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kEQ, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 1>::value << std::endl;
    
    using s1 = std::integer_sequence<int, 1,2,3>;
    using s2 = std::integer_sequence<int, 3, 4, 5>;
    using s3 = concat_int_seqs_t<s1, s2, s1, s2>;
    //abcc(s3());
    std::cout << std::endl;

    using ss1 = monotone_int_seq_t<int, -2, 3, 2>;
    //abcc(ss1());
    using ss2 = monotone_int_seq_t<int, -2, -2, 1>;
    //abcc(ss2());
    using ss3 = monotone_int_seq_t<int, -2, 3, 0>;
    //abcc(ss3());
    using ss4 = monotone_int_seq_t<int, 3, 1, -1>;
    //abcc(ss4());
    return 0;
}

