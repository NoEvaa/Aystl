/**
 * Copyright 2024 NoEvaa
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
#pragma once

#include "aytestm.hpp"

namespace testlib {
enum class TestEnum : int {
    k0 = 0,
    k1, k2, k3, k4, k5,
    k6, k7, k8, k9, k10,
    k11, k12, k13, k14, k15,
    k16, k17, k18, k19, k20,

    k00 = k0, k000 = k00,
};
}

