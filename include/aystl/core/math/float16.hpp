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
    float _toFp32() const noexcept;

private:
    uint16_t m_value{};
};

namespace detail {
union Float32Bits {
    float    f;
    uint32_t u;
};
}

/**
 * Inspired by SwiftShader::half.
 */
inline void AyFloat16::_fromFp32(float _fp32) noexcept {
    constexpr uint32_t _kFp32InfVal = 255 << 23;
    constexpr uint32_t _kFp16MaxVal = (127 + 16) << 23;

    uint32_t _fp32ui = std::bit_cast<uint32_t>(_fp32);
    uint32_t _sign = (_fp32ui & 0x80000000) >> 16;
    uint32_t _abs = _fp32ui & 0x7FFFFFFF;

    if (_abs >= _kFp16MaxVal) {
        // Inf or NaN 
        m_value = _abs > _kFp32InfVal ? AyFloat16::kNaNValue : AyFloat16::kInfValue;
    } else if (_abs < 0x38800000) {
        // (De)normalized number or zero
		uint32_t _e = 113 - (_abs >> 23);
        if (_e < 24) {
            _abs = ((_abs & 0x007FFFFF) | 0x00800000) >> _e;
            m_value = (_abs + 0x00000FFF + ((_abs >> 13) & 1)) >> 13;
        } else {
            m_value = 0x00000FFF >> 13;
        }
	} else {
		m_value = (_abs + 0xC8000FFF + ((_abs >> 13) & 1)) >> 13;
	}

    // Sign bit
    m_value |= _sign;
}

/**
 * Inspired by Eigen::half.
 */
inline float AyFloat16::_toFp32() const noexcept {
    constexpr uint32_t _kShiftedExp = 0x7C00 << 13;
    constexpr float _kMagic = std::bit_cast<float>(uint32_t{ 113 << 23 });

    detail::Float32Bits o;

    o.u = (m_value & 0x7FFF) << 13;    // Exponent/mantissa bits
    uint32_t exp = _kShiftedExp & o.u; // Just the exponent
    o.u += (127 - 15) << 23;           // Exponent adjust

    if (exp == _kShiftedExp) {
        // Inf or NaN 
        o.u += (128 - 16) << 23; // Extra exp adjust
    } else if (exp == 0) {
        // (De)normalized number or zero
        o.u += 1 << 23; // Extra exp adjust
        o.f -= _kMagic; // Renormalize
    }

    // Sign bit
    o.u |= (m_value & 0x8000) << 16;
    return o.f;
}
}

