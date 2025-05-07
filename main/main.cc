#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string_view>
#include <utility>
#include "aystl/aystl.hpp"
#include "aystl/utility.hpp"
#include "aystl/reflect/type_name.hpp"
#include "aystl/reflect/enum_name.hpp"
#include "aystl/utility/hash.hpp"
#include "aystl/tmp/utils/placeholder.hpp"
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

#include <any>

template <typename T>
void ppp(T && v) {
    std::cout << getTypeName<decltype(v)>() << std::endl;
}

void foo(auto &&... args) {
    (ppp(std::forward<decltype(args)>(args)), ...);
}

template <TyTmplType TmplT, TyListType TmplArgs = type_list<>>
struct currying_template;

template <TyTmplType TmplT, TyListType TmplArgs>
struct currying_template {
    using tmpl_type = TmplT;
    using args_type = TmplArgs;
    using plhs_type = sorted_placeholders_t<args_type>;
};

template <typename...>
struct abc_de;
template <typename...>
struct abc_fg;

int main()
{
    #if 0
    using fff1 = constant_list<int, 1,2,0,3>::apply_algo<detail::ct_std_reverse_t>;
    std::cout << getTypeName<fff1>() << std::endl;
    using ttt1 = rec_tmpl_t<ty_tmpl_t<abc_fg>, ty_list_tt, ty_tmpl_t<abc_de>, ty_list_tt>;
    using ttt2 = rec_tmpl_t<ty_list_tt, ttt1>;
    using fff2 = meta_wrap_t<ttt2, type_list<int, int>>;
    std::cout << getTypeName<fff2>() << std::endl;
    #endif

    using xxx1 = type_list<plh_t<0>, int, plh_t<5>, plh_t<1>, char, plh_t<2>, plh_t<1>>;
    using b1_type = type_list<bool, double, plh_t<10>>;
    using a1_type = xxx1;
    std::cout << getTypeName<a1_type>() << std::endl;
    std::cout << getTypeName<b1_type>() << std::endl;
    using ooo1 = _tmp_impl::tmpl_args_bind<a1_type, b1_type>::type;
    std::cout << getTypeName<ooo1>() << std::endl;
    using ooo2 = _tmp_impl::tmpl_args_bind<ooo1, type_list<value_t<1>, value_t<2>, value_t<3>>>::type;
    std::cout << getTypeName<ooo2>() << std::endl;

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

