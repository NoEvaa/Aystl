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

template <typename... Ts>
using to_index_seq = index_seq<Ts::value...>;

template <typename _Cmp, typename T, T... Vs>
struct ct_sorted_array {
    using element_type = T;
    using value_type   = std::array<element_type, sizeof...(Vs)>;

    static constexpr value_type value = [] {
        value_type temp = { Vs... };
        std::sort(temp.begin(), temp.end(), _Cmp{});
        return temp;
    }();

    static constexpr std::size_t size() noexcept { return sizeof...(Vs); }

    template <std::size_t pos> requires CtCmp<CmpOp::kLT, pos, size()>
    static constexpr element_type at = std::get<pos>(value);
};

template <typename _Cmp, typename T, T... Vs>
auto _constantListSortImpl(constant_list<T, Vs...>) {
    using sorted_type = ct_sorted_array<_Cmp, T, Vs...>;
    //make_index_seq<sizeof...(Vs)>::template map<sorted_type::template at>;
    return []<auto... Is>(index_seq<Is...>) {
        return constant_list<T, sorted_type::template at<Is>...>{};
    }(make_index_seq<sizeof...(Vs)>{});
}

template <ConstantListType T, typename _Cmp = std::less<>>
using constant_list_sort_t = decltype(_constantListSortImpl<_Cmp>(std::declval<T>()));

int main()
{
    using xxx1 = type_list<plh_t<0>, int, plh_t<5>, plh_t<1>, char, plh_t<2>>;
    using xxx2 = xxx1::map<is_placeholder>;
    using xxx3 = xxx1::filter<xxx2>;
    std::cout << getTypeName<xxx3>() << std::endl;
    using xxx4 = xxx3::wrapped<to_index_seq>;
    std::cout << getTypeName<xxx4>() << std::endl;
    using xxx5 = constant_list_sort_t<xxx4>;
    std::cout << getTypeName<xxx5>() << std::endl;
    using xxx6 = xxx5::type_map<plh_t>;
    std::cout << getTypeName<xxx6>() << std::endl;

    return 0;

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

