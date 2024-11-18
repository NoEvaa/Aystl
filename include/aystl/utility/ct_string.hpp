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

#include <cstddef>
#include <array>
#include <string_view>

#include "aystl/type_traits/utils.hpp"

namespace iin {
template <typename CharT, std::size_t N>
struct ct_str {
    std::array<CharT, N> value{};

    consteval ct_str() = default;
    consteval ct_str(CharT const (&s)[N]) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = s[i];
        }
    }

    static constexpr std::size_t size() noexcept {
        return N - 1U;
    }
    static constexpr std::size_t capacity() noexcept {
        return N;
    }
    static constexpr std::size_t empty() noexcept {
        return size() == 0;
    }

    template<class _Traits>
    constexpr explicit operator std::basic_string_view<CharT, _Traits>() const noexcept {
        return std::basic_string_view<CharT, _Traits>{ value.data(), size() };
    }
};

template <typename CharT, std::size_t N, std::size_t M>
constexpr bool operator==(ct_str<CharT, N> const & lhs,
    ct_str<CharT, M> const & rhs) noexcept {
    return static_cast<std::string_view>(lhs)
        == static_cast<std::string_view>(rhs);
}

template <typename CharT, std::size_t N, std::size_t M>
consteval auto operator+(ct_str<CharT, N> const & lhs,
    ct_str<CharT, M> const & rhs) noexcept -> ct_str<CharT, N + M - 1> {
    ct_str<CharT, N + M - 1> ret{};
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        ret.value[i] = lhs.value[i];
    }
    for (std::size_t i = 0; i < rhs.capacity(); ++i) {
        ret.value[i + N - 1] = rhs.value[i];
    }
    return ret;
}

template <ct_str _s>
struct ct_str_t : value_t<_s> {};
}

