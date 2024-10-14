#include <iostream>
#include <utility>
#include "aytl/type_traits/int_seq.hpp"

template <typename T, T... Is>
void abcc(std::integer_sequence<T, Is...>)
{
    (..., (std::cout << Is << std::endl));
}

int main()
{
    using namespace iin;
    using namespace iin::detail;
    
    using s1 = std::integer_sequence<int, 1,2,3>;
    using s2 = std::integer_sequence<int, 3, 4, 5>;
    using s3 = concat_int_seqs<s1, s2, s1, s2>::type;
    abcc(s3());
    return 0;
}

