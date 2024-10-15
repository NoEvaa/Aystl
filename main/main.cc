#include <iostream>
#include <utility>
#include "aytl/type_traits/int_seq.hpp"
#include "aytl/type_traits/cmp.hpp"

template <typename T, T... Is>
void abcc(std::integer_sequence<T, Is...>)
{
    (..., (std::cout << Is << std::endl));
}

int main()
{
    using namespace iin;
    using namespace iin::detail;

    std::cout << ct_cmp<CmpOp::kEQ, 1, 1>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kEQ, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 2>::value << std::endl;
    std::cout << ct_cmp<CmpOp::kLT, 1, 1>::value << std::endl;
    
    using s1 = std::integer_sequence<int, 1,2,3>;
    using s2 = std::integer_sequence<int, 3, 4, 5>;
    using s3 = concat_int_seqs_t<s1, s2, s1, s2>;
    abcc(s3());
    std::cout << std::endl;

    using ss1 = monotone_int_seq_t<int, -2, 3, 2>;
    abcc(ss1());
    using ss2 = monotone_int_seq_t<int, -2, -2, 1>;
    abcc(ss2());
    using ss3 = monotone_int_seq_t<int, -2, 3, 0>;
    abcc(ss3());
    using ss4 = monotone_int_seq_t<int, 3, 1, -1>;
    abcc(ss4());
    return 0;
}

