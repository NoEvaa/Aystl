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
#include <utility>
#include <algorithm>

#include "aystl/type_traits/utils.hpp"
#include "aystl/utility/type_list.hpp"
#include "aystl/utility/int_seq.hpp"

namespace iin {
template <typename CharT, std::size_t N>
struct ct_str {
    using value_type = CharT;

    std::array<value_type, N> value{};

    consteval ct_str() = default;
    consteval ct_str(CharT const (&s)[N]) noexcept {
        std::copy_n(s, N, value.begin());
    }
    consteval explicit ct_str(char const * p_s, std::size_t sz) {
        std::copy_n(p_s, sz, value.begin());
        value[sz] = '\0';
    }
    template<class _Traits>
    consteval explicit ct_str(std::basic_string_view<CharT, _Traits> const & s)
        : ct_str(s.data(), s.size()) {}

    static constexpr std::size_t size() noexcept { return N - 1U; }
    static constexpr std::size_t capacity() noexcept { return N; }
    static constexpr std::size_t empty() noexcept { return size() == 0; }

    constexpr value_type const * data() const noexcept { return value.data(); }

    constexpr auto begin() noexcept { return value.begin(); }
    constexpr auto begin() const noexcept { return value.begin(); }
    constexpr auto end() noexcept { return value.end(); }
    constexpr auto end() const noexcept { return value.end(); }

    template<class _Traits>
    constexpr explicit operator std::basic_string_view<CharT, _Traits>() const noexcept {
        return std::basic_string_view<CharT, _Traits>{ value.data(), size() };
    }
};

template <ct_str _s>
struct ct_str_t : value_t<_s> {};
template <ct_str _s>
constexpr auto ct_str_v = ct_str_t<_s>::value;

template <typename CharT, std::size_t N, std::size_t M>
constexpr bool operator==(ct_str<CharT, N> const & lhs,
    ct_str<CharT, M> const & rhs) noexcept {
    return static_cast<std::basic_string_view<CharT>>(lhs)
        == static_cast<std::basic_string_view<CharT>>(rhs);
}

template <typename CharT, std::size_t N, std::size_t M>
consteval auto operator+(ct_str<CharT, N> const & lhs,
    ct_str<CharT, M> const & rhs) noexcept -> ct_str<CharT, N + M - 1> {
    ct_str<CharT, N + M - 1> ret{};
    std::copy_n(lhs.begin(), lhs.size(), ret.begin());
    std::copy_n(rhs.begin(), rhs.capacity(), ret.begin() + lhs.size());
    return ret;
}

template<typename CharT, CharT... Cs>
struct char_seq
{
    using value_type = CharT;
    using type       = value_list<Cs...>;

    static constexpr std::size_t size() noexcept { return sizeof...(Cs); }
};

namespace detail {
template <typename T>
struct is_char_seq : std::false_type {};
template<typename CharT, CharT... Cs>
struct is_char_seq<char_seq<CharT, Cs...>> : std::true_type {};
}
template <typename T>
concept CharSeqType = detail::is_char_seq<T>::value;

namespace detail{
template <ct_str _s>
consteval auto ct_str_to_char_seq() noexcept {
    using _type     = decltype(_s);
    using char_type = typename _type::value_type;
    return [&]<auto... Is>(index_seq<Is...>) {
        return char_seq<char_type, std::get<Is>(_s.value)...>{};
    }(std::make_index_sequence<_s.size()>{});
}
}
template <ct_str _s>
using char_seq_t = typename type_t<decltype(detail::ct_str_to_char_seq<_s>())>::type;

namespace detail {
template <ct_str _s, std::size_t _pos, std::size_t _len>
consteval auto _substr() {
    using char_type = decltype(_s)::value_type;
    if constexpr (_len == 0 || _pos > _s.size()) {
        return ct_str<char_type, 1>("");
    } else if constexpr (_pos + _len > _s.size()) {
        return _substr<_s, _pos, _s.size() - _pos>();
    } else {
        return ct_str<char_type, _len + 1>(std::basic_string_view<char_type>(_s).substr(_pos, _len));
    }
}
}
template <ct_str _s, std::size_t _pos, std::size_t _len>
constexpr auto ct_str_substr_v = detail::_substr<_s, _pos, _len>();
}

