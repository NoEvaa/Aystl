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

#include <bit>
#include <cstdint>

namespace iin {
class AyFloat16 {
public:
    static constexpr uint16_t kNaNValue = 0x7e00;
    static constexpr uint16_t kInfValue = 0x7c00;

    AyFloat16() = default;
    ~AyFloat16() noexcept = default;

    AyFloat16(AyFloat16 const &) noexcept = default;
    AyFloat16(AyFloat16 &&) noexcept = default;

    AyFloat16 & operator=(AyFloat16 const &) noexcept = default;
    AyFloat16 & operator=(AyFloat16 &&) noexcept = default;

private:
    void _fromFp32(float) noexcept;
    float _toFp32() noexcept;

private:
    uint16_t m_value{};
};

inline void AyFloat16::_fromFp32(float _fp32) noexcept {
    constexpr uint32_t _kFp32InfVal = 255 << 23;
    constexpr uint32_t _kFp16MaxVal = (127 + 16) << 23;

    uint32_t _fp32ui = std::bit_cast<uint32_t>(_fp32);
    uint32_t _sign = (_fp32ui & 0x80000000) >> 16;
    uint32_t _abs = _fp32ui & 0x7FFFFFFF;

    if (_abs >= _kFp16MaxVal) {
        // Inf or NaN 
        m_value = _sign | (_abs > _kFp32InfVal ? AyFloat16::kNaNValue : AyFloat16::kInfValue);
        return;
    } else if (_abs < 0x38800000) {
        // (De)normalized number or zero
		uint32_t _mantissa = (_abs & 0x007FFFFF) | 0x00800000;
		uint32_t _e = 113 - (_abs >> 23);
        _abs = _e < 24 ? (_mantissa >> _e) : 0;
		m_value = _sign | (_abs + 0x00000FFF + ((_abs >> 13) & 1)) >> 13;
        return;
	} else {
		m_value = _sign | (_abs + 0xC8000FFF + ((_abs >> 13) & 1)) >> 13;
        return;
	}
}
}

