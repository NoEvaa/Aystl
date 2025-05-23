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
#pragma once

#include <functional>

#include "aystl/global/common.hpp"

namespace iin {
template <typename FnT = std::function<void(void)>>
requires requires (FnT fn) { fn(); bool(fn); }
class ScopeGuard {
public:
    ScopeGuard(FnT && fn) noexcept : m_fn{fn} {}
    ~ScopeGuard() noexcept { reset(); }

    AY_DISABLE_COPY(ScopeGuard)

    ScopeGuard(ScopeGuard && ot) noexcept {
        reset();
        m_fn = std::exchange(ot.m_fn, FnT{});
    }
    ScopeGuard & operator=(ScopeGuard && ot) noexcept {
        reset();
        m_fn = std::exchange(ot.m_fn, FnT{});
    }

    void reset() noexcept {
        if (m_fn) {
            m_fn();
            m_fn = FnT{};
        }
    }

    void discard() noexcept { m_fn = FnT{}; }

private:
    FnT m_fn{};
};
}

