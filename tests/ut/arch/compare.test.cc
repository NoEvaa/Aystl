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
#include "aystl/core/arch/compare.hpp"

using namespace iin;

namespace {
struct _TestC1 {
    bool operator==(_TestC1 const &) const { return true; }
};
}

TEST_CASE("AyCmp") {
    CHECK(AyCmp<CmpOp::kEQ, int>{}(1, 1));
    CHECK(AyCmp<CmpOp::kEQ, _TestC1>{}(_TestC1{}, _TestC1{}));
}

