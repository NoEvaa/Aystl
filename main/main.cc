#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string_view>
#include <utility>
#include <any>
#include "aystl/aystl.hpp"
#include "aystl/utility.hpp"
#include "aystl/reflect/type_name.hpp"
#include "aystl/reflect/enum_name.hpp"
#include "aystl/tmp.hpp"


using namespace iin;
using namespace iin::detail;

// 4567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

template <typename T, std::size_t N>
std::ostream & operator<<(std::ostream & ost, ct_str<T, N> const & s) {
    return (ost << std::string_view(s));
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

template <typename T>
void ppp(T && v) {
    std::cout << getTypeName<decltype(v)>() << std::endl;
}

void foo(auto &&... args) {
    (ppp(std::forward<decltype(args)>(args)), ...);
}

template <typename...>
struct abc_de;
template <typename...>
struct abc_fg;

template <typename...> struct TestC1 {};
int main()
{
    using uuu1 = value_list<1, 2, 3>;
    using uuu2 = meta_wrap_t<compose_tmpl_t<ty_tmpl_t<TestC1>>, uuu1>;
    std::cout << getTypeName<uuu2>() << std::endl;
    return 0;

    #if 0
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
    #endif
    return 0;
}

