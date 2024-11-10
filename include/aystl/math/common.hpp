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

namespace iin {
constexpr decltype(auto) ayAbs(auto const & v) {
    return v < 0 ? -v : v;
}
constexpr decltype(auto) ayMax(auto const & a, auto const & b) {
    return a < b ? b : a;
}
constexpr decltype(auto) ayMin(auto const & a, auto const & b) {
    return a > b ? b : a;
}
}

