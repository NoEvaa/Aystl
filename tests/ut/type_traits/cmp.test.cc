#include "testlib.h"
#include "aystl/type_traits/cmp.hpp"

using namespace iin;

TEST_CASE("ct cmp EQ") {
    constexpr auto op = CmpOp::kEQ;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp NE") {
    constexpr auto op = CmpOp::kNE;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp LT") {
    constexpr auto op = CmpOp::kLT;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp LE") {
    constexpr auto op = CmpOp::kLE;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(ct_cmp_v<op, 0, 1>);
    CHECK(!ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp GT") {
    constexpr auto op = CmpOp::kGT;
    CHECK(!ct_cmp_v<op, 0, 0>);
    CHECK(!ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

TEST_CASE("ct cmp GE") {
    constexpr auto op = CmpOp::kGE;
    CHECK(ct_cmp_v<op, 0, 0>);
    CHECK(ct_cmp_v<op, 1, 1>);
    CHECK(!ct_cmp_v<op, 0, 1>);
    CHECK(ct_cmp_v<op, 1, 0>);
}

