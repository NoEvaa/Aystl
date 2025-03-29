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

#include <cstdint>
#include <cassert>

#include "aystl/global/system.hpp"
#include "aystl/global/compiler.hpp"
#include "aystl/global/enum.hpp"

#define AY_CAT(a, b)   AY_CAT_IMPL(a, b)
#define AY_STR(...)    AY_STR_IMPL(__VA_ARGS__)
#define AY_EXPAND(...) AY_EXPAND_IMPL(__VA_ARGS__)
#define AY_EMPTY(...)  AY_EMPTY_IMPL(__VA_ARGS__)

#define AY_CAT_IMPL(a, b) a##b
#define AY_STR_IMPL(...) #__VA_ARGS__
#define AY_EXPAND_IMPL(...) __VA_ARGS__
#define AY_EMPTY_IMPL(...)

#define AY_UNUSED(...) static_cast<void>(__VA_ARGS__)

#if defined(AY_CC_MSVC)
#define AY_FUNCSIG __FUNCSIG__
#else
#define AY_FUNCSIG __PRETTY_FUNCTION__
#endif

// assert
#if defined(_DEBUG)
#define AY_ASSERT(...) assert(__VA_ARGS__)
#else
#define AY_ASSERT(...) 
#endif

// unreachable
#define AY_UNREACHABLE(...) AY_ASSERT(0)

#define AY_DECL_CLASS_COPY(_class, _mode)                                                          \
    _class(_class const &) = _mode;                                                                \
    _class & operator=(_class const &) = _mode;
#define AY_DECL_CLASS_MOVE(_class, _mode)                                                          \
    _class(_class &&) = _mode;                                                                     \
    _class & operator=(_class &&) = _mode;

#define AY_DISABLE_COPY(_class) AY_DECL_CLASS_COPY(_class, delete)
#define AY_DISABLE_MOVE(_class) AY_DECL_CLASS_MOVE(_class, delete)

namespace iin {
// compare operation
enum class CmpOp : std::uint8_t {
    _kB0 = 0x01, // ==
    _kB1 = 0x02, // <
    _kB2 = 0x04, // >
    _kB3 = 0x08, // Fuzzy
    _kB4 = 0x10,
    _kB5 = 0x20,
    _kB6 = 0x40,
    _kB7 = 0x80,

    kEQ = _kB0,        // equal to
    kNE = _kB1 | _kB2, // not equal
    kLT = _kB1,        // less than
    kLE = _kB1 | _kB0, // less or equal
    kGT = _kB2,        // greater than
    kGE = _kB2 | _kB0, // greater or equal

    k3Way = kLT | kEQ | kGT, // <=>

    kFuzzy   = _kB3,
    kFuzzyEQ = kFuzzy | kEQ,
    kFuzzyLE = kFuzzy | kLE,
    kFuzzyGE = kFuzzy | kGE,
};

// copy operation
enum class CpyOp : std::uint8_t {
    kCopy = 1,
    kMove = 2,
    kMemCopy = 3, // memory copy
    kDeepCopy = 4,
};
}

