/**
 * Copyright 2025 NoEvaa
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "testlib.h"
#include "aystl/utility/any.hpp"

#include <array>

using namespace iin;

TEST_CASE("AyAny") {
    CHECK(sizeof(AyBasicAny<AyAlloc<void>, value_t<std::size_t{16U}>>) == 16);
    CHECK(sizeof(AyBasicAny<AyAlloc<void>, constant_t<std::size_t, 32U>>) == 32);
    CHECK(sizeof(AyBasicAny<AyAlloc<void>, constant_t<std::size_t, 64U>>) == 64);
    CHECK(sizeof(AyBasicAny<AyAlloc<void>, constant_t<std::size_t, 128U>>) == 128);

    AyAny a;
    CHECK(!a.hasValue());
    CHECK(!bool(a));
    CHECK(a.isType<void>());
    CHECK(!a.isType<int>());
    CHECK(a.isType(typeid(void)));
    CHECK(a.getValuePtr<int>() == nullptr);

    a = int(1);
    REQUIRE(a.hasValue());
    CHECK(bool(a));
    CHECK(!a.isType<void>());
    REQUIRE(a.isType<int>());
    CHECK(a.isType(typeid(int)));
    CHECK(a.getValue<int>() == 1);
    a.getValue<int &>() = 2;
    CHECK(*a.getValuePtr<int>() == 2);
    a.fastGetValue<int &>() = 3;
    CHECK(a.getValue<int const &>() == 3);

    a.setValue<bool>(true);
    CHECK(a.hasValue());
    CHECK(a.isType<bool>());
    CHECK(a.getValuePtr<int>() == nullptr);
    CHECK(a.getValue<bool>() == true);
    CHECK_THROWS_AS(std::runtime_error, a.getValue<int>());

    a.reset();
    CHECK(!a.hasValue());
    CHECK(a.getValuePtr<int>() == nullptr);
    CHECK_THROWS_AS(std::runtime_error, a.getValue<int>());
}

TEST_CASE("AyAny array") {
    AyAny a = std::array<int, 10>({0});
    REQUIRE(a.hasValue());
    REQUIRE(a.isType<std::array<int, 10>>());
    auto & arr = a.fastGetValue<std::array<int, 10> &>();
    for (int i = 0; i < 10; ++i) {
        CHECK(arr[i] == 0);
        arr[i] = i;
    }
    auto arr_temp = a.getValue<std::array<int, 10>>();
    for (int i = 0; i < 10; ++i) {
        CHECK(arr_temp[i] == i);
    }
}

namespace {
struct _TestC {
    static int v_;
    static int w_;

    _TestC() { v_ = 1; }
    _TestC(int) { v_ = 2; }
    _TestC(int, int) { v_ = 3; }
    ~_TestC() { ++w_; };

    _TestC(_TestC const &) { v_ = 5; }
    _TestC(_TestC &&) { v_ = 6; }
    _TestC & operator=(_TestC const &) { v_ = 7; return *this; }
    _TestC & operator=(_TestC &&) { v_ = 8; return *this; }

};
int _TestC::v_ = 0;
int _TestC::w_ = 0;
}

TEST_CASE("AyAny class") {
    _TestC::v_ = 0;
    _TestC::w_ = 0;
    
    AyAny a = _TestC{};
    CHECK(_TestC::v_ == 6);
    CHECK(_TestC::w_ == 1);

    SECTION("s1") {
        auto t = _TestC{1};
        CHECK(_TestC::v_ == 2);
        CHECK(_TestC::w_ == 1);

        auto b = t;
        CHECK(_TestC::v_ == 5);
        CHECK(_TestC::w_ == 1);

        a.setValue<_TestC>();
        CHECK(_TestC::v_ == 1);
        CHECK(_TestC::w_ == 2);
        a.setValue<_TestC>(1, 2);
        CHECK(_TestC::v_ == 3);
        CHECK(_TestC::w_ == 3);
    }

    SECTION("s2") {
        AyAny a1 = a;
        CHECK(_TestC::v_ == 5);
        CHECK(_TestC::w_ == 1);

        AyAny a2 = std::move(a);
        CHECK(_TestC::v_ == 6);
        CHECK(_TestC::w_ == 1);
        CHECK(!a.hasValue());

        a1 = a2;
        CHECK(_TestC::v_ == 5);
        CHECK(_TestC::w_ == 2);

        a1 = std::move(a2);
        CHECK(_TestC::v_ == 6);
        CHECK(_TestC::w_ == 3);
    }

    SECTION("s3") {
        {
            AyAny c = _TestC{};
            _TestC::w_ = 0;
        }
        CHECK(_TestC::w_ == 1);

        a.reset();
        CHECK(_TestC::w_ == 2);
    }
}

TEST_CASE("AyAny shared_ptr") {
    auto p = std::make_shared<int>();
    REQUIRE(p.use_count() == 1);
    {
        AyAny a = p;
        CHECK(p.use_count() == 2);
        AyAny b = std::move(a);
        CHECK(p.use_count() == 2);
    }
    CHECK(p.use_count() == 1);
}

