#include "testlib.h"
#include <type_traits>
#include "aystl/type_traits/int_seq.hpp"

using namespace iin;

using test_seq_0 = int_seq<int>;
using test_seq_1 = int_seq<int, 1, 2, 3>;
using test_seq_2 = int_seq<int, 7, 8, 9>;
using test_seq_3 = int_seq<std::size_t, 7, 8, 9>;

using test_seq_1_2 = int_seq<int, 1, 2, 3, 7, 8, 9>;
using test_seq_1_3 = int_seq<int, 1, 2, 3, 7, 8, 9>;
using test_seq_3_1 = int_seq<std::size_t, 7, 8, 9, 1, 2, 3>;

TEST_CASE("is int seq") {
    CHECK(detail::is_int_seq<test_seq_0>::value);
    CHECK(detail::is_int_seq<test_seq_1>::value);
    CHECK(detail::is_int_seq<test_seq_3>::value);
    CHECK(!detail::is_int_seq<int>::value);
}

TEST_CASE("concat int seqs") {
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_0, test_seq_1>, test_seq_1>);
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_0, test_seq_3>, test_seq_2>);
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_3, test_seq_0>, test_seq_3>);
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_1, test_seq_2>, test_seq_1_2>);
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_1, test_seq_3>, test_seq_1_3>);
    CHECK(std::is_same_v<concat_int_seqs_t<test_seq_3, test_seq_1>, test_seq_3_1>);
}

TEST_CASE("monotone int seq") {
    using test_seq_t1 = int_seq<int, 1, 2, 3, 4>;
    CHECK(std::is_same_v<monotone_int_seq_t<int, 1, 5, 1>, test_seq_t1>);
    using test_seq_t2 = int_seq<int, 1, -1, -3>;
    CHECK(std::is_same_v<monotone_int_seq_t<int, 1, -5, -2>, test_seq_t2>);
    CHECK(std::is_same_v<monotone_int_seq_t<int, 1, 5, 0, -2, -2>, int_seq<int, -2, -2>>);
    CHECK(std::is_same_v<monotone_int_seq_t<int, 1, 1, 1, -2, -2>, int_seq<int, -2, -2>>);
    CHECK(std::is_same_v<monotone_int_seq_t<int, -1, 1, -1>, int_seq<int>>);
}

TEST_CASE("int multi seqs") {
    using test_mseqs_t1 = int_multi_seqs<test_seq_1, test_seq_2>;
    using test_seq_t1 = test_mseqs_t1::template wrapped<concat_int_seqs_t>;
    CHECK(std::is_same_v<test_seq_t1, test_seq_1_2>);

    using test_mseqs_t2 = int_multi_seqs<test_seq_0, test_seq_3>;
    using test_seq_t2 = test_mseqs_t2::template wrapped<concat_int_seqs_t>;
    CHECK(std::is_same_v<test_seq_t2, test_seq_2>);

    using test_mseqs_t3 = int_multi_seqs<
        monotone_int_seq_t<int, -3, 1, 1>,
        monotone_int_seq_t<int, 1, 5, 1>,
        monotone_int_seq_t<int, 5, 8, 1>
    >;
    using test_seq_t3 = test_mseqs_t3::template wrapped<concat_int_seqs_t>;
    CHECK(std::is_same_v<test_seq_t3, monotone_int_seq_t<int, -3, 8, 1>>);
}