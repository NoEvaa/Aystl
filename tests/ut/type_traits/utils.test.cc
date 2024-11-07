#include "testlib.h"
#include <type_traits>
#include "aystl/type_traits/utils.hpp"

using namespace iin;

TEST_CASE("is any of") {
    CHECK(is_any_of_v<std::true_type, std::true_type>);
    CHECK(is_any_of_v<std::true_type, std::false_type>);
    CHECK(is_any_of_v<std::false_type, std::true_type>);
    CHECK(!is_any_of_v<std::false_type, std::false_type>);
}

TEST_CASE("is all of") {
    CHECK(is_all_of_v<std::true_type, std::true_type>);
    CHECK(!is_all_of_v<std::true_type, std::false_type>);
    CHECK(!is_all_of_v<std::false_type, std::true_type>);
    CHECK(!is_all_of_v<std::false_type, std::false_type>);
}

template <typename T, typename T2 = int>
struct TestTmpl {};
template <typename... Args>
using Tmpl1 = TestTmpl<Args...>;
template <typename T>
using Tmpl2 = TestTmpl<T>;
template <typename T, typename T2 = int>
struct TestTmplB {};
template <int _>
struct TestTmplC {};

TEST_CASE("is spec of") {
    CHECK(is_spec_of_v<TestTmpl<int>, TestTmpl>);
    CHECK(is_spec_of_v<TestTmpl<int>, Tmpl1>);
    CHECK(!is_spec_of_v<TestTmpl<int>, Tmpl2>);

    CHECK(is_spec_of_v<Tmpl1<int>, TestTmpl>);
    CHECK(is_spec_of_v<Tmpl1<int>, Tmpl1>);
    CHECK(!is_spec_of_v<Tmpl1<int>, Tmpl2>);

    CHECK(is_spec_of_v<Tmpl2<int>, TestTmpl>);
    CHECK(is_spec_of_v<Tmpl2<int>, Tmpl1>);
    CHECK(!is_spec_of_v<Tmpl2<int>, Tmpl2>);

    CHECK(!is_spec_of_v<int, TestTmpl>);
    CHECK(!is_spec_of_v<Tmpl1<int>, TestTmplB>);
}

TEST_CASE("wrap tmpl") {
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int>, TestTmpl<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, int, float>, Tmpl1<int, float>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmpl, Tmpl1<int>>, Tmpl1<int>>);
    CHECK(std::is_same_v<wrap_tmpl_t<TestTmplB, Tmpl1<int>>, TestTmplB<Tmpl1<int>>>);
}

TEST_CASE("replace tmpl args") {
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmpl<int>, float, bool>, Tmpl1<float, bool>>);
    CHECK(std::is_same_v<replace_tmpl_args_t<int, float, bool>, int>);
    CHECK(std::is_same_v<replace_tmpl_args_t<TestTmplC<1>, int>, TestTmplC<1>>);
}

TEST_CASE("replace tmpl wrapper") {
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<Tmpl1<int, bool>, TestTmplB>, TestTmplB<int, bool>>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<int, TestTmplB>, int>);
    CHECK(std::is_same_v<replace_tmpl_wrapper_t<TestTmplC<1>, TestTmplB>, TestTmplC<1>>);
}

