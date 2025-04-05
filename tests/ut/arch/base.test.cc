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
#include "aystl/core/arch/base.hpp"

using namespace iin;

namespace {
enum class _TestE {
    kBool = 0,
    kInt = 1,
    kFloat = 2,
};

template <_TestE, typename T>
struct _TestECase;
}

namespace iin {
template <_TestE _e>
struct AyArch<value_t<_e>> : AyArchBaseVT<_TestE, value_t<_e>, _TestECase> {};
template <_TestE _e>
using _test_e_case_t = AyArch<value_t<_e>>;
}

namespace {
template <typename T>
struct _TestECase<_TestE::kBool, T> {
    bool operator()(T && v) noexcept {
        return static_cast<bool>(v);
    }
};

template <typename T>
struct _TestECase<_TestE::kInt, T> {
    int operator()(T && v) noexcept {
        return static_cast<int>(v);
    }
};
template <typename T>
struct _TestECase<_TestE::kFloat, T> {
    float operator()(T && v) noexcept {
        return static_cast<float>(v);
    }
};
}

TEST_CASE("AyArchBaseVT") {
    using _test_e_bool = _test_e_case_t<_TestE::kBool>;
    auto a1 = build_arch_t<_test_e_bool, int>{}(1);
    CHECK(std::is_same_v<decltype(a1), bool>);
    CHECK(a1 == true);
    auto a2 = ayArchCall<_test_e_bool, int>(0);
    CHECK(std::is_same_v<decltype(a2), bool>);
    CHECK(a2 == false);

    using _test_e_int = _test_e_case_t<_TestE::kInt>;
    auto b1 = ayArchCall<_test_e_int, float>(2.3);
    CHECK(std::is_same_v<decltype(b1), int>);
    CHECK(b1 == 2);

    using _test_e_float = _test_e_case_t<_TestE::kFloat>;
    auto c1 = ayArchCall<_test_e_float, bool>(true);
    CHECK(std::is_same_v<decltype(c1), float>);
}

