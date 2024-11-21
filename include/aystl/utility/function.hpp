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

#include "aystl/type_traits/utils.hpp"

namespace iin {
template <typename... Ts>
decltype(auto) invokeTmplFunc(auto _fn, auto... args) {
    using ret_type = decltype(_fn.template operator()<take_off<Ts>::thing...>(args...));
    if constexpr (std::is_void_v<ret_type>) {
        _fn.template operator()<take_off<Ts>::thing...>(args...);
        return;
    } else {
        return _fn.template operator()<take_off<Ts>::thing...>(args...);
    }
}
}

