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
#include "aystl/tmp/utils/ct_sorted_array.hpp"
#include "aystl/tmp/utils/placeholder.hpp"
#include "aystl/tmp.hpp"


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

template <typename T>
void ppp(T && v) {
    std::cout << getTypeName<decltype(v)>() << std::endl;
}

void foo(auto &&... args) {
    (ppp(std::forward<decltype(args)>(args)), ...);
}
#if 0
template <template <typename...> typename Tmpl,
    TypeListType TmplArgs = type_list<>>
struct currying_template : template_t<Tmpl> {
    using args_type = TmplArgs;
    using plhs_type = sorted_placeholders_t<args_type>;
    
    static constexpr std::size_t size() noexcept { return plhs_type::size(); }

};
#endif
int main()
{
#if 0
    using xxx1 = type_list<plh_t<0>, int, plh_t<5>, plh_t<1>, char, plh_t<2>>;
    using xxx2 = xxx1::slice<make_index_seq<xxx1::size()>::template filter<value_t_list<true, false>>>;
    std::cout << getTypeName<xxx2>() << std::endl;

    using xxx7 = sorted_placeholders_t<xxx1>;
    std::cout << getTypeName<xxx7::type>() << std::endl;
    return 0;
#endif

    int i = 3;
    foo(1, i, std::move(i));

    std::cout << sizeof(std::any) << std::endl;
    std::cout << sizeof(iin::AyAny) << std::endl;
    std::cout << sizeof(void*) << std::endl;
    std::cout << sizeof(std::shared_ptr<int>) << std::endl;
    std::cout << sizeof(std::weak_ptr<int>) << std::endl;
    std::cout << sizeof(std::unique_ptr<int>) << std::endl;
    std::cout << alignof(std::max_align_t) << std::endl;
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

