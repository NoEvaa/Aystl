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
namespace detail {
template <typename CharT, std::size_t N>
struct ct_str_base {
    std::array<CharT, N> value{};

    consteval ct_str_base() noexcept = default;
    consteval ct_str_base(char const * p_s, std::size_t sz)  noexcept {
        for (std::size_t i = 0; i < sz; ++i) {
            value[i] = p_s[i];
        }
    }
    consteval ct_str_base(CharT const (&s)[N]) noexcept
        : ct_str_base(s, N) {}
    consteval ct_str_base(std::string_view s) noexcept
        : ct_str_base(s.data(), s.size()) {}

    static constexpr std::size_t size() noexcept {
        return N - static_cast<std::size_t>(1);
    }
    static constexpr std::size_t capacity() noexcept {
        return N;
    }
    static constexpr std::size_t empty() noexcept {
        return size() == 0;
    }

    constexpr operator std::string_view() const noexcept {
        return std::string_view{ value.cbegin(), size() };
    }

    template <std::size_t M>
    constexpr bool operator==(ct_str_base<CharT, M> const & rhs) noexcept {
        return static_cast<std::string_view>(*this)
            == static_cast<std::string_view>(rhs);
    }

    template <std::size_t M>
    constexpr auto operator+(ct_str_base<CharT, M> const & rhs) noexcept {
        ct_str_base<CharT, N + M - 1> ret{};
        for (std::size_t i = 0; i < size(); ++i) {
            ret.value[i] = value[i];
        }
        for (std::size_t i = 0; i < rhs.capacity(); ++i) {
            ret.value[i + N - 1] = rhs.value[i];
        }
        return ret;
    }
};
}
template <std::size_t N>
using ct_str = detail::ct_str_base<char, N>;

template <ct_str _s>
struct ct_str_t : value_t<_s> {};
}

