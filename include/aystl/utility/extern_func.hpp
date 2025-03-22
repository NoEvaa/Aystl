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

#include "aystl/utility/ct_string.hpp"

template <iin::ct_str _name, typename RetTy = void, typename... Args>
extern auto ay_ext_func(Args...) -> RetTy;

namespace iin{
template <ct_str _name, typename RetT = void, typename... Args>
constexpr auto ext_func_lambda = [](Args... args) -> RetT {
    return ay_ext_func<_name, RetT, Args...>(std::forward<Args>(args)...);
};

template <ct_str _name, typename RetT = void, typename... Args>
using ext_func_lambda_t = decltype(ext_func_lambda<_name, RetT, Args...>);
}

