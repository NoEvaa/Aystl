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

#include <any>
int main()
{
    std::any aa = int(1);
    auto ddd =  std::any_cast<const int>(aa);
    std::cout << ddd << std::endl;
    std::cout << sizeof(std::any) << std::endl;
    std::cout << sizeof(iin::AyAny) << std::endl;
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

